cmake_minimum_required(VERSION 4.2)
project(tmpool)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(tmpool STATIC library.cpp
        lru.h
        btree.h
        cache.h)

enable_testing()

add_executable(lru_test lru_test.cpp
        cache_test.cpp)

target_link_libraries(lru_test GTest::gtest_main tmpool)

include(GoogleTest)
gtest_discover_tests(lru_test)