//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISE_H
#define EVENTKIT_PROMISE_H

#include <eventkit/common/Allocator.h>
#include <eventkit/common/IntrusivePtr.h>
#include <eventkit/promise/Resolver.h>

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

    Promise();

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

    Promise pipe(const ek::common::IntrusivePtr<ResultHandler<T, E>>& handler) const;

    Resolver<T, E> getResolver() {
        return Resolver<T, E>(m_pCore);
    }

    struct promise_type {

        std::suspend_never initial_suspend() noexcept { return {}; }

        std::suspend_never final_suspend() noexcept { return {}; }

        std::suspend_never return_value(const T& value) {
            m_promise.getResolver().fulfill(value);
            return {};
        }

        void unhandled_exception() {
            auto pException = std::current_exception();
            if constexpr (std::is_same_v<E, std::exception_ptr>) {
                m_promise.getResolver().reject(pException);
            } else {
                std::terminate();
            }
        }

        auto get_return_object() {
            return m_promise;
        };

        template <typename U>
        auto await_transform(ek::promise::Promise<U, E> promise) {
            struct Unit {};
            struct Awaiter {
                ek::promise::Promise<U, E> m_promise;

                bool await_ready() const {
                    return m_promise.m_pCore->isResolved();
                }

                void await_suspend(std::coroutine_handle<promise_type> handle) {
                    m_promise.then([handle](const U&){
                        handle.resume();
                        return ek::promise::Promise<Unit, E>::value();
                    });
                }

                const U& await_resume() {
                    if (m_promise.m_pCore->getResult().getType() == ek::promise::ResultType::succeeded) {
                        return m_promise.m_pCore->getResult().getValue();
                    } else {
                        if constexpr (std::is_same_v<E, std::exception_ptr>) {
                            std::rethrow_exception(m_promise.m_pCore->getResult().getError());
                        } else {
                            std::terminate();
                        }
                    }
                }
            };

            return Awaiter { promise };
        }

    private:
        Promise m_promise {};

    };

private:
    using Core = detail::PromiseCore<T, E>;

    template <typename U, typename F>
    friend class ek::promise::Promise;

    template <typename U, typename F>
    friend ek::promise::Promise<U, F> detail::make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<U, F>>& pCore);

    struct with_core_t {};
    static constexpr with_core_t with_core = with_core_t {};

    explicit Promise(with_core_t, const ek::common::IntrusivePtr<Core>& pCore);

private:
    ek::common::IntrusivePtr<Core> m_pCore;

};

}
}

#include <eventkit/promise/detail/Promise-inl.h>

#endif //EVENTKIT_PROMISE_H
