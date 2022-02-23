#include <sstream>
#include <chrono>
#include <eventkit/promise/Promise.h>
#include "main.h"
#include "../sample_utils/logging.h"


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
