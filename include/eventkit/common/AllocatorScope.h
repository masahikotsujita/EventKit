//
// Created by Tsujita Masahiko on 2021/03/10.
//

#ifndef EVENTKIT_ALLOCATORSCOPE_H
#define EVENTKIT_ALLOCATORSCOPE_H

#include <eventkit/common/AllocatorStack.h>
#include <type_traits>

namespace ek {
namespace common {

class AllocatorScope final {
public:

    explicit AllocatorScope(Allocator* pAllocator) {
        pushAllocator(pAllocator);
    }

    ~AllocatorScope() {
        popAllocator();
    }

    AllocatorScope(const AllocatorScope&) = delete;

    AllocatorScope& operator = (const AllocatorScope&) = delete;

};

template <typename Block>
std::result_of_t<Block()> withAllocator(Allocator* pAllocator, Block&& block) {
    AllocatorScope scopedAllocation(pAllocator);
    return block();
}

}
}

#define EK_USING_ALLOCATOR(pA) ek::common::AllocatorScope __ek_scoped_allocator_using ## __LINE__ { pA }

#endif //EVENTKIT_ALLOCATORSCOPE_H
