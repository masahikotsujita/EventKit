//
// Created by Masahiko Tsujita on 2019-08-08.
//

#include <algorithm>
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/dispatch/RunLoopSource.h>

namespace ek {
namespace dispatch {

void RunLoop::run() {
    bool isFinished = false;
    do {
        m_condition.wait([&](bool& isOpen){});
        { // dispatch all tasks
            auto pos = m_sources.begin();
            auto end = m_sources.end();
            for (; pos != end; ++pos) {
                const ek::common::IntrusivePtr<RunLoopSource>& pTaskQueue = *pos;
                pTaskQueue->fire();
            }
        }
        // wait for next event
        if (m_sources.empty()) {
            isFinished = true;
        }
    } while (!isFinished);
}

void RunLoop::addSource(const ek::common::IntrusivePtr<RunLoopSource>& pQueue) {
    if (pQueue == nullptr) {
        return;
    }
    m_sources.push_back(pQueue);
    pQueue->setCondition(&m_condition);
}

void RunLoop::removeSource(RunLoopSource* pQueue) {
    auto pos = std::find_if(m_sources.begin(), m_sources.end(), [&](const auto& s){ return s.get() == pQueue; });
    if (pos != m_sources.end()) {
        (*pos)->setCondition(nullptr);
        auto itr = m_sources.erase(pos);
    }
}

}
}
