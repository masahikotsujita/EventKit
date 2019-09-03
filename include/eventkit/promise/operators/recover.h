//
// Created by Masahiko Tsujita on 2019/09/02.
//

#ifndef EVENTKIT_RECOVER_H
#define EVENTKIT_RECOVER_H

#include <eventkit/promise/operators/detail/recover-inl.h>
#include <type_traits>

namespace ek {
namespace promise {
namespace operators {

template <typename Handler>
auto recover(Handler&& handler) -> detail::RecoverOperator<Handler&&> {
    return detail::RecoverOperator<Handler&&>(std::forward<Handler>(handler));
}
    
}
}
}

#endif //EVENTKIT_RECOVER_H
