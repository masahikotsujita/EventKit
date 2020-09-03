//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISECORE_H
#define EVENTKIT_PROMISECORE_H

#include <memory>
#include <mutex>
#include <list>
#include <eventkit/promise/Result.h>
#include <eventkit/promise/ResultHandler.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class PromiseCore : public ek::common::IntrusiveObject, public ResultHandler<T, E> {
public:
    using Handler = ResultHandler<T, E>;

    explicit PromiseCore(ek::common::Allocator* pA)
        : ek::common::IntrusiveObject(pA)
        , m_isResolved(false) {
    }

    virtual void onResult(const Result<T, E>& result) override {
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
    
    virtual void ref() override {
        ek::common::IntrusiveObject::ref();
    }
    
    virtual void unref() override {
        ek::common::IntrusiveObject::unref();
    }

private:
    std::mutex m_mutex;
    bool m_isResolved;
    Result<T, E> m_result;
    std::list<ek::common::IntrusivePtr<Handler>> m_handlers;
};

}
}
}

#endif //EVENTKIT_PROMISECORE_H
