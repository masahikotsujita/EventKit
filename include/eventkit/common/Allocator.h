//
// Created by Tsujita Masahiko on 2020/08/23.
//

#ifndef EVENTKIT_ALLOCATOR_H
#define EVENTKIT_ALLOCATOR_H

#include <cstddef>
#include <memory>
#include <utility>

namespace ek {
namespace common {

class Allocator {
public:
    virtual ~Allocator() = default;

    virtual void* allocate(std::size_t size) = 0;

    virtual void deallocate(void* p) = 0;

    template <typename T, typename ...Args>
    void construct(T* p, Args&& ...args) {
        new(p) T(std::forward<Args>(args)...);
    }

    template <typename T>
    void destruct(T* p) {
        p->~T();
    }

    template <typename T, typename ...Args>
    T* create(Args&& ...args) {
        T* p = static_cast<T*>(allocate(sizeof(T)));
        construct(p, std::forward<Args>(args)...);
        return p;
    }

    template <typename T>
    void destroy(T* p) {
        destruct(p);
        deallocate(static_cast<void*>(p));
    }

};

}
}

#endif //EVENTKIT_ALLOCATOR_H
