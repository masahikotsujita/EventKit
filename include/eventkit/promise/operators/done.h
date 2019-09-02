//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_DONE_H
#define EVENTKIT_DONE_H

#include <eventkit/promise/operators/detail/done-inl.h>
#include <type_traits>

namespace ek {
namespace promise {
namespace operators {

template <typename Handler>
auto done(Handler&& handler) -> detail::DoneOperator<Handler&&> {
    return detail::DoneOperator<Handler&&>(std::forward<Handler>(handler));
}
    
}
}
}

#endif //EVENTKIT_DONE_H
