#include <thread>
#include <sstream>
#include <chrono>
#include <eventkit/promise/Promise.h>
#include "main.h"
#include "../sample_utils/logging.h"

using namespace std::chrono_literals;

#define LOG_WITH_THREAD_ID(...) LOG("[thread:", std::this_thread::get_id(), "] ", __VA_ARGS__)

ek::promise::Promise<std::string, std::exception_ptr> getHello() {
    LOG_WITH_THREAD_ID("getHello() 開始。ここはメインスレッド。バックグラウンドスレッドに切り替えます...");
    co_await switchToBackgroundThread();
    LOG_WITH_THREAD_ID("getHello() 2秒待機開始。ここはバックグラウンドスレッド。");
    co_await after(std::chrono::system_clock::now() + 2s);
    LOG_WITH_THREAD_ID("getHello() 2秒待機完了。ここはバックグラウンドスレッド。メインスレッドに切り替えます...");
    co_await switchToMainThread();
    LOG_WITH_THREAD_ID("getHello() 完了。ここはメインスレッド。");
    co_return "hello";
}

ek::promise::Promise<std::string, std::exception_ptr> getWorld() {
    LOG_WITH_THREAD_ID("getWorld() 開始。ここはメインスレッド。バックグラウンドスレッドに切り替えます...");
    co_await switchToBackgroundThread();
    LOG_WITH_THREAD_ID("getWorld() 3秒待機開始。ここはバックグラウンドスレッド。");
    co_await after(std::chrono::system_clock::now() + 3s);
    LOG_WITH_THREAD_ID("getWorld() 3秒待機完了。ここはバックグラウンドスレッド。メインスレッドに切り替えます...");
    co_await switchToMainThread();
    LOG_WITH_THREAD_ID("getWorld() 完了。ここはメインスレッド。");
    co_return "world";
}

ek::promise::Promise<std::string, std::exception_ptr> getHelloWorld() {
    LOG_WITH_THREAD_ID("getHelloWorld() 開始。ここはメインスレッド。getHello()を非同期実行します...");
    std::string hello = co_await getHello();
    LOG_WITH_THREAD_ID("getHelloWorld() getHello()非同期実行完了。ここはメインスレッド。getWorld()を非同期実行します...");
    std::string world = co_await getWorld();
    LOG_WITH_THREAD_ID("getHelloWorld() getWorld()非同期実行完了。ここはメインスレッド。文字列を結合します...");
    std::stringstream ss;
    ss << hello << ", " << world << "!";
    LOG_WITH_THREAD_ID("getHelloWorld() 完了。");
    co_return ss.str();
}

void innerMain() {
    EK_USING_ALLOCATOR(ek::common::getDefaultAllocator());
    getHelloWorld().then([=](const std::string& helloWorld){
        LOG_WITH_THREAD_ID(helloWorld.c_str());
        return ek::promise::Promise<Unit, std::exception_ptr>::value();
    });
}
