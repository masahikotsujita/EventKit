//
// Created by Masahiko Tsujita on 2019-08-07.
//

#ifndef EVENTKIT_CONDITION_H
#define EVENTKIT_CONDITION_H

#include <cassert>
#include <mutex>
#include <condition_variable>

namespace ek {
namespace common {

template <typename T>
class Condition final {
private:
    using ScopedLock = std::lock_guard<std::mutex>;

public:
    using Value = T;
    using value_type = Value;
    using LockHandle = std::unique_lock<std::mutex>;

    template <typename ...Args>
    explicit Condition(Args&& ...args)
        : m_mutex()
        , m_conditionVariable()
        , m_variable(std::forward<Args>(args)...) {
    }

    Condition(const Condition&) = delete;

    Condition& operator = (const Condition&) = delete;

    ~Condition() = default;

    template <typename Predicate>
    void wait(const Predicate& predicate, LockHandle& lock) {
        m_conditionVariable.wait(lock, [&]{ return predicate(m_variable); });
    }

    Value getValue(LockHandle* pLockHandle = nullptr) {
        LockHandle lock(m_mutex);
        if (pLockHandle != nullptr) {
            *pLockHandle = std::move(lock);
        }
        return m_variable;
    }

    template <typename Body>
    void mutateValue(const Body& body) {
        ScopedLock lock(m_mutex);
        bool shouldNotify = false;
        body(m_variable, &shouldNotify);
        if (shouldNotify) {
            m_conditionVariable.notify_all();
        }
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_conditionVariable;
    T m_variable;

};

}
}

#endif //EVENTKIT_CONDITION_H
