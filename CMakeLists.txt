cmake_minimum_required(VERSION 3.0)
project(eventkit)

add_compile_options(-std=c++14)

find_package(Threads)
include_directories(include)

set(LIBRARY_SOURCE_FILES
    include/eventkit/dispatch/detail/Semaphore.h
    include/eventkit/dispatch/DispatchQueue.h
    include/eventkit/dispatch/detail/DispatchQueue-inl.h
    include/eventkit/dispatch/RunLoop.h
    source/DispatchQueue.cpp
    source/RunLoop.cpp
    include/eventkit/promise/Result.h
    include/eventkit/promise/Promise.h
    include/eventkit/promise/Resolver.h
    include/eventkit/promise/detail/ResultHandler.h
    include/eventkit/promise/detail/PromiseCore.h
    include/eventkit/promise/detail/ThenTransformation.h
    include/eventkit/promise/detail/RecoverTransformation.h
)

add_library(eventkit ${LIBRARY_SOURCE_FILES})

add_executable(eventkitexample example/dispatch_sample/main.cpp)
target_link_libraries(eventkitexample eventkit ${CMAKE_THREAD_LIBS_INIT})

add_executable(promiseexample example/promise_sample/main.cpp)
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
