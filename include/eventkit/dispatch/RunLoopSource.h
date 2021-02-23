//
// Created by Tsujita Masahiko on 2020/10/04.
//

#ifndef EVENTKIT_RUNLOOPSOURCE_H
#define EVENTKIT_RUNLOOPSOURCE_H

namespace ek {
namespace common {
template <typename T>
class Condition;
}
}

namespace ek {
namespace dispatch {

class RunLoopSource {
public:

    virtual ~RunLoopSource() = default;

    virtual void fire() = 0;

    ek::common::Condition<uint64_t>* getCondition() const;

    void setCondition(ek::common::Condition<uint64_t>* pCondition);

    virtual void ref() = 0;

    virtual void unref() = 0;

private:
    ek::common::Condition<uint64_t>* m_pCondition = nullptr;

};

inline void intrusive_ptr_ref(RunLoopSource* pObj) {
    pObj->ref();
}

inline void intrusive_ptr_unref(RunLoopSource* pObj) {
    pObj->unref();
}

inline ek::common::Condition<uint64_t>* RunLoopSource::getCondition() const {
    return m_pCondition;
}

inline void RunLoopSource::setCondition(ek::common::Condition<uint64_t>* pCondition) {
    m_pCondition = pCondition;
}

}
}

#endif //EVENTKIT_RUNLOOPSOURCE_H