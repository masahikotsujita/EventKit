//
// Created by Masahiko Tsujita on 2019-08-13.
//

#include <eventkit/dispatch/DispatchQueue.h>

namespace ek {
namespace dispatch {

void DispatchQueue::dispatchItemAsync(const ek::common::intrusive_ptr<DispatchItem>& pTask) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(pTask);
    m_pRunLoop->signal();
}

void DispatchQueue::fire() {
    bool isEmpty = false;
    do {
        ek::common::intrusive_ptr<DispatchItem> item = nullptr;
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