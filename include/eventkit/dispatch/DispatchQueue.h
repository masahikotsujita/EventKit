//
// Created by Masahiko Tsujita on 2019-08-12.
//

#ifndef EVENTKIT_DISPATCHQUEUE_H
#define EVENTKIT_DISPATCHQUEUE_H

#include <mutex>
#include <list>
#include <queue>
#include <eventkit/common/Allocator.h>
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/common/IntrusiveObject.h>

namespace ek {
namespace dispatch {

class DispatchItem : public ek::common::IntrusiveObject {
public:
    explicit DispatchItem(ek::common::Allocator* pA) : ek::common::IntrusiveObject(pA) {}

    virtual ~DispatchItem() = default;

    virtual void run() = 0;

};

template <typename Function>
class DispatchFunctionItem : public DispatchItem {
public:

    template <typename F>
    explicit DispatchFunctionItem(ek::common::Allocator* pA, F&& function);

    ~DispatchFunctionItem() override = default;

    void run() override;

private:
    Function m_function;

};

class DispatchQueue : public ek::common::IntrusiveObject {
public:

    friend class RunLoop;

    explicit DispatchQueue(ek::common::Allocator* pA)
        : ek::common::IntrusiveObject(pA)
        , m_pRunLoop(nullptr) {
    }

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
