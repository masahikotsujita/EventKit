//
// Created by Masahiko Tsujita on 2019/09/04.
//

#ifndef EVENTKIT_MAKE_PROMISE_H
#define EVENTKIT_MAKE_PROMISE_H

#include <eventkit/common/IntrusivePtr.h>

namespace ek {
namespace promise {
namespace detail {
template <typename T, typename E>
class Promise;
template <typename T, typename E>
class PromiseCore;
}
}
}

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
ek::promise::Promise<T, E> make_promise(const ek::common::IntrusivePtr<ek::promise::detail::PromiseCore<T, E>>& pCore);

}
}
}

#include <eventkit/promise/detail/make_promise-inl.h>

#endif //EVENTKIT_MAKE_PROMISE_H
