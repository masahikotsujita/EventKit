//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_PROMISE_INL_H
#define EVENTKIT_PROMISE_INL_H

#include <eventkit/promise/detail/PromiseCore.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
ek::promise::Promise<T, E> make_promise(const std::shared_ptr<ek::promise::detail::PromiseCore<T, E>>& pCore) {
    return ek::promise::Promise<T, E>(pCore);
}

}
}
}

#endif //EVENTKIT_PROMISE_INL_H
