//
// Created by Masahiko Tsujita on 2019-08-05.
//

#include <iostream>
#include <thread>
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/dispatch/DispatchQueue.h>

int main(int argc, const char* argv[]) {
    ek::dispatch::EventLoop mainLoop;
    auto pMainQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>();
    mainLoop.addSource(pMainQueue);

    ek::dispatch::EventLoop bgLoop;
    auto pBgQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>();
    bgLoop.addSource(pBgQueue);

    pMainQueue->dispatchAsync([] {
        printf("a task running on main thread.\n");
    });

    pBgQueue->dispatchAsync([] {
        printf("a task running on background thread.\n");
    });

    pBgQueue->dispatchAsync([&] {
        printf("dispatching a task to main thread from background thread.\n");
        pMainQueue->dispatchAsync([] {
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
