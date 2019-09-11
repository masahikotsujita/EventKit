//
// Created by Masahiko Tsujita on 2019/09/10.
//

#ifndef EVENTKIT_INTRUSIVE_PTR_H
#define EVENTKIT_INTRUSIVE_PTR_H

#include <utility>

namespace ek {
namespace common {

template <typename T>
class intrusive_ptr {
public:
    
    using element_type = T;
    
    intrusive_ptr()
        : m_p(nullptr) {
    }
    
    explicit intrusive_ptr(element_type* p, bool add_ref = true)
        : m_p(p) {
        if (p != nullptr && add_ref) {
            intrusive_ptr_ref(p);
        }
    }
    
    intrusive_ptr(const intrusive_ptr& another)
        : m_p(another.m_p) {
        intrusive_ptr_ref(m_p);
    }
    
    intrusive_ptr(intrusive_ptr&& another) noexcept
        : m_p(another.m_p) {
        another.m_p = nullptr;
    }
    
    intrusive_ptr& operator = (const intrusive_ptr& another) {
        if (this == &another) {
            return *this;
        }
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = another.m_p;
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
        return *this;
    }
    
    intrusive_ptr& operator = (intrusive_ptr&& another) noexcept {
        if (this == &another) {
            return *this;
        }
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = another.m_p;
        another.m_p = nullptr;
        return *this;
    }
    
    ~intrusive_ptr() {
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
    T *const m_p;

};

template<class T, class U>
inline bool operator == (const intrusive_ptr<T>& a, const intrusive_ptr<U>& b) {
    return a.get() == b.get();
}

template<class T, class U>
inline bool operator != (const intrusive_ptr<T>& a, const intrusive_ptr<U>& b) {
    return a.get() != b.get();
}

template<class T, class U>
inline bool operator == (const intrusive_ptr<T>& a, U* b) {
    return a.get() == b;
}

template<class T, class U>
inline bool operator != (const intrusive_ptr<T>& a, U * b) {
    return a.get() != b;
}

template<class T, class U>
inline bool operator == (T* a, const intrusive_ptr<U>& b) {
    return a == b.get();
}

template<class T, class U>
inline bool operator != (T * a, const intrusive_ptr<U>& b) {
    return a != b.get();
}

template<class T>
inline bool operator == (const intrusive_ptr<T>& p, std::nullptr_t) {
    return p.get() == nullptr;
}

template<class T>
inline bool operator == (std::nullptr_t, const intrusive_ptr<T>& p) {
    return p.get() == nullptr;
}

template<class T>
inline bool operator != (const intrusive_ptr<T>& p, std::nullptr_t) {
    return p.get() != nullptr;
}

template<class T>
inline bool operator != (std::nullptr_t, const intrusive_ptr<T>& p) {
    return p.get() != nullptr;
}

template<class T>
T * get_pointer(const intrusive_ptr<T>& p) {
    return p.get();
}

template<class T, class U>
intrusive_ptr<T> static_pointer_cast(const intrusive_ptr<U>& p) {
    return static_cast<T*>(p.get());
}

template<class T, class U>
intrusive_ptr<T> const_pointer_cast(const intrusive_ptr<U>& p) {
    return const_cast<T*>(p.get());
}

#if defined(EVENTKIT_ENABLE_RTTI)
template<class T, class U>
intrusive_ptr<T> dynamic_pointer_cast(const intrusive_ptr<U>& p) {
    return dynamic_cast<T*>(p.get());
}
#endif

}
}

#endif //EVENTKIT_INTRUSIVE_PTR_H
