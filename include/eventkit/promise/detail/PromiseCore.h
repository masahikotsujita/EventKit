//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISECORE_H
#define EVENTKIT_PROMISECORE_H

#include <memory>
#include <mutex>
#include <list>
#include <eventkit/common/IntrusiveMixin.h>
#include <eventkit/promise/Result.h>
#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class PromiseCore {
public:
    using Handler = ResultHandler<T, E>;

    explicit PromiseCore(ek::common::Allocator* pA)
        : m_isResolved(false)
        , m_pA(pA)
        , m_intrusiveMixin([](ek::common::IntrusiveMixin*, void* pContext){
            auto* pThis = static_cast<PromiseCore*>(pContext);
            pThis->m_pA->destroy(pThis);
        }, this) {
    }

    virtual void resolve(const Result<T, E>& result) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_isResolved) {
            return;
        }
        m_isResolved = true;
        m_result = result;
        std::list<ek::common::IntrusivePtr<Handler>> handlers = std::move(m_handlers);
        lock.unlock();
        for (const auto& pHandler : handlers) {
            pHandler->onResult(m_result);
        }
    }

    void addHandler(const ek::common::IntrusivePtr<Handler>& handler) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_isResolved) {
            m_handlers.push_back(handler);
        } else {
            handler->onResult(m_result);
        }
    }
    
    void ref() {
        m_intrusiveMixin.ref();
    }
    
    void unref() {
        m_intrusiveMixin.unref();
    }

private:
    std::mutex m_mutex;
    bool m_isResolved;
    Result<T, E> m_result;
    std::list<ek::common::IntrusivePtr<Handler>> m_handlers;
    ek::common::Allocator* m_pA;
    ek::common::IntrusiveMixin m_intrusiveMixin;

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
