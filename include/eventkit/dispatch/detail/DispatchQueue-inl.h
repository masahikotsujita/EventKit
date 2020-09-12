//
// Created by Masahiko Tsujita on 2019-08-13.
//

#ifndef EVENTKIT_DISPATCHQUEUE_INL_H
#define EVENTKIT_DISPATCHQUEUE_INL_H

#include <eventkit/dispatch/DispatchQueue.h>
#include <eventkit/common/Allocator.h>
#include <eventkit/dispatch/DispatchFunctionItem.h>

namespace ek {
namespace dispatch {

template <typename F>
void DispatchQueue::dispatchAsync(ek::common::Allocator* pA, F&& function) {
    dispatchItemAsync(ek::common::make_intrusive<DispatchFunctionItem<std::decay_t<F>>>(pA, pA, std::forward<F>(function)));
}

}
}

#endif //EVENTKIT_DISPATCHQUEUE_INL_H
