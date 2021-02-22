//
// Created by Masahiko Tsujita on 2019-08-08.
//

#include <algorithm>
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/dispatch/RunLoopSource.h>

namespace ek {
namespace dispatch {

void RunLoop::run() {
    do {
        ek::common::Condition<uint64_t>::LockHandle lock;
        do {
            auto pos = m_sources.begin();
            auto end = m_sources.end();
            if (pos == end) { return; }
            for (; pos != end; ++pos) {
                const ek::common::IntrusivePtr<RunLoopSource>& pSource = *pos;
                pSource->fire();
            }
            if (m_condition.getValue(&lock) > 0) {
                lock.unlock();
            } else {
                break;
            }
        } while (true);
        m_condition.wait([&](uint64_t count){ return count > 0; }, lock);
    } while (true);
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
