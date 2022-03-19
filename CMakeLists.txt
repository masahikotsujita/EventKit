cmake_minimum_required(VERSION 3.0)
project(eventkit)

add_compile_options(-std=c++20)

find_package(Threads)
include_directories(include)

set(LIBRARY_SOURCE_FILES
    include/eventkit/common/Allocator.h
    include/eventkit/common/detail/Allocator-inl.h
    include/eventkit/common/SystemAllocator.h
    include/eventkit/common/IntrusivePtr.h
    include/eventkit/common/detail/IntrusivePtr-inl.h
    include/eventkit/common/IntrusiveObject.h
    include/eventkit/common/IntrusiveObjectMixin.h
    source/common/Allocator.cpp
    source/common/SystemAllocator.cpp
    source/common/IntrusiveObjectMixin.cpp
    include/eventkit/common/Condition.h
    include/eventkit/dispatch/DispatchQueue.h
    include/eventkit/dispatch/detail/DispatchQueue-inl.h
    include/eventkit/dispatch/EventSource.h
    include/eventkit/dispatch/EventLoop.h
    include/eventkit/dispatch/DispatchItem.h
    include/eventkit/dispatch/detail/FunctionDispatchItem.h
    include/eventkit/dispatch/detail/DispatchFunctionItem-inl.h
    include/eventkit/dispatch/DispatchAwaitable.h
    source/dispatch/DispatchQueue.cpp
    source/dispatch/EventLoop.cpp
    include/eventkit/promise/Result.h
    include/eventkit/promise/Promise.h
    include/eventkit/promise/detail/Promise-inl.h
    include/eventkit/promise/detail/make_promise.h
    include/eventkit/promise/detail/make_promise-inl.h
    include/eventkit/promise/Resolver.h
    include/eventkit/promise/ResultHandler.h
    include/eventkit/promise/detail/ResultHandler-inl.h
    include/eventkit/promise/detail/FunctionResultHandler.h
    include/eventkit/promise/detail/FunctionResultHandler-inl.h
    include/eventkit/promise/detail/PromiseCore.h
    include/eventkit/promise/detail/BasicPromiseCore.h
    include/eventkit/promise/detail/ResultHandlerMixin.h
    include/eventkit/promise/detail/ThenTransformationPromiseCore.h
    include/eventkit/promise/detail/RecoverTransformationPromiseCore.h
    include/eventkit/promise/detail/type_traits.h
    include/eventkit/promise/detail/WhenAllTransformationPromiseCore.h
    include/eventkit/promise/global_functions/detail/whenAll-inl.h
    include/eventkit/promise/global_functions/whenAll.h
    include/eventkit/common/AllocatorStack.h
    include/eventkit/common/AllocatorScope.h
    source/common/AllocatorStack.cpp
)

add_library(eventkit ${LIBRARY_SOURCE_FILES})

add_executable(eventkitexample sample/dispatch_sample/main.cpp)
target_link_libraries(eventkitexample eventkit ${CMAKE_THREAD_LIBS_INIT})

add_executable(promiseexample sample/promise_sample/main.cpp sample/sample_utils/logging.h sample/sample_utils/logging.cpp)
target_link_libraries(promiseexample eventkit ${CMAKE_THREAD_LIBS_INIT})

add_executable(coroutinesample sample/coroutine/innerMain.cpp sample/sample_utils/logging.h sample/sample_utils/logging.cpp sample/coroutine/main.h sample/coroutine/main.cpp)
target_link_libraries(coroutinesample eventkit ${CMAKE_THREAD_LIBS_INIT})

if(BUILD_TEST)
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    set(TEST_SOURCE_FILES
        test/testMain.cpp
        test/TestUtils.h
        test/EventLoopTest.cpp
    )
    add_executable(eventkittest ${TEST_SOURCE_FILES})
    target_link_libraries(eventkittest eventkit ${CMAKE_THREAD_LIBS_INIT})
endif()
