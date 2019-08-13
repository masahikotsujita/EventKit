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

template <typename Clock, typename Duration, typename Func>
bool doOnThreadAndWait(const std::chrono::time_point<Clock, Duration>& timeout, Func&& func) {
    auto pMutex = std::make_shared<std::mutex>();
    {
        std::unique_lock<std::mutex> lock(*pMutex);
        auto pFlagAndCv = std::make_shared<std::pair<std::atomic_bool, std::condition_variable>>();
        pFlagAndCv->first = false;
        auto pFunc = std::make_shared<std::function<void()>>(std::forward<Func>(func));
        std::thread thread([pMutex, pFunc, pWeakFlagAndCv=std::weak_ptr<std::pair<std::atomic_bool, std::condition_variable>>(pFlagAndCv)]{
            pFunc->operator()();
            {
                if (auto pFlagAndCv = pWeakFlagAndCv.lock()) {
                    pFlagAndCv->first = true;
                    pFlagAndCv->second.notify_all();
                }
            }
        });
        thread.detach();
        return pFlagAndCv->second.wait_until(lock, timeout, [&]{ return (bool)pFlagAndCv->first; });
    }
}

/**
 * @macro       REQUIRE_RETURN_WITH_TIMEOUT(expr, timeout)
 * @brief       Asserts the expression returns the control in specified duration.
 * @param[in]   expr    the evaluated expression.
 * @param[in]   timeout the time to stop waiting.
 * @note        The expression will be captured by a lambda and evaluated in a thread.\n
 *              So the expression must be thread safe and variables must be able to be captured.\n
 *              If the expression does never return the control, it will cause memory leak.
 */
#define REQUIRE_RETURN_WITH_TIMEOUT(expr, timeout)                      \
    do {                                                                \
        auto timeoutAt = std::chrono::system_clock::now() + timeout;    \
        REQUIRE(doOnThreadAndWait(timeoutAt, [=]{ ( expr ); }));        \
    } while (0)

#define REQUIRE_NOT_RETURN_WITH_TIMEOUT(expr, timeout)                  \
    do {                                                                \
        auto timeoutAt = std::chrono::system_clock::now() + timeout;    \
        REQUIRE(!doOnThreadAndWait(timeoutAt, [=]{ ( expr ); }));       \
    } while (0)

#endif //EVENTKIT_TESTUTILS_H
