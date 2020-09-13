//
// Created by Tsujita Masahiko on 2020/09/13.
//

#ifndef EVENTKIT_FUNCTIONDISPATCHITEM_H
#define EVENTKIT_FUNCTIONDISPATCHITEM_H

#include <eventkit/dispatch/DispatchItem.h>

namespace ek {
namespace dispatch {

template <typename Function>
class FunctionDispatchItem : public DispatchItem {
public:

    template <typename F>
    explicit FunctionDispatchItem(ek::common::Allocator* pA, F&& function);

    ~FunctionDispatchItem() override = default;

    void run() override;

private:
    Function m_function;

};

}
}

#include <eventkit/dispatch/detail/DispatchFunctionItem-inl.h>

#endif //EVENTKIT_FUNCTIONDISPATCHITEM_H
