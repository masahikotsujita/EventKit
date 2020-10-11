//
// Created by Masahiko Tsujita on 2019-08-12.
//

#ifndef EVENTKIT_DISPATCHQUEUE_H
#define EVENTKIT_DISPATCHQUEUE_H

#include <mutex>
#include <list>
#include <queue>
#include <eventkit/common/IntrusiveObjectMixin.h>
#include <eventkit/common/Allocator.h>
#include <eventkit/common/IntrusivePtr.h>
#include <eventkit/dispatch/RunLoopSource.h>

namespace ek {
namespace dispatch {

class DispatchItem;

class DispatchQueue : public ek::dispatch::RunLoopSource {
public:

    explicit DispatchQueue(ek::common::Allocator* pA);

    template <typename F>
    void dispatchAsync(ek::common::Allocator* pA, F&& function);

    virtual void fire() override;

    virtual void ref() override;

    virtual void unref() override;

private:

    void dispatchItemAsync(const ek::common::IntrusivePtr<DispatchItem>& pTask);

    static void deleteCallback(ek::common::IntrusiveObjectMixin*, void* pContext);

private:
    ek::common::IntrusiveObjectMixin m_intrusiveObjectMixin;
    ek::common::Allocator* m_pA;
    std::mutex m_mutex;
    std::queue<ek::common::IntrusivePtr<DispatchItem>, std::list<ek::common::IntrusivePtr<DispatchItem>>> m_queue;

};

}
}

#include <eventkit/dispatch/detail/DispatchQueue-inl.h>

#endif //EVENTKIT_DISPATCHQUEUE_H
