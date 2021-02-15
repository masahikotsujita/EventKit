//
// Created by Masahiko Tsujita on 2019-08-06.
//

#ifndef EVENTKIT_RUNLOOP_H
#define EVENTKIT_RUNLOOP_H

#include <list>
#include <eventkit/common/Condition.h>
#include <eventkit/common/IntrusivePtr.h>

namespace ek {
namespace dispatch {

class RunLoopSource;

class RunLoop final {
public:

    RunLoop() = default;

    ~RunLoop() = default;

    void run();

    void addSource(const ek::common::IntrusivePtr<RunLoopSource>& pQueue);

    void removeSource(RunLoopSource* pQueue);

public:

    RunLoop(const RunLoop&) = delete;

    RunLoop& operator=(const RunLoop&) = delete;

private:
    ek::common::Condition m_condition;
    std::list<ek::common::IntrusivePtr<RunLoopSource>> m_sources;

};

}
}

#endif //EVENTKIT_RUNLOOP_H
