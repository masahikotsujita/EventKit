//
// Created by Masahiko Tsujita on 2019-08-07.
//

#ifndef EVENTKIT_CONDITION_H
#define EVENTKIT_CONDITION_H

#include <mutex>
#include <condition_variable>

namespace ek {
namespace common {

class Condition final {
public:

    Condition()
        : m_mutex()
        , m_conditionVariable()
        , m_isOpen() {
    }

    Condition(const Condition&) = delete;

    Condition& operator = (const Condition&) = delete;

    ~Condition() = default;

    template <typename Body>
    void wait(const Body& body) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_conditionVariable.wait(lock, [&]{ return m_isOpen; });
        bool isOpen = m_isOpen;
        body(isOpen);
        if (isOpen != m_isOpen) {
            m_isOpen = isOpen;
        }
    }

    template <typename Body>
    void modify(const Body& body) {
        std::lock_guard<std::mutex> lock(m_mutex);
        bool isOpen = m_isOpen;
        body(isOpen);
        if (isOpen != m_isOpen) {
            m_isOpen = isOpen;
            m_conditionVariable.notify_all();
        }
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_conditionVariable;
    bool m_isOpen;

};

}
}

#endif //EVENTKIT_CONDITION_H
