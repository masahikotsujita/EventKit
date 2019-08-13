cmake_minimum_required(VERSION 3.0)
project(eventkit)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(eventkitexample example/main.cpp include/eventkit/detail/Semaphore.h include/eventkit/RunLoop.h source/RunLoop.cpp include/eventkit/DispatchQueue.h source/DispatchQueue.cpp include/eventkit/detail/DispatchQueue-inl.h)
