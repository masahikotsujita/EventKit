//
// Created by Tsujita Masahiko on 2020/09/12.
//

#ifndef EVENTKIT_INTRUSIVEPTR_INL_H
#define EVENTKIT_INTRUSIVEPTR_INL_H

#include <eventkit/common/IntrusivePtr.h>
#include <eventkit/common/Allocator.h>
#include <utility>

namespace ek {
namespace common {

template <typename T>
IntrusivePtr<T>::IntrusivePtr()
    : m_p(nullptr) {
}

template <typename T>
template <typename U>
IntrusivePtr<T>::IntrusivePtr(U* p, bool add_ref)
    : m_p(p) {
    if (p != nullptr && add_ref) {
        intrusive_ptr_ref(p);
    }
}

template <typename T>
IntrusivePtr<T>::IntrusivePtr(std::nullptr_t)
    : m_p(nullptr) {
}

template <typename T>
IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr& rhs)
    : m_p(rhs.m_p) {
    if (m_p != nullptr) {
        intrusive_ptr_ref(m_p);
    }
}

template <typename T>
IntrusivePtr<T>::IntrusivePtr(IntrusivePtr&& rhs)
    : m_p(rhs.m_p) {
    rhs.m_p = nullptr;
}

template <typename T>
template <class U>
IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr<U>& rhs)
    : m_p(rhs.m_p) {
    if (m_p != nullptr) {
        intrusive_ptr_ref(m_p);
    }
}

template <typename T>
template <class U>
IntrusivePtr<T>::IntrusivePtr(IntrusivePtr<U>&& rhs)
    : m_p(rhs.m_p) {
    rhs.m_p = nullptr;
}

template <typename T>
template <typename U>
IntrusivePtr<T>& IntrusivePtr<T>::operator =(const IntrusivePtr<U>& rhs) {
    if (m_p != nullptr) {
        intrusive_ptr_unref(m_p);
    }
    m_p = rhs.m_p;
    if (m_p != nullptr) {
        intrusive_ptr_ref(m_p);
    }
    return *this;
}

template <typename T>
template <typename U>
IntrusivePtr<T>& IntrusivePtr<T>::operator =(IntrusivePtr<U>&& another) noexcept {
    if (m_p != nullptr) {
        intrusive_ptr_unref(m_p);
    }
    m_p = another.m_p;
    another.m_p = nullptr;
    return *this;
}

template <typename T>
IntrusivePtr<T>::~IntrusivePtr() {
    if (m_p != nullptr) {
        intrusive_ptr_unref(m_p);
    }
}

template <typename T>
T* IntrusivePtr<T>::get() const {
    return m_p;
}

template <typename T>
T& IntrusivePtr<T>::operator *() const {
    return *m_p;
}

template <typename T>
T* IntrusivePtr<T>::operator ->() const {
    return m_p;
}

template <typename T>
IntrusivePtr<T>& IntrusivePtr<T>::operator =(IntrusivePtr&& another) noexcept {
    if (m_p != nullptr) {
        intrusive_ptr_unref(m_p);
    }
    m_p = another.m_p;
    another.m_p = nullptr;
    return *this;
}

template <typename T>
IntrusivePtr<T>& IntrusivePtr<T>::operator =(const IntrusivePtr& rhs) {
    if (m_p != nullptr) {
        intrusive_ptr_unref(m_p);
    }
    m_p = rhs.m_p;
    if (m_p != nullptr) {
        intrusive_ptr_ref(m_p);
    }
    return *this;
}

template <class T, class U>
bool operator ==(const IntrusivePtr<T>& a, const IntrusivePtr<U>& b) {
    return a.get() == b.get();
}

template <class T, class U>
bool operator !=(const IntrusivePtr <T>& a, const IntrusivePtr <U>& b) {
    return a.get() != b.get();
}

template <class T, class U>
bool operator ==(const IntrusivePtr <T>& a, U* b) {
    return a.get() == b;
}

template <class T, class U>
bool operator !=(const IntrusivePtr <T>& a, U* b) {
    return a.get() != b;
}

template <class T, class U>
bool operator ==(T* a, const IntrusivePtr <U>& b) {
    return a == b.get();
}

template <class T, class U>
bool operator !=(T* a, const IntrusivePtr <U>& b) {
    return a != b.get();
}

template <class T>
bool operator ==(const IntrusivePtr <T>& p, std::nullptr_t) {
    return p.get() == nullptr;
}

template <class T>
bool operator ==(std::nullptr_t, const IntrusivePtr <T>& p) {
    return p.get() == nullptr;
}

template <class T>
bool operator !=(const IntrusivePtr <T>& p, std::nullptr_t) {
    return p.get() != nullptr;
}

template <class T>
bool operator !=(std::nullptr_t, const IntrusivePtr <T>& p) {
    return p.get() != nullptr;
}

template <class T>
T* get_pointer(const IntrusivePtr <T>& p) {
    return p.get();
}

template <class T, class U>
IntrusivePtr <T> static_pointer_cast(const IntrusivePtr <U>& p) {
    return static_cast<T*>(p.get());
}

template <class T, class U>
IntrusivePtr <T> const_pointer_cast(const IntrusivePtr <U>& p) {
    return const_cast<T*>(p.get());
}

#if defined(EVENTKIT_ENABLE_RTTI)
template<class T, class U>
IntrusivePtr<T> dynamic_pointer_cast(const IntrusivePtr<U>& p) {
    return dynamic_cast<T*>(p.get());
}
#endif

template <typename T, typename... Args>
IntrusivePtr <T> make_intrusive(Allocator* pA, Args&& ... args) {
    return IntrusivePtr<T>(pA->create<T>(std::forward<Args>(args)...), false);
}

}
}

#endif //EVENTKIT_INTRUSIVEPTR_INL_H
