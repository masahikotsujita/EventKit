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
namespace operators {
namespace detail {
template <typename Handler>
class ThenOperator;

template <typename Handler>
class RecoverOperator;
}
}

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
    
    template <typename Handler>
    friend class ::ek::promise::operators::detail::ThenOperator;
    
    template <typename Handler>
    friend class ::ek::promise::operators::detail::RecoverOperator;

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

#endif //EVENTKIT_PROMISE_H
