//
// Created by Tsujita Masahiko on 2020/09/13.
//

#ifndef EVENTKIT_DISPATCHFUNCTIONITEM_INL_H
#define EVENTKIT_DISPATCHFUNCTIONITEM_INL_H

#include <eventkit/dispatch/detail/FunctionDispatchItem.h>
#include <eventkit/common/Allocator.h>

namespace ek {
namespace dispatch {
namespace detail {

template <typename Function>
template <typename F>
FunctionDispatchItem<Function>::FunctionDispatchItem(ek::common::Allocator* pA, F&& function)
    : DispatchItem(pA)
    , m_function(std::forward<F>(function)) {
}

template <typename Function>
void FunctionDispatchItem<Function>::run() {
    m_function();
}

}
}
}

#endif //EVENTKIT_DISPATCHFUNCTIONITEM_INL_H
