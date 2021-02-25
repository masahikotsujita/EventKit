//
// Created by Masahiko Tsujita on 2019-08-08.
//

#include <algorithm>
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/dispatch/EventSource.h>

namespace ek {
namespace dispatch {

void EventLoop::run() {
    do {
        ek::common::Condition<uint64_t>::LockHandle lock;
        do {
            auto pos = m_sources.begin();
            auto end = m_sources.end();
            if (pos == end) { return; }
            for (; pos != end; ++pos) {
                const ek::common::IntrusivePtr<EventSource>& pSource = *pos;
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

void EventLoop::addSource(const ek::common::IntrusivePtr<EventSource>& pSource) {
    if (pSource == nullptr) {
        return;
    }
    m_sources.push_back(pSource);
    pSource->setCondition(&m_condition);
}

void EventLoop::removeSource(EventSource* pSource) {
    auto pos = std::find_if(m_sources.begin(), m_sources.end(), [&](const auto& s){ return s.get() == pSource; });
    if (pos != m_sources.end()) {
        (*pos)->setCondition(nullptr);
        auto itr = m_sources.erase(pos);
    }
}

}
}
