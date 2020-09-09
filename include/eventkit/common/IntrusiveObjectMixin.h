//
// Created by Tsujita Masahiko on 2020/09/04.
//

#ifndef EVENTKIT_INTRUSIVEOBJECTMIXIN_H
#define EVENTKIT_INTRUSIVEOBJECTMIXIN_H

#include <cassert>
#include <atomic>
#include <eventkit/common/Allocator.h>

namespace ek {
namespace common {

class IntrusiveObjectMixin final {
public:

    using Deleter = void (*)(IntrusiveObjectMixin* pMixin, void* pContext);

    explicit IntrusiveObjectMixin(Deleter pDeleter, void* pContext)
        : m_pDeleter(pDeleter)
        , m_pContext(pContext)
        , m_refCount(1) {
    }

    IntrusiveObjectMixin(const IntrusiveObjectMixin&) = delete;
    IntrusiveObjectMixin& operator = (const IntrusiveObjectMixin&) = delete;

    ~IntrusiveObjectMixin() {
        assert(m_refCount == 0);
    }

    void ref() {
        std::atomic_fetch_add_explicit(&m_refCount, 1u, std::memory_order_relaxed);
    }

    void unref() {
        if (std::atomic_fetch_sub_explicit(&m_refCount, 1u, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            m_pDeleter(this, m_pContext);
        }
    }

private:
    Deleter m_pDeleter;
    void* m_pContext;
    mutable std::atomic_uint m_refCount;

};

}
}

#endif //EVENTKIT_INTRUSIVEOBJECTMIXIN_H
