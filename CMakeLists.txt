cmake_minimum_required(VERSION 3.0)
project(eventkit)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(LIBRARY_SOURCE_FILES
    include/eventkit/detail/Semaphore.h
    include/eventkit/DispatchQueue.h
    include/eventkit/detail/DispatchQueue-inl.h
    include/eventkit/RunLoop.h
    source/DispatchQueue.cpp
    source/RunLoop.cpp
)

add_library(eventkit ${LIBRARY_SOURCE_FILES})

add_executable(eventkitexample example/main.cpp)
target_link_libraries(eventkitexample eventkit)
