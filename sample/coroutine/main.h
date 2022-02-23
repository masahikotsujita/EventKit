//
// Created by Tsujita Masahiko on 2022/02/22.
//

#ifndef EVENTKIT_MAIN_H
#define EVENTKIT_MAIN_H

struct Unit {};

void innerMain();

ek::promise::Promise<Unit, std::exception_ptr> switchToMainThread();

ek::promise::Promise<Unit, std::exception_ptr> switchToBackgroundThread();

ek::promise::Promise<Unit, std::exception_ptr> after(std::chrono::system_clock::time_point timePoint);

#endif //EVENTKIT_MAIN_H
