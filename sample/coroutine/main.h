//
// Created by Tsujita Masahiko on 2022/02/22.
//

#ifndef EVENTKIT_MAIN_H
#define EVENTKIT_MAIN_H

struct Unit {};

void innerMain();

struct DispatchAwaitable {
    explicit DispatchAwaitable(ek::dispatch::DispatchQueue* pDispatchQueue)
        : m_pDispatchQueue(pDispatchQueue) {
    }

    auto operator co_await () const {
        struct Awaiter {
            ek::dispatch::DispatchQueue* m_pDispatchQueue;

            bool await_ready() const {
                return false;
            }

            void await_suspend(std::coroutine_handle<> handle) {
                m_pDispatchQueue->template dispatchAsync(ek::common::getDefaultAllocator(), [handle]() -> void{
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

DispatchAwaitable switchToMainThread();

DispatchAwaitable switchToBackgroundThread();

ek::promise::Promise<Unit, std::exception_ptr> after(std::chrono::system_clock::time_point timePoint);

#endif //EVENTKIT_MAIN_H
