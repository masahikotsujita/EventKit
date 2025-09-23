//
// Created by Tsujita Masahiko on 2020/09/12.
//

#include <eventkit/common/SystemAllocator.h>

namespace ek {
namespace common {

void* SystemAllocator::allocate(std::size_t size) {
    return ::operator new(size);
}

void SystemAllocator::deallocate(void* p) {
    ::operator delete(p);
}

}
}
