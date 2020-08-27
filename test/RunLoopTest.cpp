//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <thread>
#include <catch2/catch.hpp>
#include "TestUtils.h"
#include <eventkit/common/SystemAllocator.h>
#include <eventkit/dispatch/RunLoop.h>
#include <eventkit/dispatch/DispatchQueue.h>

using namespace std::chrono_literals;

ek::common::SystemAllocator g_allocator;

SCENARIO("a run loop", "[run_loop]") {

    GIVEN("a run loop without any event sources") {
        auto pRunLoop = std::make_shared<ek::dispatch::RunLoop>();

        WHEN("the run loop starts running on a thread") {
            auto pFlag = std::make_shared<std::atomic_bool>(false);
            std::thread thread([=]{
                pRunLoop->run();
                *pFlag = true;
            });
            thread.detach();

            THEN("the thread will terminate immediately") {
                REQUIRE_EVENTUALLY(*pFlag, 5s);
            }
        }

        WHEN("a event source added to the run loop") {
            auto pDispatchQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(&g_allocator, &g_allocator);
            pRunLoop->addDispatchQueue(pDispatchQueue);

            WHEN("the run loop starts running on a thread") {
                auto pFlag = std::make_shared<std::atomic_bool>(false);
                std::thread thread([=]{
                    pRunLoop->run();
                    *pFlag = true;
                });
                thread.detach();

                THEN("the thread will keep running") {
                    REQUIRE_EVENTUALLY_NOT(*pFlag, 5s);
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
