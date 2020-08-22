//
// Created by Masahiko Tsujita on 2019-08-12.
//

#ifndef EVENTKIT_DISPATCHQUEUE_H
#define EVENTKIT_DISPATCHQUEUE_H

#include <mutex>
#include <list>
#include <queue>
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/common/IntrusiveObject.h>

namespace ek {
namespace dispatch {

class DispatchItem : public ek::common::IntrusiveObject {
public:
    virtual ~DispatchItem() = default;

    virtual void run() = 0;

};

template <typename Function>
class DispatchFunctionItem : public DispatchItem {
public:

    template <typename F>
    explicit DispatchFunctionItem(F&& function);

    ~DispatchFunctionItem() override = default;

    void run() override;

private:
    Function m_function;

};

class DispatchQueue : public ek::common::IntrusiveObject {
public:

    friend class RunLoop;

    template <typename F>
    void dispatchAsync(F&& function);

private:

    void dispatchItemAsync(const ek::common::intrusive_ptr<DispatchItem>& pTask);

    void fire();

    void setRunLoop(RunLoop* pRunLoop);

private:
    std::mutex m_mutex;
    std::queue<ek::common::intrusive_ptr<DispatchItem>, std::list<ek::common::intrusive_ptr<DispatchItem>>> m_queue;
    RunLoop* m_pRunLoop;

};

}
}

#include <eventkit/dispatch/detail/DispatchQueue-inl.h>

#endif //EVENTKIT_DISPATCHQUEUE_H
