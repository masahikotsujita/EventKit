//
// Created by Tsujita Masahiko on 2022/03/19.
//

#ifndef EVENTKIT_DISPATCHAWAITABLE_H
#define EVENTKIT_DISPATCHAWAITABLE_H

#include <coroutine>
#include <eventkit/dispatch/DispatchQueue.h>

namespace ek {
namespace dispatch {

struct DispatchAwaitable {
    explicit DispatchAwaitable(ek::dispatch::DispatchQueue* pDispatchQueue)
        : m_pDispatchQueue { pDispatchQueue } {
    }

    auto operator co_await () const {
        struct Awaiter {
            ek::dispatch::DispatchQueue* m_pDispatchQueue;

            bool await_ready() const {
                return false;
            }

            void await_suspend(std::coroutine_handle<> handle) {
                m_pDispatchQueue->dispatchAsync(ek::common::getDefaultAllocator(), [handle]() -> void{
                    handle.resume();
                });
            }

            void await_resume() {}
        };
        return Awaiter { m_pDispatchQueue };
    }
private:
    ek::dispatch::DispatchQueue* m_pDispatchQueue;
};

}
}

#endif //EVENTKIT_DISPATCHAWAITABLE_H
