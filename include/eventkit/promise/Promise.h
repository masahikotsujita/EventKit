//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISE_H
#define EVENTKIT_PROMISE_H

#include <eventkit/common/Allocator.h>
#include <eventkit/common/IntrusivePtr.h>

namespace ek {
namespace promise {
template <typename T, typename E>
class ResultHandler;
template <typename T, typename E>
class Promise;
namespace detail {
template <typename T, typename E>
class PromiseCore;
template <typename T, typename E>
ek::promise::Promise<T, E> make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<T, E>>& pCore);
}
}
}

namespace ek {
namespace promise {

template <typename T, typename E>
class Promise {
public:
    using Value = T;
    using Error = E;

    template <typename StartHandler>
    explicit Promise(const StartHandler& startHandler);

    template <typename ...Args>
    static Promise value(Args&& ...args);

    template <typename ...Args>
    static Promise error(Args&& ...args);

    template <typename Handler>
    Promise<typename std::result_of_t<Handler(T)>::Value, E> then(Handler&& handler) const;

    template <typename Handler>
    Promise<T, typename std::result_of_t<Handler(E)>::Error> recover(Handler&& handler) const;

    template <typename Handler>
    Promise done(Handler&& handler) const;

    Promise done(const ek::common::IntrusivePtr<ResultHandler<T, E>>& handler, void*) const;

private:
    using Core = detail::PromiseCore<T, E>;

    template <typename U, typename F>
    friend ek::promise::Promise<U, F> detail::make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<U, F>>& pCore);

    explicit Promise(const ek::common::IntrusivePtr<Core>& pCore, void*);

private:
    ek::common::IntrusivePtr<Core> m_pCore;

};

}
}

#include <eventkit/promise/detail/Promise-inl.h>

#endif //EVENTKIT_PROMISE_H
