//
// Created by Tsujita Masahiko on 2020/10/04.
//

#ifndef EVENTKIT_EVENTSOURCE_H
#define EVENTKIT_EVENTSOURCE_H

namespace ek {
namespace common {
template <typename T>
class Condition;
}
}

namespace ek {
namespace dispatch {

class EventSource {
public:

    virtual ~EventSource() = default;

    virtual void fire() = 0;

    ek::common::Condition<uint64_t>* getCondition() const;

    void setCondition(ek::common::Condition<uint64_t>* pCondition);

    virtual void ref() = 0;

    virtual void unref() = 0;

private:
    ek::common::Condition<uint64_t>* m_pCondition = nullptr;

};

inline void intrusive_ptr_ref(EventSource* pObj) {
    pObj->ref();
}

inline void intrusive_ptr_unref(EventSource* pObj) {
    pObj->unref();
}

inline ek::common::Condition<uint64_t>* EventSource::getCondition() const {
    return m_pCondition;
}

inline void EventSource::setCondition(ek::common::Condition<uint64_t>* pCondition) {
    m_pCondition = pCondition;
}

}
}

#endif //EVENTKIT_EVENTSOURCE_H
