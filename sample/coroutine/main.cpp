#include <iostream>
#include <utility>
#include <sstream>
#include <thread>
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/promise/Promise.h>
#include "../sample_utils/logging.h"

struct Unit {};

void innerMain();

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

ek::promise::Promise<std::string, std::exception_ptr> getHello() {
    co_return "hello";
}

ek::promise::Promise<std::string, std::exception_ptr> getWorld() {
    co_return "world";
}

ek::promise::Promise<std::string, std::exception_ptr> getHelloWorld() {
    std::string hello = co_await getHello();
    std::string world = co_await getWorld();
    std::stringstream ss;
    ss << hello << ", " << world << "!";
    co_return ss.str();
}

void innerMain() {
    EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());
    getHelloWorld().then([=](const std::string& helloWorld){
        LOG(helloWorld.c_str());
        return ek::promise::Promise<Unit, std::exception_ptr>::value();
    });
}
