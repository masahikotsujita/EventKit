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

    virtual void* allocate(std::size_t size) override;

    virtual void deallocate(void* p) override;

};

}
}

#endif //EVENTKIT_SYSTEMALLOCATOR_H
