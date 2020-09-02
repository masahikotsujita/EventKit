//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISE_H
#define EVENTKIT_PROMISE_H

#include <memory>
#include <eventkit/common/Allocator.h>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/detail/PromiseCore.h>
#include <eventkit/promise/detail/ThenTransformationCore.h>
#include <eventkit/promise/detail/RecoverTransformationCore.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class Promise;

namespace detail {
template <typename T, typename E>
ek::promise::Promise<T, E> make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<T, E>>& pCore);
}

template <typename T, typename E>
class Promise {
private:
    using Core = detail::PromiseCore<T, E>;
    
    template <typename U, typename F>
    friend ek::promise::Promise<U, F> detail::make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<U, F>>& pCore);

public:
    using Value = T;
    using Error = E;

    template <typename StartHandler>
    explicit Promise(ek::common::Allocator* pA, const StartHandler& startHandler) {
        auto pCore = ek::common::make_intrusive<Core>(pA, pA);
        Resolver<T, E> resolver(pCore);
        startHandler(resolver);
        m_pCore = pCore;
    }

    template <typename ...Args>
    static Promise value(ek::common::Allocator* pA, Args&& ...args) {
        auto pCore = ek::common::make_intrusive<Core>(pA, pA);
        pCore->onResult(Result<T, E>::succeeded(std::forward<Args>(args)...));
        return Promise(pCore);
    }

    template <typename ...Args>
    static Promise error(ek::common::Allocator* pA, Args&& ...args) {
        auto pCore = ek::common::make_intrusive<Core>(pA, pA);
        pCore->onResult(Result<T, E>::failed(std::forward<Args>(args)...));
        return Promise(pCore);
    }

    template <typename Handler>
    auto then(ek::common::Allocator* pA, Handler&& handler) const -> Promise<typename std::result_of_t<Handler(T)>::Value, E> {
        using U = typename std::result_of_t<Handler(T)>::Value;
        auto pCore = ek::common::make_intrusive<ek::promise::detail::ThenTransformationCore<T, E, U, std::decay_t<Handler>>>(pA, pA, std::forward<std::decay_t<Handler>>(handler));
        done(pCore->asHandler());
        return ek::promise::detail::make_promise(pCore->asCore());
    }

    template <typename Handler>
    auto recover(ek::common::Allocator* pA, Handler&& handler) const -> Promise<T, typename std::result_of_t<Handler(E)>::Error> {
        using F = typename std::result_of_t<Handler(E)>::Error;
        auto pCore = ek::common::make_intrusive<ek::promise::detail::RecoverTransformationCore<T, E, F, std::decay_t<Handler>>>(pA, pA, std::forward<std::decay_t<Handler>>(handler));
        done(pCore->asHandler());
        return ek::promise::detail::make_promise(pCore->asCore());
    }

    template <typename Handler>
    Promise done(ek::common::Allocator* pA, Handler&& handler) const {
        done(ek::promise::detail::make_function_observer<T, E>(pA, std::forward<Handler>(handler)));
        return *this;
    }

    Promise done(const ek::common::IntrusivePtr<ResultObserver<T, E>>& handler) const {
        m_pCore->addHandler(handler);
        return *this;
    }

private:

    explicit Promise(const ek::common::IntrusivePtr<Core>& pCore)
        : m_pCore(pCore) {
    }

private:
    ek::common::IntrusivePtr<Core> m_pCore;

};

}
}

#include <eventkit/promise/detail/make_promise.h>

#endif //EVENTKIT_PROMISE_H
