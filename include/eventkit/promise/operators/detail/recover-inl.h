//
// Created by Masahiko Tsujita on 2019/09/03.
//

#ifndef EVENTKIT_RECOVER_INL_H
#define EVENTKIT_RECOVER_INL_H

#include <eventkit/promise/detail/RecoverTransformationCore.h>
#include <eventkit/promise/Promise.h>

namespace ek {
namespace promise {
namespace operators {
namespace detail {

template <typename Handler>
class RecoverOperator {
public:
    template <typename F>
    explicit RecoverOperator(F&& handler)
        : m_handler(std::forward<F>(handler)) {
    }
    
    template <typename PromiseType>
    auto operator()(PromiseType&& promise) && {
        using T = typename PromiseType::Value;
        using E = typename PromiseType::Error;
        using F = typename std::result_of_t<Handler(E)>::Error;
        auto pCore = ek::common::make_intrusive<ek::promise::detail::RecoverTransformationCore<T, E, F, std::decay_t<Handler>>>(std::forward<std::decay_t<Handler>>(m_handler));
        promise.pipe(pCore->asHandler());
        return ek::promise::detail::make_promise(pCore->asCore());
    }

private:
    Handler m_handler;
    
};

}
}
}
}

#endif //EVENTKIT_RECOVER_INL_H
