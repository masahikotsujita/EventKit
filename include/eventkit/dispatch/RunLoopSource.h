//
// Created by Tsujita Masahiko on 2020/10/04.
//

#ifndef EVENTKIT_RUNLOOPSOURCE_H
#define EVENTKIT_RUNLOOPSOURCE_H

namespace ek {
namespace common {
class Condition;
}
}

namespace ek {
namespace dispatch {

class RunLoopSource {
public:

    virtual ~RunLoopSource() = default;

    virtual void fire() = 0;

    ek::common::Condition* getCondition() const;

    void setCondition(ek::common::Condition* pCondition);

    virtual void ref() = 0;

    virtual void unref() = 0;

private:
    ek::common::Condition* m_pCondition = nullptr;

};

inline void intrusive_ptr_ref(RunLoopSource* pObj) {
    pObj->ref();
}

inline void intrusive_ptr_unref(RunLoopSource* pObj) {
    pObj->unref();
}

inline ek::common::Condition* RunLoopSource::getCondition() const {
    return m_pCondition;
}

inline void RunLoopSource::setCondition(ek::common::Condition* pCondition) {
    m_pCondition = pCondition;
}

}
}

#endif //EVENTKIT_RUNLOOPSOURCE_H
