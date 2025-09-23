//
// Created by Masahiko Tsujita on 2019-08-13.
//

#include <eventkit/dispatch/DispatchQueue.h>
#include <eventkit/common/Condition.h>
#include <cstdio>

namespace ek {
namespace dispatch {

thread_local DispatchQueue* g_pCurrentDispatchQueue = nullptr;

DispatchQueue::DispatchQueue(ek::common::Allocator* pA)
    : m_pA(pA)
    , m_intrusiveObjectMixin(deleteCallback, this) {
}

void DispatchQueue::dispatchItemAsync(const ek::common::IntrusivePtr<DispatchItem>& pTask) {
    getCondition()->mutateValue([&](uint64_t& count, bool* pShouldNotify) {
        m_queue.push(pTask);
        count += 1;
        *pShouldNotify = true;
    });
}

void DispatchQueue::fire() {
    ek::common::IntrusivePtr<DispatchItem> item = nullptr;
        getCondition()->mutateValue([&](uint64_t& count, bool*) {
            if (!m_queue.empty()) {
                item = m_queue.front();
                m_queue.pop();
                count -= 1;
            }
        });
    if (item != nullptr) {
        item->run();
    }
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

DispatchQueue* getCurrentDispatchQueue() {
    return g_pCurrentDispatchQueue;
}

void setCurrentDispatchQueue(DispatchQueue* pDispatchQueue) {
    g_pCurrentDispatchQueue = pDispatchQueue;
}

}
}