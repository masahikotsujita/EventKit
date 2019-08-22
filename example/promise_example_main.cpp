//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <eventkit/promise/Promise.h>
#include <sstream>

std::atomic_bool g_isDone { false };
struct Unit {};
struct NoError {};

int main(int argc, const char* argv[]) {
    using Promise = ek::promise::Promise<std::string, int>;
    using namespace std::chrono_literals;

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
    }).then([](const std::string& text){
        std::cout << "quoting..." << std::endl;
        std::stringstream ss;
        ss << "\"" << text << "\"";
        std::string quoted = ss.str();
        return ek::promise::Promise<std::string, int>([quoted](const ek::promise::Resolver<std::string, int>& resolver){
            resolver.fulfill(quoted);
        });
    }).then([](const std::string& text){
        std::cout << "succeeded: " << text << std::endl;
        return ek::promise::Promise<Unit, int>([](const ek::promise::Resolver<Unit, int>& resolver){
            resolver.fulfill(Unit());
        });
    }).recover([](int error){
        std::cout << "failed: " << error << std::endl;
        return ek::promise::Promise<Unit, NoError>([](const ek::promise::Resolver<Unit, NoError>& resolver){
            resolver.fulfill(Unit());
        });
    }).finally([](const ek::promise::Result<Unit, NoError>& result){
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