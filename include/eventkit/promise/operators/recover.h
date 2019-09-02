//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_RECOVER_H
#define EVENTKIT_RECOVER_H

#include <type_traits>
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
        auto pCore = std::make_shared<ek::promise::detail::RecoverTransformationCore<T, E, F, std::decay_t<Handler>>>(std::forward<std::decay_t<Handler>>(m_handler));
        promise.pipe(pCore->asHandler());
        return Promise<T, F>(pCore->asCore());
    }

private:
    Handler m_handler;
    
};
    
}

template <typename Handler>
auto recover(Handler&& handler) -> detail::RecoverOperator<Handler&&> {
    return detail::RecoverOperator<Handler&&>(std::forward<Handler>(handler));
}
    
}
}
}

#endif //EVENTKIT_RECOVER_H
