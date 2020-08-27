//
// Created by Masahiko Tsujita on 2019/09/12.
//

#ifndef EVENTKIT_INTRUSIVEOBJECT_H
#define EVENTKIT_INTRUSIVEOBJECT_H

#include <atomic>
#include <eventkit/common/Allocator.h>

namespace ek {
namespace common {

class IntrusiveObject {
public:
    explicit IntrusiveObject(Allocator* pA)
        : m_pA(pA)
        , m_refCount(1) {
    }
    
    virtual ~IntrusiveObject() = default;
    
    void ref() {
        std::atomic_fetch_add_explicit(&m_refCount, 1u, std::memory_order_relaxed);
    }
    
    void unref() {
        if (std::atomic_fetch_sub_explicit(&m_refCount, 1u, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            m_pA->destroy(this);
        }
    }

private:
    mutable Allocator* m_pA;
    mutable std::atomic_uint m_refCount;
    
};

inline void intrusive_ptr_ref(IntrusiveObject* pObj) {
    pObj->ref();
}

inline void intrusive_ptr_unref(IntrusiveObject* pObj) {
    pObj->unref();
}

}
}

#endif //EVENTKIT_INTRUSIVEOBJECT_H
