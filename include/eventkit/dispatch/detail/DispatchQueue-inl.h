//
// Created by Masahiko Tsujita on 2019-08-13.
//

#ifndef EVENTKIT_DISPATCHQUEUE_INL_H
#define EVENTKIT_DISPATCHQUEUE_INL_H

#include <eventkit/common/Allocator.h>

namespace ek {
namespace dispatch {

template <typename Function>
template <typename F>
DispatchFunctionItem<Function>::DispatchFunctionItem(ek::common::Allocator* pA, F&& function)
    : DispatchItem(pA)
    , m_function(std::forward<F>(function)) {
}

template <typename Function>
void DispatchFunctionItem<Function>::run() {
    m_function();
}

template <typename F>
void DispatchQueue::dispatchAsync(ek::common::Allocator* pA, F&& function) {
    dispatchItemAsync(ek::common::make_intrusive<DispatchFunctionItem<std::decay_t<F>>>(pA, pA, std::forward<F>(function)));
}

}
}

#endif //EVENTKIT_DISPATCHQUEUE_INL_H
