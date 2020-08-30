//
// Created by Tsujita Masahiko on 2020/08/30.
//

#include <eventkit/common/Allocator.h>
#include <eventkit/common/SystemAllocator.h>

namespace ek {
namespace common {

SystemAllocator g_systemAllocator;

Allocator* getDefaultAllocator() {
    return &g_systemAllocator;
}

}
}
