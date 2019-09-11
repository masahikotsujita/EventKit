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
    
    template <typename U>
    explicit intrusive_ptr(U* p, bool add_ref = true)
        : m_p(p) {
        if (p != nullptr && add_ref) {
            intrusive_ptr_ref(p);
        }
    }
    
    intrusive_ptr(std::nullptr_t)
        : m_p(nullptr) {
    }
    
    intrusive_ptr(const intrusive_ptr& rhs)
        : m_p(rhs.m_p) {
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
    }
    
    template<class U>
    intrusive_ptr(const intrusive_ptr<U>& rhs)
        : m_p(rhs.m_p) {
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
    }
    
    intrusive_ptr(intrusive_ptr&& rhs)
        : m_p(rhs.m_p) {
        rhs.m_p = nullptr;
    }
    
    template<class U>
    friend class intrusive_ptr;
    
    template<class U>
    intrusive_ptr(intrusive_ptr<U>&& rhs)
        : m_p(rhs.m_p) {
        rhs.m_p = nullptr;
    }
    
    intrusive_ptr& operator = (const intrusive_ptr& rhs) {
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
    intrusive_ptr& operator = (const intrusive_ptr<U>& rhs) {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = rhs.m_p;
        if (m_p != nullptr) {
            intrusive_ptr_ref(m_p);
        }
        return *this;
    }
    
    intrusive_ptr& operator = (intrusive_ptr&& another) noexcept {
        if (m_p != nullptr) {
            intrusive_ptr_unref(m_p);
        }
        m_p = another.m_p;
        another.m_p = nullptr;
        return *this;
    }
    
    template <typename U>
    intrusive_ptr& operator = (intrusive_ptr<U>&& another) noexcept {
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
    T* m_p;

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

template <typename T, typename ...Args>
intrusive_ptr<T> make_intrusive(Args&& ...args) {
    return intrusive_ptr<T>(new T (std::forward<Args>(args)...));
}

}
}

#endif //EVENTKIT_INTRUSIVE_PTR_H
