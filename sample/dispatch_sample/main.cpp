//
// Created by Masahiko Tsujita on 2019-08-05.
//

#include <iostream>
#include <thread>
#include <eventkit/common/SystemAllocator.h>
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/dispatch/DispatchQueue.h>

int main(int argc, const char* argv[]) {
    ek::dispatch::EventLoop mainLoop;
    auto pMainQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::getDefaultAllocator(),
                                                                              ek::common::getDefaultAllocator());
    mainLoop.addSource(pMainQueue);

    ek::dispatch::EventLoop bgLoop;
    auto pBgQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::getDefaultAllocator(),
                                                                            ek::common::getDefaultAllocator());
    bgLoop.addSource(pBgQueue);

    pMainQueue->dispatchAsync(ek::common::getDefaultAllocator(), [] {
        printf("a task running on main thread.\n");
    });

    pBgQueue->dispatchAsync(ek::common::getDefaultAllocator(), [] {
        printf("a task running on background thread.\n");
    });

    pBgQueue->dispatchAsync(ek::common::getDefaultAllocator(), [&] {
        printf("dispatching a task to main thread from background thread.\n");
        pMainQueue->dispatchAsync(ek::common::getDefaultAllocator(), [] {
            printf("a task running on main thread dispatched from background thread.\n");
        });
    });

    ek::dispatch::setCurrentDispatchQueue(pMainQueue.get());

    std::thread bgThread([&]{
        ek::dispatch::setCurrentDispatchQueue(pBgQueue.get());
        bgLoop.run();
    });

    mainLoop.run();

    bgThread.join();

    return 0;
}
