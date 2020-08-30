//
// Created by Masahiko Tsujita on 2019-08-05.
//

#include <iostream>
#include <thread>
#include <eventkit/common/SystemAllocator.h>
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/dispatch/DispatchQueue.h>

int main(int argc, const char* argv[]) {
    ek::dispatch::RunLoop mainLoop;
    auto pMainQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::GetDefaultAllocator(), ek::common::GetDefaultAllocator());
    mainLoop.addDispatchQueue(pMainQueue);

    ek::dispatch::RunLoop bgLoop;
    auto pBgQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::GetDefaultAllocator(), ek::common::GetDefaultAllocator());
    bgLoop.addDispatchQueue(pBgQueue);

    pMainQueue->dispatchAsync(ek::common::GetDefaultAllocator(), [] {
        printf("a task running on main thread.\n");
    });

    pBgQueue->dispatchAsync(ek::common::GetDefaultAllocator(), [] {
        printf("a task running on background thread.\n");
    });

    pBgQueue->dispatchAsync(ek::common::GetDefaultAllocator(), [&] {
        printf("dispatching a task to main thread from background thread.\n");
        pMainQueue->dispatchAsync(ek::common::GetDefaultAllocator(), [] {
            printf("a task running on main thread dispatched from background thread.\n");
        });
    });

    std::thread bgThread([&]{
        bgLoop.run();
    });

    mainLoop.run();

    bgThread.join();

    return 0;
}
