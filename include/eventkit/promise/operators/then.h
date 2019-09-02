//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_THEN_H
#define EVENTKIT_THEN_H

#include <eventkit/promise/operators/detail/then-inl.h>
#include <type_traits>

namespace ek {
namespace promise {
namespace operators {

template <typename Handler>
auto then(Handler&& handler) -> detail::ThenOperator<Handler&&> {
    return detail::ThenOperator<Handler&&>(std::forward<Handler>(handler));
}

}
}
}

#endif //EVENTKIT_THEN_H
