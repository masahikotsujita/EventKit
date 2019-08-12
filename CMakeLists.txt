cmake_minimum_required(VERSION 3.0)
project(eventkit)

set(CMAKE_CXX_STANDARD 17)

add_executable(eventkitexample example/main.cpp include/detail/Semaphore.h include/RunLoop.h source/RunLoop.cpp include/DispatchQueue.h)
