//
// Created by Tsujita Masahiko on 2020/09/02.
//

#ifndef EVENTKIT_PROMISE_INL_H
#define EVENTKIT_PROMISE_INL_H

#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/make_promise.h>
#include <eventkit/promise/detail/FunctionResultHandler.h>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/detail/ThenTransformationCore.h>
#include <eventkit/promise/detail/RecoverTransformationCore.h>

namespace ek {
namespace promise {

template<typename T, typename E>
template<typename StartHandler>
Promise<T, E>::Promise(ek::common::Allocator* pA, const StartHandler& startHandler) {
    auto pCore = ek::common::make_intrusive<Core>(pA, pA);
    Resolver<T, E> resolver(pCore);
    startHandler(resolver);
    m_pCore = pCore;
}

template<typename T, typename E>
template<typename... Args>
Promise<T, E> Promise<T, E>::value(ek::common::Allocator* pA, Args&& ... args) {
    auto pCore = ek::common::make_intrusive<Core>(pA, pA);
    pCore->onResult(Result<T, E>::succeeded(std::forward<Args>(args)...));
    return Promise(pCore);
}

template<typename T, typename E>
template<typename... Args>
Promise<T, E> Promise<T, E>::error(ek::common::Allocator* pA, Args&& ... args) {
    auto pCore = ek::common::make_intrusive<Core>(pA, pA);
    pCore->onResult(Result<T, E>::failed(std::forward<Args>(args)...));
    return Promise(pCore);
}

template<typename T, typename E>
template<typename Handler>
auto Promise<T, E>::then(ek::common::Allocator* pA, Handler&& handler) const -> Promise<typename std::result_of_t<Handler(T)>::Value, E> {
    using U = typename std::result_of_t<Handler(T)>::Value;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::ThenTransformationCore<T, E, U, std::decay_t<Handler>>>(pA, pA, std::forward<std::decay_t<Handler>>(handler));
    done(pCore->asHandler());
    return ek::promise::detail::make_promise(pCore->asCore());
}

template<typename T, typename E>
template<typename Handler>
auto Promise<T, E>::recover(ek::common::Allocator* pA, Handler&& handler) const -> Promise<T, typename std::result_of_t<Handler(E)>::Error> {
    using F = typename std::result_of_t<Handler(E)>::Error;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::RecoverTransformationCore<T, E, F, std::decay_t<Handler>>>(pA, pA, std::forward<std::decay_t<Handler>>(handler));
    done(pCore->asHandler());
    return ek::promise::detail::make_promise(pCore->asCore());
}

template<typename T, typename E>
template<typename Handler>
Promise<T, E> Promise<T, E>::done(ek::common::Allocator* pA, Handler&& handler) const {
    done(ek::promise::detail::make_function_observer<T, E>(pA, std::forward<Handler>(handler)));
    return *this;
}

template<typename T, typename E>
Promise<T, E> Promise<T, E>::done(const common::IntrusivePtr<ResultHandler<T, E>>& handler) const {
    m_pCore->addHandler(handler);
    return *this;
}

template<typename T, typename E>
Promise<T, E>::Promise(const common::IntrusivePtr<Core>& pCore)
        : m_pCore(pCore) {
}

}
}

#endif //EVENTKIT_PROMISE_INL_H
