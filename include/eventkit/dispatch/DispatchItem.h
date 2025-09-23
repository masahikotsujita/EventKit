//
// Created by Tsujita Masahiko on 2020/09/12.
//

#ifndef EVENTKIT_DISPATCHITEM_H
#define EVENTKIT_DISPATCHITEM_H

#include <eventkit/common/IntrusiveObject.h>

namespace ek {
namespace dispatch {

class DispatchItem : public ek::common::IntrusiveObject {
public:
    explicit DispatchItem()
        : ek::common::IntrusiveObject() {}

    virtual ~DispatchItem() = default;

    virtual void run() = 0;

};

}
}

#endif //EVENTKIT_DISPATCHITEM_H
