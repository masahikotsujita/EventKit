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

std::atomic_bool g_isDone { false };
struct Unit {};
struct NoError {};

int main(int argc, const char* argv[]) {
    using Promise = ek::promise::Promise<std::string, int>;
    using namespace std::chrono_literals;
    using namespace ek::promise::operators;

    Promise([argc, argv](const ek::promise::Resolver<std::string, int>& resolver){
        std::thread thread([resolver, argc, argv]{
            std::cout << "processing..." << std::endl;
            std::this_thread::sleep_for(5s);
            if (argc >= 2) {
                std::string text = argv[1];
                std::cout << "fulfill with: " << text << std::endl;
                resolver.fulfill(text);
            } else {
                int error = -1;
                std::cout << "reject with: " << error << std::endl;
                resolver.reject(error);
            }
        });
        thread.detach();
    }) | then([](const std::string& text){
        std::cout << "quoting..." << std::endl;
        std::stringstream ss;
        ss << "\"" << text << "\"";
        std::string quoted = ss.str();
        return ek::promise::StaticPromise<std::string, int>::value(quoted);
    }) | then([](const std::string& text){
        std::cout << "succeeded: " << text << std::endl;
        return ek::promise::StaticPromise<Unit, int>::value();
    }) | recover([](int error){
        std::cout << "failed: " << error << std::endl;
        return ek::promise::StaticPromise<Unit, NoError>::value();
    }) | done([](const ek::promise::Result<Unit, NoError>& result){
        std::cout << "done. " << std::endl;
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