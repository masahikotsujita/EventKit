//
// Created by Tsujita Masahiko on 2020/09/04.
//

#ifndef EVENTKIT_INTRUSIVEMIXIN_H
#define EVENTKIT_INTRUSIVEMIXIN_H

#include <atomic>
#include <eventkit/common/Allocator.h>

namespace ek {
namespace common {

class IntrusiveMixin final {
public:

    using Deleter = void (*)(IntrusiveMixin* pMixin, void* pContext);

    explicit IntrusiveMixin(Deleter pDeleter, void* pContext)
        : m_pDeleter(pDeleter)
        , m_pContext(pContext)
        , m_refCount(1) {
    }

    IntrusiveMixin(const IntrusiveMixin&) = delete;
    IntrusiveMixin& operator = (const IntrusiveMixin&) = delete;

    ~IntrusiveMixin() {
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

#endif //EVENTKIT_INTRUSIVEMIXIN_H
