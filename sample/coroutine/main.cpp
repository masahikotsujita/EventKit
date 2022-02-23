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
