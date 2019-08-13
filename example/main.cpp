//
// Created by Masahiko Tsujita on 2019-08-05.
//

#include <iostream>
#include <thread>
#include <eventkit/RunLoop.h>
#include <eventkit/DispatchQueue.h>

int main(int argc, const char* argv[]) {
    ek::RunLoop mainLoop;
    auto pMainQueue = std::make_shared<ek::DispatchQueue>();
    mainLoop.addDispatchQueue(pMainQueue);

    ek::RunLoop bgLoop;
    auto pBgQueue = std::make_shared<ek::DispatchQueue>();
    bgLoop.addDispatchQueue(pBgQueue);

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

    std::thread bgThread([&]{
        bgLoop.run();
    });

    mainLoop.run();

    bgThread.join();

    return 0;
}
