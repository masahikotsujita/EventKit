//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISE_H
#define EVENTKIT_PROMISE_H

#include <memory>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/detail/PromiseCore.h>
#include <eventkit/promise/detail/ThenTransformation.h>
#include <eventkit/promise/detail/RecoverTransformation.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class Promise {
private:
    using Core = detail::PromiseCore<T, E>;

    template <typename U, typename F>
    friend class ::ek::promise::Promise;

    template <typename U, typename V, typename W, typename Handler>
    friend class ::ek::promise::detail::ThenTransformation;

    template <typename U, typename V, typename W, typename Handler>
    friend class ::ek::promise::detail::RecoverTransformation;

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
        auto pTransformedCore = std::make_shared<detail::PromiseCore<U, E>>();
        auto pTransformation = std::make_shared<detail::ThenTransformation<T, E, U, std::decay_t<ThenHandler>>>(pTransformedCore, std::forward<ThenHandler>(handler));
        m_pCore->addHandler(pTransformation);
        return Promise<U, E>(pTransformedCore);
    }

    template <typename RecoverHandler>
    auto recover(RecoverHandler&& handler) const -> Promise<T, typename std::result_of_t<RecoverHandler(E)>::Error> {
        using F = typename std::result_of_t<RecoverHandler(E)>::Error;
        auto pTransformedCore = std::make_shared<detail::PromiseCore<T, F>>();
        auto pTransformation = std::make_shared<detail::RecoverTransformation<T, E, F, std::decay_t<RecoverHandler>>>(pTransformedCore, std::forward<RecoverHandler>(handler));
        m_pCore->addHandler(pTransformation);
        return Promise<T, F>(pTransformedCore);
    }

    template <typename FinallyHandler>
    auto finally(FinallyHandler&& handler) const -> Promise<T, E> {
        addHandler(detail::make_function_handler<T, E>(std::forward<FinallyHandler>(handler)));
        return Promise<T, E>(m_pCore);
    }

private:

    void addHandler(const std::shared_ptr<detail::ResultHandler<T, E>>& handler) const {
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
