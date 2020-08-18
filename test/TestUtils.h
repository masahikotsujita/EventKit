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

#define _TRIPLET_NAME(labelName) __pTriplet_ ## name

#define DECLARE_FLAG(name)                                                          \
    auto _TRIPLET_NAME(name) = std::make_shared<                                    \
        std::tuple<                                                                 \
            std::mutex,                                                             \
            std::condition_variable,                                                \
            std::atomic_bool                                                        \
        >                                                                           \
    >();                                                                            \
    std::get<2>(*_TRIPLET_NAME(name)) = false;

#define SET_FLAG(flagName)                                                          \
    do {                                                                            \
        std::get<2>(*_TRIPLET_NAME(flagName)) = true;                               \
        std::get<1>(*_TRIPLET_NAME(flagName)).notify_all();                         \
    } while(0)

#define REQUIRE_FLAG_SET(flagName, timeout)                                         \
    do {                                                                            \
        std::unique_lock<std::mutex> lock(std::get<0>(*_TRIPLET_NAME(flagName)));   \
        auto timeoutAt = std::chrono::system_clock::now() + timeout;                \
        REQUIRE(std::get<1>(*_TRIPLET_NAME(flagName))                               \
            .wait_until(lock, timeoutAt, [=]{                                       \
                return std::get<2>(*_TRIPLET_NAME(name)).load();                    \
            }));                                                                    \
    } while (0)

#define REQUIRE_NOT_FLAG_SET(flagName, timeout)                                     \
    do {                                                                            \
        std::unique_lock<std::mutex> lock(std::get<0>(*_TRIPLET_NAME(flagName)));   \
        auto timeoutAt = std::chrono::system_clock::now() + timeout;                \
        REQUIRE(!std::get<1>(*_TRIPLET_NAME(flagName))                              \
            .wait_until(lock, timeoutAt, [=]{                                       \
                return std::get<2>(*_TRIPLET_NAME(name)).load();                    \
            }));                                                                    \
    } while (0)

#endif //EVENTKIT_TESTUTILS_H
