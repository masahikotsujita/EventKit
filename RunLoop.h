//
// Created by Masahiko Tsujita on 2019-08-06.
//

#ifndef EVENTKIT_RUNLOOP_H
#define EVENTKIT_RUNLOOP_H

#include <list>
#include "Semaphore.h"

namespace ek {

class DispatchQueue;

class RunLoop final {
public:

    friend class DispatchQueue;

    RunLoop() = default;

    ~RunLoop() = default;

    void run();

    void addDispatchQueue(const std::shared_ptr<DispatchQueue>& pQueue);

    void removeDispatchQueue(DispatchQueue* pQueue);

public:
    RunLoop(const RunLoop&) = delete;
    RunLoop& operator=(const RunLoop&) = delete;

private:
    void signal();

private:
    detail::Semaphore m_semaphore;
    std::list<std::shared_ptr<DispatchQueue>> m_dispatchQueues;

};

}

#endif //EVENTKIT_RUNLOOP_H
