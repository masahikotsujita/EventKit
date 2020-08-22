cmake_minimum_required(VERSION 3.0)
project(eventkit)

add_compile_options(-std=c++14)

find_package(Threads)
include_directories(include)

set(LIBRARY_SOURCE_FILES
    include/eventkit/common/IntrusivePtr.h
    include/eventkit/common/IntrusiveObject.h
    include/eventkit/dispatch/detail/Semaphore.h
    include/eventkit/dispatch/DispatchQueue.h
    include/eventkit/dispatch/detail/DispatchQueue-inl.h
    include/eventkit/dispatch/RunLoop.h
    source/dispatch/DispatchQueue.cpp
    source/dispatch/RunLoop.cpp
    include/eventkit/promise/Result.h
    include/eventkit/promise/Promise.h
    include/eventkit/promise/detail/Promise-inl.h
    include/eventkit/promise/Resolver.h
    include/eventkit/promise/ResultObserver.h
    include/eventkit/promise/detail/ResultObserver-inl.h
    include/eventkit/promise/detail/PromiseCore.h
    include/eventkit/promise/detail/ResultObserverMultipleInheritanceHelper.h
    include/eventkit/promise/detail/ThenTransformationCore.h
    include/eventkit/promise/detail/RecoverTransformationCore.h
    include/eventkit/promise/detail/type_traits.h
    include/eventkit/promise/detail/WhenAllTransformationCore.h
    include/eventkit/promise/global_functions/detail/whenAll-inl.h
    include/eventkit/promise/global_functions/whenAll.h
)

add_library(eventkit ${LIBRARY_SOURCE_FILES})

add_executable(eventkitexample sample/dispatch_sample/main.cpp)
target_link_libraries(eventkitexample eventkit ${CMAKE_THREAD_LIBS_INIT})

add_executable(promiseexample sample/promise_sample/main.cpp sample/sample_utils/logging.h sample/sample_utils/logging.cpp)
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
