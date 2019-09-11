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
    
    void pipe(const ek::common::intrusive_ptr<ResultObserver<T, E>>& handler) const {
        handler->onResult(m_result);
    }

private:
    Result<T, E> m_result;

};

}
}

#endif //EVENTKIT_STATICPROMISE_H
