//
// Created by Tsujita Masahiko on 2020/09/12.
//

#include <eventkit/common/IntrusiveObjectMixin.h>
#include <cassert>

namespace ek {
namespace common {

IntrusiveObjectMixin::IntrusiveObjectMixin(IntrusiveObjectMixin::Deleter pDeleter, void* pContext)
    : m_pDeleter(pDeleter)
    , m_pContext(pContext)
    , m_refCount(1) {
}

IntrusiveObjectMixin::~IntrusiveObjectMixin() {
    assert(m_refCount == 0);
}

void IntrusiveObjectMixin::ref() {
    std::atomic_fetch_add_explicit(&m_refCount, 1u, std::memory_order_relaxed);
}

void IntrusiveObjectMixin::unref() {
    if (std::atomic_fetch_sub_explicit(&m_refCount, 1u, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        m_pDeleter(this, m_pContext);
    }
}

}
}
