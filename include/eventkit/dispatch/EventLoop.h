//
// Created by Masahiko Tsujita on 2019-08-06.
//

#ifndef EVENTKIT_EVENTLOOP_H
#define EVENTKIT_EVENTLOOP_H

#include <list>
#include <eventkit/common/Condition.h>
#include <eventkit/common/IntrusivePtr.h>

namespace ek {
namespace dispatch {

class EventSource;

class EventLoop final {
public:

    EventLoop() = default;

    ~EventLoop() = default;

    void run();

    void addSource(const ek::common::IntrusivePtr<EventSource>& pSource);

    void removeSource(EventSource* pSource);

public:

    EventLoop(const EventLoop&) = delete;

    EventLoop& operator=(const EventLoop&) = delete;

private:
    ek::common::Condition<uint64_t> m_condition { 0 };
    std::list<ek::common::IntrusivePtr<EventSource>> m_sources;

};

}
}

#endif //EVENTKIT_EVENTLOOP_H
