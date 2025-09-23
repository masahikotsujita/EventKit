//
// Created by Masahiko Tsujita on 2019-08-13.
//

#ifndef EVENTKIT_DISPATCHQUEUE_INL_H
#define EVENTKIT_DISPATCHQUEUE_INL_H

#include <eventkit/dispatch/DispatchQueue.h>
#include <eventkit/dispatch/detail/FunctionDispatchItem.h>

namespace ek {
namespace dispatch {

template <typename F>
void DispatchQueue::dispatchAsync(F&& function) {
    dispatchItemAsync(ek::common::make_intrusive<detail::FunctionDispatchItem<std::decay_t<F>>>(std::forward<F>(function)));
}

}
}

#endif //EVENTKIT_DISPATCHQUEUE_INL_H
