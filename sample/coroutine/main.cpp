//
// Created by Tsujita Masahiko on 2022/02/22.
//

#include <iostream>
#include <utility>
#include <sstream>
#include <thread>
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/promise/Promise.h>
#include "../sample_utils/logging.h"
#include "main.h"

ek::dispatch::DispatchQueue* g_pMainQueue = nullptr;
ek::dispatch::DispatchQueue* g_pBackgroundQueue = nullptr;

int main(int argc, const char* argv[]) {
    ek::common::pushAllocator(ek::common::getDefaultAllocator());

    ek::dispatch::EventLoop mainLoop;
    auto pMainQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::getDefaultAllocator(),
                                                                              ek::common::getDefaultAllocator());
    mainLoop.addSource(pMainQueue);
    g_pMainQueue = pMainQueue.get();

    ek::dispatch::EventLoop bgLoop;
    auto pBgQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::getDefaultAllocator(),
                                                                            ek::common::getDefaultAllocator());
    bgLoop.addSource(pBgQueue);
    g_pBackgroundQueue = pBgQueue.get();

    ek::dispatch::setCurrentDispatchQueue(pMainQueue.get());

    std::thread bgThread([&]{
        ek::common::pushAllocator(ek::common::getDefaultAllocator());
        ek::dispatch::setCurrentDispatchQueue(pBgQueue.get());
        bgLoop.run();
    });

    pMainQueue->dispatchAsync(ek::common::getDefaultAllocator(), []{
        innerMain();
    });

    mainLoop.run();

    bgThread.join();

    return 0;
}

ek::dispatch::DispatchAwaitable switchToMainThread() {
    return ek::dispatch::DispatchAwaitable { g_pMainQueue };
}

ek::dispatch::DispatchAwaitable switchToBackgroundThread() {
    return ek::dispatch::DispatchAwaitable { g_pBackgroundQueue };
}

ek::promise::Promise<Unit, std::exception_ptr> after(std::chrono::system_clock::time_point timePoint) {
    std::this_thread::sleep_until(timePoint);
    co_return {};
}
