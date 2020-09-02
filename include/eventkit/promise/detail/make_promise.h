//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_MAKE_PROMISE_H
#define EVENTKIT_MAKE_PROMISE_H

#include <eventkit/promise/detail/PromiseCore.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
ek::promise::Promise<T, E> make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<T, E>>& pCore) {
    return ek::promise::Promise<T, E>(pCore);
}

}
}
}

#endif //EVENTKIT_MAKE_PROMISE_H
