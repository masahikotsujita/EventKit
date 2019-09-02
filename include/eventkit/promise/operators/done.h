//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_DONE_H
#define EVENTKIT_DONE_H

#include <type_traits>

namespace ek {
namespace promise {
namespace operators {

namespace detail {

template <typename Handler>
class DoneOperator {
public:
    template <typename F>
    explicit DoneOperator(F&& handler)
        : m_handler(std::forward<F>(handler)) {
    }
    
    template <typename PromiseType>
    decltype(auto) operator()(PromiseType&& promise) && {
        using T = typename PromiseType::Value;
        using E = typename PromiseType::Error;
        promise.pipe(ek::promise::detail::make_function_observer<T, E>(std::forward<Handler>(m_handler)));
        return std::forward<PromiseType>(promise);
    }

private:
    Handler m_handler;
    
};
    
}

template <typename Handler>
auto done(Handler&& handler) -> detail::DoneOperator<Handler&&> {
    return detail::DoneOperator<Handler&&>(std::forward<Handler>(handler));
}
    
}
}
}

#endif //EVENTKIT_DONE_H
