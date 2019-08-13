//
// Created by Masahiko Tsujita on 2019-08-07.
//

#ifndef EVENTKIT_SEMAPHORE_H
#define EVENTKIT_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

namespace ek {
namespace detail {

class Semaphore final {
public:

    explicit Semaphore(int maxCount = COUNT_MAX_DEFAULT);

    ~Semaphore() = default;

    void wait();

    template <typename Clock, typename Duration>
    bool waitUntil(const std::chrono::time_point<Clock, Duration>& timeoutTime);

    void notify();

public:
    Semaphore(const Semaphore&) = delete;
    Semaphore& operator = (const Semaphore&) = delete;

private:
    static constexpr int COUNT_MAX_DEFAULT = 1;
    const int m_maxCount;
    int m_count;
    std::mutex m_mutex;
    std::condition_variable m_conditionVariable;

};

inline Semaphore::Semaphore(int maxCount)
    : m_maxCount(maxCount)
    , m_count(0)
    , m_mutex()
    , m_conditionVariable() {
}

inline void Semaphore::wait() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_conditionVariable.wait(lock, [&]{ return m_count > 0; });
    m_count -= 1;
}

template <typename Clock, typename Duration>
bool Semaphore::waitUntil(const std::chrono::time_point<Clock, Duration>& timeoutTime) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (!m_conditionVariable.wait_until(lock, timeoutTime, [&]{ return m_count > 0; })) {
        return false;
    }
    m_count -= 1;
    return true;
}

inline void Semaphore::notify() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_count < m_maxCount) {
        m_count += 1;
        lock.unlock();
        m_conditionVariable.notify_one();
    }
}

}
}

#endif //EVENTKIT_SEMAPHORE_H
