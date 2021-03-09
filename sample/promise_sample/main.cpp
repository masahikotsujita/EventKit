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
#include <eventkit/common/AllocatorScope.h>

std::atomic_bool g_isDone { false };
struct Unit {};
struct NoError {};

int main(int argc, const char* argv[]) {
    using Promise = ek::promise::Promise<std::string, int>;
    using namespace std::chrono_literals;
    using namespace ek::promise::global_functions;
    EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());

    whenAll({
        Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver) {
            std::thread thread([resolver, argc, argv] {
                LOG("processing...");
                std::this_thread::sleep_for(5s);
                if (argc >= 2) {
                    std::string text = argv[1];
                    LOG("fulfill with: ", text);
                    resolver.fulfill(text);
                } else {
                    int error = -1;
                    LOG("reject with: ", error);
                    resolver.reject(error);
                }
            });
            thread.detach();
        }),
        Promise::value(", "),
        Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver) {
            std::thread thread([resolver, argc, argv] {
                LOG("processing...");
                std::this_thread::sleep_for(3s);
                if (argc >= 3) {
                    std::string text = argv[2];
                    LOG("fulfill with: ", text);
                    resolver.fulfill(text);
                } else {
                    int error = -1;
                    LOG("reject with: ", error);
                    resolver.reject(error);
                }
            });
            thread.detach();
        }),
        Promise::value("!")
    }).then([](const std::vector<std::string>& texts) {
        EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());
        LOG("concatenating...");
        std::string concatenated = texts[0] + texts[1] + texts[2] + texts[3];
        return ek::promise::Promise<std::string, int>::value(concatenated);
    }).then([](const std::string& text) {
        EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());
        LOG("quoting...");
        std::stringstream ss;
        ss << "\"" << text << "\"";
        std::string quoted = ss.str();
        return ek::promise::Promise<std::string, int>::value(quoted);
    }).then([](const std::string& text) {
        EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());
        LOG("succeeded: ", text);
        return ek::promise::Promise<Unit, int>::value();
    }).recover([](int error) {
        EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());
        LOG("failed: ", error);
        return ek::promise::Promise<Unit, NoError>::value();
    }).done([](const ek::promise::Result<Unit, NoError>& result){
        LOG("done. ");
        g_isDone = true;
    });

    while (true) {
        if (g_isDone) {
            break;
        }
        std::this_thread::sleep_for(16ms);
    }

    return 0;
}