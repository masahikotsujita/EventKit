//
// Created by Tsujita Masahiko on 2020/09/04.
//

#ifndef EVENTKIT_INTRUSIVEOBJECTMIXIN_H
#define EVENTKIT_INTRUSIVEOBJECTMIXIN_H

#include <atomic>

namespace ek {
namespace common {

class Allocator;

class IntrusiveObjectMixin final {
public:

    using Deleter = void (*)(IntrusiveObjectMixin* pMixin, void* pContext);

    explicit IntrusiveObjectMixin(Deleter pDeleter, void* pContext);

    IntrusiveObjectMixin(const IntrusiveObjectMixin&) = delete;

    IntrusiveObjectMixin& operator = (const IntrusiveObjectMixin&) = delete;

    ~IntrusiveObjectMixin();

    void ref();

    void unref();

private:
    Deleter m_pDeleter;
    void* m_pContext;
    mutable std::atomic_uint m_refCount;

};

}
}

#endif //EVENTKIT_INTRUSIVEOBJECTMIXIN_H
