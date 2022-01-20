//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISECORE_H
#define EVENTKIT_PROMISECORE_H

#include <memory>
#include <mutex>
#include <list>
#include <eventkit/common/AllocatorScope.h>
#include <eventkit/common/IntrusiveObjectMixin.h>
#include <eventkit/dispatch/DispatchQueue.h>
#include <eventkit/promise/Result.h>
#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class PromiseCore {
public:
    using Handler = ResultHandler<T, E>;

    explicit PromiseCore(ek::common::Allocator* pAllocator)
        : m_isResolved(false)
        , m_pAllocator(pAllocator) {
    }

    virtual ~PromiseCore() = default;

    virtual void resolve(const Result<T, E>& result) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_isResolved) {
            return;
        }
        m_isResolved = true;
        m_result = result;
        std::list<std::tuple<ek::dispatch::DispatchQueue*, ek::common::IntrusivePtr<Handler>>> handlers = std::move(m_handlers);
        lock.unlock();
        EK_USING_ALLOCATOR(m_pAllocator);
        for (const auto& handler : handlers) {
            ek::dispatch::DispatchQueue* pQueue = std::get<0>(handler);
            ek::common::IntrusivePtr<Handler> pHandler = std::get<1>(handler);
            pQueue->dispatchAsync(m_pAllocator, [pHandler, result=result]{
                pHandler->onResult(result);
            });
        }
    }

    void addHandler(ek::dispatch::DispatchQueue* pDispatchQueue, const ek::common::IntrusivePtr<Handler>& handler) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_isResolved) {
            m_handlers.push_back(std::make_tuple(pDispatchQueue, handler));
        } else {
            lock.unlock();
            pDispatchQueue->template dispatchAsync(m_pAllocator, [pAllocator=m_pAllocator, handler, result=m_result]{
                EK_USING_ALLOCATOR(pAllocator);
                handler->onResult(result);
            });
        }
    }
    
    virtual void ref() = 0;
    
    virtual void unref() = 0;

private:
    std::mutex m_mutex;
    bool m_isResolved;
    Result<T, E> m_result;
    std::list<std::tuple<ek::dispatch::DispatchQueue*, ek::common::IntrusivePtr<Handler>>> m_handlers;
    ek::common::Allocator* m_pAllocator;

};

template <typename T, typename E>
void intrusive_ptr_ref(PromiseCore<T, E>* pObj) {
    pObj->ref();
}
template <typename T, typename E>
void intrusive_ptr_unref(PromiseCore<T, E>* pObj) {
    pObj->unref();
}

}
}
}

#endif //EVENTKIT_PROMISECORE_H
