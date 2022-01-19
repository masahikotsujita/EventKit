//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <sstream>
#include <eventkit/common/SystemAllocator.h>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/Resolver.h>
#include <eventkit/promise/global_functions/whenAll.h>
#include "../sample_utils/logging.h"
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/common/AllocatorScope.h>

using namespace std::chrono_literals;

std::atomic_bool g_isDone { false };
struct Unit {};
struct NoError {};

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

void innerMain() {
    using Promise = ek::promise::Promise<std::string, int>;
    using namespace ek::promise::global_functions;
    EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());

    auto getHello = []{
        return Promise([](const ek::promise::Resolver<std::string, int>& resolver) {
            std::thread thread([resolver] {
                LOG("processing...");
                std::this_thread::sleep_for(5s);
                std::string text = "Hello";
                LOG("fulfill with: ", text);
                resolver.fulfill(text);
            });
            thread.detach();
        });
    };

    auto getWorld = []{
        return Promise([](const ek::promise::Resolver<std::string, int>& resolver) {
            std::thread thread([resolver] {
                LOG("processing...");
                std::this_thread::sleep_for(3s);
                std::string text = "world";
                LOG("fulfill with: ", text);
                resolver.fulfill(text);
            });
            thread.detach();
        });
    };

    whenAll({
        getHello(),
        Promise::value(", "),
        getWorld(),
        Promise::value("!")
    }).then([](const std::vector<std::string>& texts) {
        LOG("concatenating...");
        std::string concatenated = texts[0] + texts[1] + texts[2] + texts[3];
        return ek::promise::Promise<std::string, int>::value(concatenated);
    }).then([](const std::string& text) {
        LOG("quoting...");
        std::stringstream ss;
        ss << "\"" << text << "\"";
        std::string quoted = ss.str();
        return ek::promise::Promise<std::string, int>::value(quoted);
    }).then([](const std::string& text) {
        LOG("succeeded: ", text);
        return ek::promise::Promise<Unit, int>::value();
    }).recover([](int error) {
        LOG("failed: ", error);
        return ek::promise::Promise<Unit, NoError>::value();
    }).done([](const ek::promise::Result<Unit, NoError>& result){
        LOG("done. ");
        g_isDone = true;
    });
}
