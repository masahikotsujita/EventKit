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

ek::common::IntrusivePtr<ek::dispatch::DispatchQueue> g_pMainQueue = nullptr;
ek::common::IntrusivePtr<ek::dispatch::DispatchQueue> g_pBackgroundQueue = nullptr;

int main(int argc, const char* argv[]) {
    ek::dispatch::EventLoop mainLoop;
    auto pMainQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::getDefaultAllocator(),
                                                                              ek::common::getDefaultAllocator());
    mainLoop.addSource(pMainQueue);

    ek::dispatch::EventLoop bgLoop;
    auto pBgQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(ek::common::getDefaultAllocator(),
                                                                            ek::common::getDefaultAllocator());
    bgLoop.addSource(pBgQueue);

    ek::dispatch::setCurrentDispatchQueue(pMainQueue.get());

    std::thread bgThread([&]{
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

ek::promise::Promise<Unit, std::exception_ptr> switchToMainThread() {
    co_return {};
}

ek::promise::Promise<Unit, std::exception_ptr> switchToBackgroundThread() {
    co_return {};
}

ek::promise::Promise<Unit, std::exception_ptr> after(std::chrono::system_clock::time_point timePoint) {
    co_return {};
}
