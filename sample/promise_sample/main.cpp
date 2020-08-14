//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <eventkit/promise/Promise.h>
#include <sstream>
#include <eventkit/promise/StaticPromise.h>
#include <eventkit/promise/Operator.h>
#include <eventkit/promise/operators/then.h>
#include <eventkit/promise/operators/recover.h>
#include <eventkit/promise/operators/done.h>
#include <eventkit/promise/operators/all.h>
#include <vector>
#include "../sample_utils/logging.h"

std::atomic_bool g_isDone { false };
struct Unit {};
struct NoError {};

int main(int argc, const char* argv[]) {
    using Promise = ek::promise::Promise<std::string, int>;
    using namespace std::chrono_literals;
    using namespace ek::promise::operators;
    
    whenAll( std::vector<Promise> {
        Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
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
        Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
            resolver.fulfill(", ");
        }),
        Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
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
        Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
            resolver.fulfill("!");
        })
    }) | then([](const std::vector<std::string>& texts){
        LOG("concatenating...");
        std::string concatenated = texts[0] + texts[1] + texts[2] + texts[3];
        return ek::promise::StaticPromise<std::string, int>::value(concatenated);
    }) | then([](const std::string& text){
        LOG("quoting...");
        std::stringstream ss;
        ss << "\"" << text << "\"";
        std::string quoted = ss.str();
        return ek::promise::StaticPromise<std::string, int>::value(quoted);
    }) | then([](const std::string& text){
        LOG("succeeded: ", text);
        return ek::promise::StaticPromise<Unit, int>::value();
    }) | recover([](int error){
        LOG("failed: ", error);
        return ek::promise::StaticPromise<Unit, NoError>::value();
    }) | done([](const ek::promise::Result<Unit, NoError>& result){
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