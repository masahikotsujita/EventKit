cmake_minimum_required(VERSION 3.0)
project(eventkit)

add_compile_options(-std=c++14)

find_package(Threads)
include_directories(include)

set(LIBRARY_SOURCE_FILES
    include/eventkit/detail/Semaphore.h
    include/eventkit/DispatchQueue.h
    include/eventkit/detail/DispatchQueue-inl.h
    include/eventkit/RunLoop.h
    source/DispatchQueue.cpp
    source/RunLoop.cpp
    include/eventkit/Result.h
    include/eventkit/Promise.h
    include/eventkit/Resolver.h
    include/eventkit/detail/ResultHandler.h
    include/eventkit/detail/PromiseCore.h
    include/eventkit/detail/ThenTransformation.h
    include/eventkit/detail/RecoverTransformation.h)

add_library(eventkit ${LIBRARY_SOURCE_FILES})

add_executable(eventkitexample example/main.cpp)
target_link_libraries(eventkitexample eventkit ${CMAKE_THREAD_LIBS_INIT})

add_executable(promiseexample example/promise_example_main.cpp)
target_link_libraries(promiseexample eventkit ${CMAKE_THREAD_LIBS_INIT})

if(BUILD_TEST)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(TEST_SOURCE_FILES
        test/testMain.cpp
        test/TestUtils.h
        test/RunLoopTest.cpp
    )
    add_executable(eventkittest ${TEST_SOURCE_FILES})
    target_link_libraries(eventkittest eventkit ${CMAKE_THREAD_LIBS_INIT})
endif()
