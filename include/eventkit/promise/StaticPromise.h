//
// Created by Masahiko Tsujita on 2019/09/01.
//

#ifndef EVENTKIT_STATICPROMISE_H
#define EVENTKIT_STATICPROMISE_H

#include <eventkit/promise/Result.h>
#include <eventkit/promise/Promise.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class StaticPromise final {
public:
    using Value = T;
    using Error = E;
    
    explicit StaticPromise<T, E>(const Result<T, E>& result)
        : m_result(result) {
    }
    
    explicit StaticPromise<T, E>(Result<T, E>&& result)
        : m_result(std::move(result)) {
    }
    
    template <typename ...Args>
    static StaticPromise<T, E> value(Args&& ...args) {
        return StaticPromise<T, E>(Result<T, E>::succeeded(std::forward<Args>(args)...));
    }
    
    template <typename ...Args>
    static StaticPromise<T, E> error(Args&& ...args) {
        return StaticPromise<T, E>(Result<T, E>::failed(std::forward<Args>(args)...));
    }
    
    template <typename ThenHandler>
    auto then(ThenHandler&& handler) const -> Promise<typename std::result_of_t<ThenHandler(T)>::Value, E> {
        using U = typename std::result_of_t<ThenHandler(T)>::Value;
        auto pCore = std::make_shared<detail::ThenTransformationCore<T, E, U, std::decay_t<ThenHandler>>>(std::forward<ThenHandler>(handler));
        pipe(pCore->asHandler());
        return Promise<U, E>(pCore->asCore());
    }
    
    template <typename RecoverHandler>
    auto recover(RecoverHandler&& handler) const -> Promise<T, typename std::result_of_t<RecoverHandler(E)>::Error> {
        using F = typename std::result_of_t<RecoverHandler(E)>::Error;
        auto pCore = std::make_shared<detail::RecoverTransformationCore<T, E, F, std::decay_t<RecoverHandler>>>(std::forward<RecoverHandler>(handler));
        pipe(pCore->asHandler());
        return Promise<T, F>(pCore->asCore());
    }
    
    template <typename DoneHandler>
    StaticPromise& done(DoneHandler&& handler) const {
        pipe(detail::make_function_observer<T, E>(std::forward<DoneHandler>(handler)));
        return *this;
    }
    
    void pipe(const std::shared_ptr<ResultObserver<T, E>>& handler) const {
        handler->onResult(m_result);
    }

private:
    Result<T, E> m_result;

};

}
}

#endif //EVENTKIT_STATICPROMISE_H
