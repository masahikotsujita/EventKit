//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <eventkit/common/SystemAllocator.h>
#include <eventkit/promise/Promise.h>
#include <sstream>
#include <eventkit/promise/Promise.h>
#include <eventkit/promise/global_functions/whenAll.h>
#include <vector>
#include "../sample_utils/logging.h"

std::atomic_bool g_isDone { false };
struct Unit {};
struct NoError {};

int main(int argc, const char* argv[]) {
    using Promise = ek::promise::Promise<std::string, int>;
    using namespace std::chrono_literals;
    using namespace ek::promise::global_functions;
    
    whenAll(ek::common::getDefaultAllocator(), {
        Promise(ek::common::getDefaultAllocator(), [argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
            std::thread thread([resolver, argc, argv]{
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
        Promise::value(ek::common::getDefaultAllocator(), ", "),
        Promise(ek::common::getDefaultAllocator(), [argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
            std::thread thread([resolver, argc, argv]{
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
        Promise::value(ek::common::getDefaultAllocator(), "!")
    }).then(ek::common::getDefaultAllocator(), [](const std::vector<std::string>& texts){
        LOG("concatenating...");
        std::string concatenated = texts[0] + texts[1] + texts[2] + texts[3];
        return ek::promise::Promise<std::string, int>::value(ek::common::getDefaultAllocator(), concatenated);
    }).then(ek::common::getDefaultAllocator(), [](const std::string& text){
        LOG("quoting...");
        std::stringstream ss;
        ss << "\"" << text << "\"";
        std::string quoted = ss.str();
        return ek::promise::Promise<std::string, int>::value(ek::common::getDefaultAllocator(), quoted);
    }).then(ek::common::getDefaultAllocator(), [](const std::string& text){
        LOG("succeeded: ", text);
        return ek::promise::Promise<Unit, int>::value(ek::common::getDefaultAllocator());
    }).recover(ek::common::getDefaultAllocator(), [](int error){
        LOG("failed: ", error);
        return ek::promise::Promise<Unit, NoError>::value(ek::common::getDefaultAllocator());
    }).done(ek::common::getDefaultAllocator(), [](const ek::promise::Result<Unit, NoError>& result){
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