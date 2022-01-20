//
// Created by Tsujita Masahiko on 2020/09/02.
//

#ifndef EVENTKIT_PROMISE_INL_H
#define EVENTKIT_PROMISE_INL_H

#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/make_promise.h>
#include <eventkit/promise/detail/FunctionResultHandler.h>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/detail/BasicPromiseCore.h>
#include <eventkit/promise/detail/ThenTransformationPromiseCore.h>
#include <eventkit/promise/detail/RecoverTransformationPromiseCore.h>
#include <eventkit/common/AllocatorStack.h>

namespace ek {
namespace promise {

template<typename T, typename E>
template<typename StartHandler>
Promise<T, E>::Promise(const StartHandler& startHandler) {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    auto pCore = ek::common::make_intrusive<detail::BasicPromiseCore<T, E>>(pA, pA);
    Resolver<T, E> resolver(pCore);
    startHandler(resolver);
    m_pCore = pCore;
}

template<typename T, typename E>
template<typename... Args>
Promise<T, E> Promise<T, E>::value(Args&& ... args) {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    auto pCore = ek::common::make_intrusive<detail::BasicPromiseCore<T, E>>(pA, pA);
    pCore->resolve(Result<T, E>::succeeded(std::forward<Args>(args)...));
    return Promise(with_core, pCore);
}

template<typename T, typename E>
template<typename... Args>
Promise<T, E> Promise<T, E>::error(Args&& ... args) {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    auto pCore = ek::common::make_intrusive<detail::BasicPromiseCore<T, E>>(pA, pA);
    pCore->resolve(Result<T, E>::failed(std::forward<Args>(args)...));
    return Promise(with_core, pCore);
}

template<typename T, typename E>
template<typename Handler>
auto Promise<T, E>::then(Handler&& handler) const -> Promise<typename std::result_of_t<Handler(T)>::Value, E> {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    using U = typename std::result_of_t<Handler(T)>::Value;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::ThenTransformationPromiseCore<T, E, U, std::decay_t<Handler>>>(pA, pA, std::forward<std::decay_t<Handler>>(handler));
    pipe(pCore->asSrcResultHandler());
    return ek::promise::detail::make_promise(ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<U, E>>(pCore));
}

template<typename T, typename E>
template<typename Handler>
auto Promise<T, E>::recover(Handler&& handler) const -> Promise<T, typename std::result_of_t<Handler(E)>::Error> {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    using F = typename std::result_of_t<Handler(E)>::Error;
    auto pCore = ek::common::make_intrusive<ek::promise::detail::RecoverTransformationPromiseCore<T, E, F, std::decay_t<Handler>>>(pA, pA, std::forward<std::decay_t<Handler>>(handler));
    pipe(pCore->asSrcResultHandler());
    return ek::promise::detail::make_promise(ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<T, F>>(pCore));
}

template<typename T, typename E>
template<typename Handler>
Promise<T, E> Promise<T, E>::done(Handler&& handler) const {
    ek::common::Allocator* pA = ek::common::getCurrentAllocator();
    pipe(ek::promise::detail::make_function_observer<T, E>(pA, std::forward<Handler>(handler)));
    return *this;
}

template<typename T, typename E>
Promise<T, E> Promise<T, E>::pipe(const common::IntrusivePtr<ResultHandler<T, E>>& handler) const {
    m_pCore->addHandler(ek::dispatch::getCurrentDispatchQueue(), handler);
    return *this;
}

template<typename T, typename E>
Promise<T, E>::Promise(with_core_t, const common::IntrusivePtr<Core>& pCore)
        : m_pCore(pCore) {
}

}
}

#endif //EVENTKIT_PROMISE_INL_H
