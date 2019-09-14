//
// Created by Masahiko Tsujita on 2019/09/12.
//

#ifndef EVENTKIT_REFCOUNTOBJECT_H
#define EVENTKIT_REFCOUNTOBJECT_H

#include <atomic>

namespace ek {
namespace common {

class RefCountObject {
public:
    RefCountObject()
        : m_refCount(1) {
    }
    
    virtual ~RefCountObject() = default;
    
    void ref() const {
        std::atomic_fetch_add_explicit(&m_refCount, 1u, std::memory_order_relaxed);
    }
    
    void unref() const {
        if (std::atomic_fetch_sub_explicit(&m_refCount, 1u, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete this;
        }
    }
    
private:
    mutable std::atomic_uint m_refCount;
    
};

inline void intrusive_ptr_ref(RefCountObject* pObj) {
    pObj->ref();
}

inline void intrusive_ptr_unref(RefCountObject* pObj) {
    pObj->unref();
}

}
}

#endif //EVENTKIT_REFCOUNTOBJECT_H
