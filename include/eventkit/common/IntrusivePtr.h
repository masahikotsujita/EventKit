//
// Created by Masahiko Tsujita on 2019/09/10.
//

#ifndef EVENTKIT_INTRUSIVEPTR_H
#define EVENTKIT_INTRUSIVEPTR_H

namespace ek {
namespace common {

class Allocator;

template <typename T>
class IntrusivePtr {
public:
    
    using element_type = T;
    
    IntrusivePtr();
    
    template <typename U>
    explicit IntrusivePtr(U* p, bool add_ref = true);

    IntrusivePtr(std::nullptr_t);
    
    IntrusivePtr(const IntrusivePtr& rhs);
    
    template<class U>
    IntrusivePtr(const IntrusivePtr<U>& rhs);
    
    IntrusivePtr(IntrusivePtr&& rhs);
    
    template<class U>
    friend class IntrusivePtr;
    
    template<class U>
    IntrusivePtr(IntrusivePtr<U>&& rhs);
    
    IntrusivePtr& operator = (const IntrusivePtr& rhs);
    
    template <typename U>
    IntrusivePtr& operator = (const IntrusivePtr<U>& rhs);
    
    IntrusivePtr& operator = (IntrusivePtr&& another) noexcept;
    
    template <typename U>
    IntrusivePtr& operator = (IntrusivePtr<U>&& another) noexcept;
    
    ~IntrusivePtr();
    
    T* get() const;
    
    T& operator * () const;
    
    T* operator -> () const;

private:
    T* m_p;

};

template<class T, class U>
inline bool operator == (const IntrusivePtr<T>& a, const IntrusivePtr<U>& b);

template<class T, class U>
inline bool operator != (const IntrusivePtr<T>& a, const IntrusivePtr<U>& b);

template<class T, class U>
inline bool operator == (const IntrusivePtr<T>& a, U* b);

template<class T, class U>
inline bool operator != (const IntrusivePtr<T>& a, U * b);

template<class T, class U>
inline bool operator == (T* a, const IntrusivePtr<U>& b);

template<class T, class U>
inline bool operator != (T * a, const IntrusivePtr<U>& b);

template<class T>
inline bool operator == (const IntrusivePtr<T>& p, std::nullptr_t);

template<class T>
inline bool operator == (std::nullptr_t, const IntrusivePtr<T>& p);

template<class T>
inline bool operator != (const IntrusivePtr<T>& p, std::nullptr_t);

template<class T>
inline bool operator != (std::nullptr_t, const IntrusivePtr<T>& p);

template<class T>
T * get_pointer(const IntrusivePtr<T>& p);

template<class T, class U>
IntrusivePtr<T> static_pointer_cast(const IntrusivePtr<U>& p);

template<class T, class U>
IntrusivePtr<T> const_pointer_cast(const IntrusivePtr<U>& p);

#if defined(EVENTKIT_ENABLE_RTTI)
template<class T, class U>
IntrusivePtr<T> dynamic_pointer_cast(const IntrusivePtr<U>& p);
#endif

template <typename T, typename ...Args>
IntrusivePtr<T> make_intrusive(Allocator* pA, Args&& ...args);

}
}

#include <eventkit/common/detail/IntrusivePtr-inl.h>

#endif //EVENTKIT_INTRUSIVEPTR_H
