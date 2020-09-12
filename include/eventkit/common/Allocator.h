//
// Created by Tsujita Masahiko on 2020/08/23.
//

#ifndef EVENTKIT_ALLOCATOR_H
#define EVENTKIT_ALLOCATOR_H

#include <cstddef>

namespace ek {
namespace common {

class Allocator {
public:
    virtual ~Allocator() = default;

    virtual void* allocate(std::size_t size) = 0;

    virtual void deallocate(void* p) = 0;

    template <typename T, typename ...Args>
    void construct(T* p, Args&& ...args);

    template <typename T>
    void destruct(T* p);

    template <typename T, typename ...Args>
    T* create(Args&& ...args);

    template <typename T>
    void destroy(T* p);

};

Allocator* getDefaultAllocator();

}
}

#include <eventkit/common/detail/Allocator-inl.h>

#endif //EVENTKIT_ALLOCATOR_H
