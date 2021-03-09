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
        std::list<ek::common::IntrusivePtr<Handler>> handlers = std::move(m_handlers);
        lock.unlock();
        EK_USING_ALLOCATOR(m_pAllocator);
        for (const auto& pHandler : handlers) {
            pHandler->onResult(m_result);
        }
    }

    void addHandler(const ek::common::IntrusivePtr<Handler>& handler) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_isResolved) {
            m_handlers.push_back(handler);
        } else {
            lock.unlock();
            EK_USING_ALLOCATOR(m_pAllocator);
            handler->onResult(m_result);
        }
    }
    
    virtual void ref() = 0;
    
    virtual void unref() = 0;

private:
    std::mutex m_mutex;
    bool m_isResolved;
    Result<T, E> m_result;
    std::list<ek::common::IntrusivePtr<Handler>> m_handlers;
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
