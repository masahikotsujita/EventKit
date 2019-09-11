cmake_minimum_required(VERSION 3.0)
project(eventkit)

add_compile_options(-std=c++14)

find_package(Threads)
include_directories(include)

set(LIBRARY_SOURCE_FILES
    include/eventkit/common/intrusive_ptr.h
    include/eventkit/dispatch/detail/Semaphore.h
    include/eventkit/dispatch/DispatchQueue.h
    include/eventkit/dispatch/detail/DispatchQueue-inl.h
    include/eventkit/dispatch/RunLoop.h
    source/dispatch/DispatchQueue.cpp
    source/dispatch/RunLoop.cpp
    include/eventkit/promise/Result.h
    include/eventkit/promise/Promise.h
    include/eventkit/promise/detail/Promise-inl.h
    include/eventkit/promise/StaticPromise.h
    include/eventkit/promise/Resolver.h
    include/eventkit/promise/ResultObserver.h
    include/eventkit/promise/detail/ResultObserver-inl.h
    include/eventkit/promise/detail/PromiseCore.h
    include/eventkit/promise/detail/ResultObserverMultipleInheritanceHelper.h
    include/eventkit/promise/detail/ThenTransformationCore.h
    include/eventkit/promise/detail/RecoverTransformationCore.h
    include/eventkit/promise/detail/type_traits.h
    include/eventkit/promise/detail/AllTransformationCore.h
    include/eventkit/promise/Operator.h
    include/eventkit/promise/operators/detail/then-inl.h
    include/eventkit/promise/operators/detail/recover-inl.h
    include/eventkit/promise/operators/detail/done-inl.h
    include/eventkit/promise/operators/then.h
    include/eventkit/promise/operators/recover.h
    include/eventkit/promise/operators/done.h
    include/eventkit/promise/operators/all.h
)

add_library(eventkit ${LIBRARY_SOURCE_FILES})

add_executable(eventkitexample sample/dispatch_sample/main.cpp)
target_link_libraries(eventkitexample eventkit ${CMAKE_THREAD_LIBS_INIT})

add_executable(promiseexample sample/promise_sample/main.cpp)
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
