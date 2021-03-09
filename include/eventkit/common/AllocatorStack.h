//
// Created by Tsujita Masahiko on 2021/03/08.
//

#ifndef EVENTKIT_ALLOCATORSTACK_H
#define EVENTKIT_ALLOCATORSTACK_H

namespace ek {
namespace common {
class Allocator;
}
}

namespace ek {
namespace common {

void pushAllocator(Allocator* pAllocator);

void popAllocator();

Allocator* getCurrentAllocator();

}
}

#endif //EVENTKIT_ALLOCATORSTACK_H
