//
// Created by Tsujita Masahiko on 2020/08/24.
//

#ifndef EVENTKIT_SYSTEMALLOCATOR_H
#define EVENTKIT_SYSTEMALLOCATOR_H

#include <eventkit/common/Allocator.h>

namespace ek {
namespace common {

class SystemAllocator : public Allocator {
public:

    SystemAllocator() = default;

    virtual ~SystemAllocator() = default;

    virtual void* allocate(std::size_t size) override {
        return ::operator new(size);
    }

    virtual void deallocate(void* p) override {
        ::operator delete(p);
    }

};

}
}

#endif //EVENTKIT_SYSTEMALLOCATOR_H
