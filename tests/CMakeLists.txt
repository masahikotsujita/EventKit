cmake_minimum_required(VERSION 3.12)

# Catch2を使用
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.10.0
)
FetchContent_MakeAvailable(Catch2)

# テスト
add_executable(eventkittest
    testMain.cpp
    TestUtils.h
    EventLoopTest.cpp
)
target_link_libraries(eventkittest PRIVATE eventkit ${CMAKE_THREAD_LIBS_INIT} Catch2::Catch2WithMain)
add_test(NAME EventKitTest COMMAND eventkittest)