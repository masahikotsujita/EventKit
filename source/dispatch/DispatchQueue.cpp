//
// Created by Masahiko Tsujita on 2019-08-13.
//

#include <eventkit/dispatch/DispatchQueue.h>
#include <eventkit/dispatch/RunLoop.h>

namespace ek {
namespace dispatch {

DispatchQueue::DispatchQueue(ek::common::Allocator* pA)
    : ek::common::IntrusiveObject(pA)
    , m_pRunLoop(nullptr) {
}

void DispatchQueue::dispatchItemAsync(const ek::common::IntrusivePtr<DispatchItem>& pTask) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(pTask);
    m_pRunLoop->signal();
}

void DispatchQueue::fire() {
    bool isEmpty = false;
    do {
        ek::common::IntrusivePtr<DispatchItem> item = nullptr;
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            if (!m_queue.empty()) {
                item = m_queue.front();
                m_queue.pop();
            }
            isEmpty = m_queue.empty();
        }
        if (item != nullptr) {
            item->run();
        }
    } while (!isEmpty);
}

void DispatchQueue::setRunLoop(RunLoop* pRunLoop) {
    m_pRunLoop = pRunLoop;
}

}
}