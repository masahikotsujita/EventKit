//
// Created by Tsujita Masahiko on 2021/03/08.
//

#include <eventkit/common/AllocatorStack.h>

#define EK_ALLOCATOR_STACK_SIZE (64)

namespace ek {
namespace common {

thread_local Allocator* tl_allocatorStack[EK_ALLOCATOR_STACK_SIZE];
thread_local Allocator** tl_ppAllocatorStackHead = tl_allocatorStack;
thread_local Allocator* tl_pCurrentAllocator = nullptr;

void pushAllocator(Allocator* pAllocator) {
    *tl_ppAllocatorStackHead = pAllocator;
    tl_ppAllocatorStackHead += 1;
    tl_pCurrentAllocator = pAllocator;
}

void popAllocator() {
    tl_ppAllocatorStackHead -= 1;
    tl_pCurrentAllocator = *tl_ppAllocatorStackHead;
}

Allocator* getCurrentAllocator() {
    return tl_pCurrentAllocator;
}

}
}
