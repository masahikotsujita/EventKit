cmake_minimum_required(VERSION 3.12)
project(eventkit LANGUAGES CXX)

option(EVENTKIT_ENABLE_COROUTINE "Enable coroutine support" ON)
option(EVENTKIT_BUILD_TESTS "Build tests" ON)
option(EVENTKIT_BUILD_EXAMPLES "Build examples" OFF)

find_package(Threads REQUIRED)

# ライブラリ
add_library(eventkit
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
target_include_directories(eventkit PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
if(EVENTKIT_ENABLE_COROUTINE)
    add_compile_definitions(EVENTKIT_ENABLE_COROUTINE)
    target_compile_features(eventkit PUBLIC cxx_std_20)
    target_compile_definitions(eventkit PUBLIC EVENTKIT_ENABLE_COROUTINE)
else()
    target_compile_features(eventkit PUBLIC cxx_std_14)
endif()

if(EVENTKIT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(EVENTKIT_BUILD_EXAMPLES)
    add_subdirectory(sample)
endif()