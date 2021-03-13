//
// Created by Tsujita Masahiko on 2020/09/02.
//

#ifndef EVENTKIT_MAKE_PROMISE_INL_H
#define EVENTKIT_MAKE_PROMISE_INL_H

#include <eventkit/promise/detail/make_promise.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/detail/PromiseCore.h>

namespace ek {
namespace promise {
namespace detail {

template<typename T, typename E>
ek::promise::Promise<T, E> make_promise(const common::IntrusivePtr<PromiseCore<T, E>>& pCore) {
    return ek::promise::Promise<T, E>(ek::promise::Promise<T, E>::with_core, pCore);
}

}
}
}

#endif //EVENTKIT_MAKE_PROMISE_H
