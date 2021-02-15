//
// Created by Masahiko Tsujita on 2019-08-13.
//

#include <eventkit/dispatch/DispatchQueue.h>
#include <eventkit/common/Condition.h>

namespace ek {
namespace dispatch {

DispatchQueue::DispatchQueue(ek::common::Allocator* pA)
    : m_pA(pA)
    , m_intrusiveObjectMixin(deleteCallback, this) {
}

void DispatchQueue::dispatchItemAsync(const ek::common::IntrusivePtr<DispatchItem>& pTask) {
    getCondition()->modify([&](bool& isOpen){
        m_queue.push(pTask);
        isOpen = true;
    });
}

void DispatchQueue::fire() {
    bool isEmpty = false;
    do {
        ek::common::IntrusivePtr<DispatchItem> item = nullptr;
        {
            getCondition()->wait([&](bool& isOpen){
                if (!m_queue.empty()) {
                    item = m_queue.front();
                    m_queue.pop();
                }
                isEmpty = m_queue.empty();
                isOpen = !isEmpty;
            });
        }
        if (item != nullptr) {
            item->run();
        }
    } while (!isEmpty);
}

void DispatchQueue::ref() {
    m_intrusiveObjectMixin.ref();
}

void DispatchQueue::unref() {
    m_intrusiveObjectMixin.unref();
}

void DispatchQueue::deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext) {
    auto* pThis = static_cast<DispatchQueue*>(pContext);
    pThis->m_pA->destroy(pThis);
}

}
}