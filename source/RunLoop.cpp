//
// Created by Masahiko Tsujita on 2019-08-08.
//

#include <algorithm>
#include <eventkit/RunLoop.h>
#include <eventkit/DispatchQueue.h>

namespace ek {

void RunLoop::run() {
    bool isFinished = false;
    do {
        { // dispatch all tasks
            auto pos = m_dispatchQueues.begin();
            auto end = m_dispatchQueues.end();
            for (; pos != end; ++pos) {
                const std::shared_ptr<DispatchQueue>& pTaskQueue = *pos;
                pTaskQueue->fire();
            }
        }
        // wait for next event
        if (m_dispatchQueues.empty()) {
            isFinished = true;
        } else {
            m_semaphore.wait();
        }
    } while (!isFinished);
}

void RunLoop::addDispatchQueue(const std::shared_ptr<DispatchQueue>& pQueue) {
    if (pQueue == nullptr) {
        return;
    }
    m_dispatchQueues.push_back(pQueue);
    pQueue->setRunLoop(this);
}

void RunLoop::removeDispatchQueue(DispatchQueue* pQueue) {
    auto pos = std::find_if(m_dispatchQueues.begin(), m_dispatchQueues.end(), [&](const auto& s){ return s.get() == pQueue; });
    if (pos != m_dispatchQueues.end()) {
        auto itr = m_dispatchQueues.erase(pos);
    }
}

void RunLoop::signal() {
    m_semaphore.notify();
}

}
