//
// Created by Masahiko Tsujita on 2019-08-14.
//

#include <thread>
#include <catch2/catch.hpp>
#include "TestUtils.h"
#include <eventkit/common/SystemAllocator.h>
#include <eventkit/dispatch/EventLoop.h>
#include <eventkit/dispatch/DispatchQueue.h>

using namespace std::chrono_literals;

SCENARIO("a event loop", "[event_loop]") {

    GIVEN("a event loop without any event sources") {
        auto pEventLoop = std::make_shared<ek::dispatch::EventLoop>();

        WHEN("the event loop starts running on a thread") {
            auto pFlag = std::make_shared<std::atomic_bool>(false);
            std::thread thread([=]{
                pEventLoop->run();
                *pFlag = true;
            });
            thread.detach();

            THEN("the thread will terminate immediately") {
                REQUIRE_EVENTUALLY(*pFlag, 5s);
            }
        }

        WHEN("a event source added to the event loop") {
            auto pDispatchQueue = ek::common::make_intrusive<ek::dispatch::DispatchQueue>(
                    ek::common::getDefaultAllocator(),
                                                                                          ek::common::getDefaultAllocator());
            pEventLoop->addSource(pDispatchQueue);

            WHEN("the event loop starts running on a thread") {
                auto pFlag = std::make_shared<std::atomic_bool>(false);
                std::thread thread([=]{
                    pEventLoop->run();
                    *pFlag = true;
                });
                thread.detach();

                THEN("the thread will keep running") {
                    REQUIRE_EVENTUALLY_NOT(*pFlag, 5s);
                }

//                WHEN("the event source is removed from the event loop") {
//                    pDispatchQueue->dispatchAsync([=]{
//                        pEventLoop->removeDispatchQueue(pDispatchQueue.getValue());
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
