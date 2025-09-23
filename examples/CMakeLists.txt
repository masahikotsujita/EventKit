# dispatchサンプル
add_executable(eventkitexample dispatch_sample/main.cpp)
target_link_libraries(eventkitexample eventkit ${CMAKE_THREAD_LIBS_INIT})

# promiseサンプル
add_executable(promiseexample promise_sample/main.cpp sample_utils/logging.h sample_utils/logging.cpp)
target_link_libraries(promiseexample eventkit ${CMAKE_THREAD_LIBS_INIT})

# coroutineサンプル
if(EVENTKIT_ENABLE_COROUTINE)
    add_executable(coroutinesample coroutine/innerMain.cpp sample_utils/logging.h sample_utils/logging.cpp coroutine/main.h coroutine/main.cpp)
    target_link_libraries(coroutinesample eventkit ${CMAKE_THREAD_LIBS_INIT})
endif()