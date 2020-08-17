//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <thread>
#include <catch2/catch.hpp>
#include "TestUtils.h"
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/dispatch/DispatchQueue.h>

using namespace std::chrono_literals;

SCENARIO("a run loop", "[run_loop]") {

    GIVEN("a run loop without any event sources") {
        auto pRunLoop = std::make_shared<ek::dispatch::RunLoop>();

        WHEN("the run loop starts running on a thread") {
            auto pThread = std::make_shared<std::thread>([&]{
                pRunLoop->run();
            });

            THEN("the thread will terminate immediately") {
                REQUIRE_RETURN_WITH_TIMEOUT(pThread->join(), 5s);
            }
        }

        WHEN("a event source added to the run loop") {
            auto pDispatchQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>();
            pRunLoop->addDispatchQueue(pDispatchQueue);

            auto pTriplet = std::make_shared<std::tuple<std::mutex, std::condition_variable, std::atomic_bool>>();
            std::get<2>(*pTriplet) = false;

            WHEN("the run loop starts running on a thread") {
                std::thread thread([pTriplet, pRunLoop]{
                    pRunLoop->run();
                    std::get<2>(*pTriplet) = true;
                    std::get<1>(*pTriplet).notify_all();
                });
                thread.detach();

                THEN("the thread will keep running") {
                    std::unique_lock<std::mutex> lock(std::get<0>(*pTriplet));
                    auto timeout = std::chrono::system_clock::now() + 5s;
                    REQUIRE(!std::get<1>(*pTriplet).wait_until(lock, timeout, [&]{ return std::get<2>(*pTriplet).load(); }));
                }

//                WHEN("the event source is removed from the run loop") {
//                    pDispatchQueue->dispatchAsync([=]{
//                        pRunLoop->removeDispatchQueue(pDispatchQueue.get());
//                    });
//
//                    THEN("the thread will terminate immediately") {
//                        REQUIRE_RETURN_WITH_TIMEOUT(pThread->join(), 5s);
//                    }
//                }
            }
        }
    }

}
