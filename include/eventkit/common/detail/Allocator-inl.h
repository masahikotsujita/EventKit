//
// Created by Tsujita Masahiko on 2020/09/12.
//

#ifndef EVENTKIT_ALLOCATOR_INL_H
#define EVENTKIT_ALLOCATOR_INL_H

#include <eventkit/common/Allocator.h>
#include <memory>
#include <utility>

namespace ek {
namespace common {

template <typename T, typename... Args>
void Allocator::construct(T* p, Args&& ... args) {
    new(p) T(std::forward<Args>(args)...);
}

template <typename T>
void Allocator::destruct(T* p) {
    p->~T();
}

template <typename T, typename... Args>
T* Allocator::create(Args&& ... args) {
    T* p = static_cast<T*>(allocate(sizeof(T)));
    construct(p, std::forward<Args>(args)...);
    return p;
}

template <typename T>
void Allocator::destroy(T* p) {
    destruct(p);
    deallocate(static_cast<void*>(p));
}

}
}

#endif //EVENTKIT_ALLOCATOR_INL_H
