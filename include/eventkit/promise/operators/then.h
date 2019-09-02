//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_THEN_H
#define EVENTKIT_THEN_H

#include <type_traits>
#include <eventkit/promise/detail/ThenTransformationCore.h>
#include <eventkit/promise/Promise.h>

namespace ek {
namespace promise {
namespace operators {

namespace detail {

template <typename Handler>
class ThenOperator {
public:
    template <typename F>
    explicit ThenOperator(F&& handler)
        : m_handler(std::forward<F>(handler)) {
    }
    
    template <typename PromiseType>
    auto operator()(PromiseType&& promise) && {
        using T = typename PromiseType::Value;
        using E = typename PromiseType::Error;
        using U = typename std::result_of_t<Handler(T)>::Value;
        auto pCore = std::make_shared<ek::promise::detail::ThenTransformationCore<T, E, U, std::decay_t<Handler>>>(std::forward<std::decay_t<Handler>>(m_handler));
        promise.pipe(pCore->asHandler());
        return Promise<U, E>(pCore->asCore());
    }

private:
    Handler m_handler;

};

}

template <typename Handler>
auto then(Handler&& handler) -> detail::ThenOperator<Handler&&> {
    return detail::ThenOperator<Handler&&>(std::forward<Handler>(handler));
}

}
}
}

#endif //EVENTKIT_THEN_H
