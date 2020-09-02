//
// Created by Masahiko Tsujita on 2019-08-14.
//

#ifndef EVENTKIT_TESTUTILS_H
#define EVENTKIT_TESTUTILS_H

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <catch2/catch.hpp>

template <typename Clock, typename Duration, typename Predicate>
bool waitUntil(const Predicate& predicate, const std::chrono::time_point<Clock, Duration>& timeout) {
    while (true) {
        if (std::chrono::system_clock::now() >= timeout) {
            return false;
        }
        if (predicate()) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

#define REQUIRE_EVENTUALLY(expression, timeout)                                                                         \
    REQUIRE(waitUntil([&]() -> bool { return (expression); }, std::chrono::system_clock::now() + (timeout)));

#define REQUIRE_EVENTUALLY_NOT(expression, timeout)                                                                     \
    REQUIRE(!waitUntil([&]() -> bool { return (expression); }, std::chrono::system_clock::now() + (timeout)));

#endif //EVENTKIT_TESTUTILS_H
