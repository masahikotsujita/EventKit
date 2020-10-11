//
// Created by Tsujita Masahiko on 2020/10/04.
//

#ifndef EVENTKIT_RUNLOOPSOURCE_H
#define EVENTKIT_RUNLOOPSOURCE_H

namespace ek {
namespace dispatch {

namespace detail {

class Semaphore;

}

class RunLoopSource {
public:

    virtual ~RunLoopSource() = default;

    virtual void fire() = 0;

    detail::Semaphore* getSemaphore() const;

    void setSemaphore(detail::Semaphore* pSemaphore);

    virtual void ref() = 0;

    virtual void unref() = 0;

private:
    detail::Semaphore* m_pSemaphore = nullptr;

};

inline void intrusive_ptr_ref(RunLoopSource* pObj) {
    pObj->ref();
}

inline void intrusive_ptr_unref(RunLoopSource* pObj) {
    pObj->unref();
}

inline detail::Semaphore* RunLoopSource::getSemaphore() const {
    return m_pSemaphore;
}

inline void RunLoopSource::setSemaphore(detail::Semaphore* pSemaphore) {
    m_pSemaphore = pSemaphore;
}

}
}

#endif //EVENTKIT_RUNLOOPSOURCE_H
