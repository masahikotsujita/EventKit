//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_PROMISECORE_H
#define EVENTKIT_PROMISECORE_H

#include <memory>
#include <mutex>
#include <list>
#include <eventkit/promise/Result.h>
#include <eventkit/promise/ResultObserver.h>

namespace ek {
namespace promise {
namespace detail {

template <typename T, typename E>
class PromiseCore : public ResultObserver<T, E> {
public:
    using Handler = ResultObserver<T, E>;

    PromiseCore() : m_isResolved(false) {}

    virtual void onResult(const Result<T, E>& result) override {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_isResolved) {
            return;
        }
        m_isResolved = true;
        m_result = result;
        std::list<std::shared_ptr<Handler>> handlers = std::move(m_handlers);
        lock.unlock();
        for (const auto& pHandler : handlers) {
            pHandler->onResult(m_result);
        }
    }

    void addHandler(const std::shared_ptr<Handler>& handler) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_isResolved) {
            m_handlers.push_back(handler);
        } else {
            handler->onResult(m_result);
        }
    }

private:
    std::mutex m_mutex;
    bool m_isResolved;
    Result<T, E> m_result;
    std::list<std::shared_ptr<Handler>> m_handlers;
};

}
}
}

#endif //EVENTKIT_PROMISECORE_H
