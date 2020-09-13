//
// Created by Masahiko Tsujita on 2019-08-12.
//

#ifndef EVENTKIT_DISPATCHQUEUE_H
#define EVENTKIT_DISPATCHQUEUE_H

#include <mutex>
#include <list>
#include <queue>
#include <eventkit/common/Allocator.h>
#include <eventkit/common/IntrusivePtr.h>
#include <eventkit/common/IntrusiveObject.h>

namespace ek {
namespace dispatch {

class RunLoop;
class DispatchItem;

class DispatchQueue : public ek::common::IntrusiveObject {
public:

    friend class RunLoop;

    explicit DispatchQueue(ek::common::Allocator* pA);

    template <typename F>
    void dispatchAsync(ek::common::Allocator* pA, F&& function);

private:

    void dispatchItemAsync(const ek::common::IntrusivePtr<DispatchItem>& pTask);

    void fire();

    void setRunLoop(RunLoop* pRunLoop);

private:
    std::mutex m_mutex;
    std::queue<ek::common::IntrusivePtr<DispatchItem>, std::list<ek::common::IntrusivePtr<DispatchItem>>> m_queue;
    RunLoop* m_pRunLoop;

};

}
}

#include <eventkit/dispatch/detail/DispatchQueue-inl.h>

#endif //EVENTKIT_DISPATCHQUEUE_H
