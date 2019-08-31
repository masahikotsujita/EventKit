//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISE_H
#define EVENTKIT_PROMISE_H

#include <memory>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/detail/PromiseCore.h>
#include <eventkit/promise/detail/ThenTransformationCore.h>
#include <eventkit/promise/detail/RecoverTransformationCore.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class Promise {
private:
    using Core = detail::PromiseCore<T, E>;

    template <typename U, typename F>
    friend class ::ek::promise::Promise;

    template <typename U, typename V, typename W, typename Handler>
    friend class ::ek::promise::detail::ThenTransformationCore;

    template <typename U, typename V, typename W, typename Handler>
    friend class ::ek::promise::detail::RecoverTransformationCore;

public:
    using Value = T;
    using Error = E;

    template <typename StartHandler>
    explicit Promise(const StartHandler& startHandler) {
        auto pCore = std::make_shared<Core>();
        Resolver<T, E> resolver(pCore);
        startHandler(resolver);
        m_pCore = pCore;
    }

    template <typename ThenHandler>
    auto then(ThenHandler&& handler) const -> Promise<typename std::result_of_t<ThenHandler(T)>::Value, E> {
        using U = typename std::result_of_t<ThenHandler(T)>::Value;
        auto pCore = std::make_shared<detail::ThenTransformationCore<T, E, U, std::decay_t<ThenHandler>>>(std::forward<ThenHandler>(handler));
        m_pCore->addHandler(pCore->asHandler());
        return Promise<U, E>(pCore->asCore());
    }

    template <typename RecoverHandler>
    auto recover(RecoverHandler&& handler) const -> Promise<T, typename std::result_of_t<RecoverHandler(E)>::Error> {
        using F = typename std::result_of_t<RecoverHandler(E)>::Error;
        auto pCore = std::make_shared<detail::RecoverTransformationCore<T, E, F, std::decay_t<RecoverHandler>>>(std::forward<RecoverHandler>(handler));
        m_pCore->addHandler(pCore->asHandler());
        return Promise<T, F>(pCore->asCore());
    }

    template <typename DoneHandler>
    auto done(DoneHandler&& handler) const -> Promise<T, E> {
        addHandler(detail::make_function_handler<T, E>(std::forward<DoneHandler>(handler)));
        return Promise<T, E>(m_pCore);
    }

private:

    void addHandler(const std::shared_ptr<ResultHandler<T, E>>& handler) const {
        m_pCore->addHandler(handler);
    }

private:

    explicit Promise(const std::shared_ptr<Core>& pCore)
        : m_pCore(pCore) {
    }

private:
    std::shared_ptr<Core> m_pCore;

};

}
}

#endif //EVENTKIT_PROMISE_H
