//
// Created by Masahiko Tsujita on 2019/09/10.
//

#ifndef EVENTKIT_INTRUSIVEPTR_H
#define EVENTKIT_INTRUSIVEPTR_H

#include <utility>
#include <eventkit/common/Allocator.h>

namespace ek {
namespace common {

template <typename T>
class IntrusivePtr {
public:
    
    using element_type = T;
    
    IntrusivePtr()
        : m_p(nullptr) {
    }
    
    template <typename U>
    explicit IntrusivePtr(U* p, bool add_ref = true)
        : m_p(p) {
        if (p != nullptr && add_ref) {
            intrusive_ptr_ref(p);
        }
    }
    
    IntrusivePtr(std::nullptr_t)
        : m_p(nullptr) {
    }
    
    IntrusivePtr(const IntrusivePtr& rhs)
        : m_p(rhs.m_p) {
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
    }
    
    template<class U>
    IntrusivePtr(const IntrusivePtr<U>& rhs)
        : m_p(rhs.m_p) {
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
    }
    
    IntrusivePtr(IntrusivePtr&& rhs)
        : m_p(rhs.m_p) {
        rhs.m_p = nullptr;
    }
    
    template<class U>
    friend class IntrusivePtr;
    
    template<class U>
    IntrusivePtr(IntrusivePtr<U>&& rhs)
        : m_p(rhs.m_p) {
        rhs.m_p = nullptr;
    }
    
    IntrusivePtr& operator = (const IntrusivePtr& rhs) {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = rhs.m_p;
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
        return *this;
    }
    
    template <typename U>
    IntrusivePtr& operator = (const IntrusivePtr<U>& rhs) {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = rhs.m_p;
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
        return *this;
    }
    
    IntrusivePtr& operator = (IntrusivePtr&& another) noexcept {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = another.m_p;
        another.m_p = nullptr;
        return *this;
    }
    
    template <typename U>
    IntrusivePtr& operator = (IntrusivePtr<U>&& another) noexcept {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = another.m_p;
        another.m_p = nullptr;
        return *this;
    }
    
    ~IntrusivePtr() {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
    }
    
    T* get() const {
        return m_p;
    }
    
    T& operator * () const {
        return *m_p;
    }
    
    T* operator -> () const {
        return m_p;
    }

private:
    T* m_p;

};

template<class T, class U>
inline bool operator == (const IntrusivePtr<T>& a, const IntrusivePtr<U>& b) {
    return a.get() == b.get();
}

template<class T, class U>
inline bool operator != (const IntrusivePtr<T>& a, const IntrusivePtr<U>& b) {
    return a.get() != b.get();
}

template<class T, class U>
inline bool operator == (const IntrusivePtr<T>& a, U* b) {
    return a.get() == b;
}

template<class T, class U>
inline bool operator != (const IntrusivePtr<T>& a, U * b) {
    return a.get() != b;
}

template<class T, class U>
inline bool operator == (T* a, const IntrusivePtr<U>& b) {
    return a == b.get();
}

template<class T, class U>
inline bool operator != (T * a, const IntrusivePtr<U>& b) {
    return a != b.get();
}

template<class T>
inline bool operator == (const IntrusivePtr<T>& p, std::nullptr_t) {
    return p.get() == nullptr;
}

template<class T>
inline bool operator == (std::nullptr_t, const IntrusivePtr<T>& p) {
    return p.get() == nullptr;
}

template<class T>
inline bool operator != (const IntrusivePtr<T>& p, std::nullptr_t) {
    return p.get() != nullptr;
}

template<class T>
inline bool operator != (std::nullptr_t, const IntrusivePtr<T>& p) {
    return p.get() != nullptr;
}

template<class T>
T * get_pointer(const IntrusivePtr<T>& p) {
    return p.get();
}

template<class T, class U>
IntrusivePtr<T> static_pointer_cast(const IntrusivePtr<U>& p) {
    return static_cast<T*>(p.get());
}

template<class T, class U>
IntrusivePtr<T> const_pointer_cast(const IntrusivePtr<U>& p) {
    return const_cast<T*>(p.get());
}

#if defined(EVENTKIT_ENABLE_RTTI)
template<class T, class U>
IntrusivePtr<T> dynamic_pointer_cast(const IntrusivePtr<U>& p) {
    return dynamic_cast<T*>(p.get());
}
#endif

template <typename T, typename ...Args>
IntrusivePtr<T> make_intrusive(Allocator* pA, Args&& ...args) {
    return IntrusivePtr<T>(pA->create<T>(std::forward<Args>(args)...), false);
}

}
}

#endif //EVENTKIT_INTRUSIVEPTR_H
