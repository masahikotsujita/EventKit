//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISE_H
#define EVENTKIT_PROMISE_H

#include <memory>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/detail/PromiseCore.h>

namespace ek {
namespace promise {

template <typename T, typename E>
class Promise;

namespace detail {
template <typename T, typename E>
ek::promise::Promise<T, E> make_promise(const std::shared_ptr<ek::promise::detail::PromiseCore<T, E>>& pCore);
}

template <typename T, typename E>
class Promise {
private:
    using Core = detail::PromiseCore<T, E>;
    
    template <typename U, typename F>
    friend ek::promise::Promise<U, F> detail::make_promise(const std::shared_ptr<ek::promise::detail::PromiseCore<U, F>>& pCore);

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

    void pipe(const std::shared_ptr<ResultObserver<T, E>>& handler) const {
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

#include <eventkit/promise/detail/Promise-inl.h>

#endif //EVENTKIT_PROMISE_H
