//
// Created by Tsujita Masahiko on 2020/09/13.
//

#ifndef EVENTKIT_DISPATCHFUNCTIONITEM_H
#define EVENTKIT_DISPATCHFUNCTIONITEM_H

#include <eventkit/dispatch/DispatchItem.h>

namespace ek {
namespace dispatch {

template <typename Function>
class DispatchFunctionItem : public DispatchItem {
public:

    template <typename F>
    explicit DispatchFunctionItem(ek::common::Allocator* pA, F&& function);

    ~DispatchFunctionItem() override = default;

    void run() override;

private:
    Function m_function;

};

}
}

#include <eventkit/dispatch/detail/DispatchFunctionItem-inl.h>

#endif //EVENTKIT_DISPATCHFUNCTIONITEM_H
