cmake_minimum_required(VERSION 3.14)
project(MatrixLib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа (демо)
add_executable(demo main.cpp)

# Подключаем Google Test для тестов
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты
add_executable(runTests test.cpp)
target_link_libraries(runTests gtest_main)

add_test(NAME MatrixTests COMMAND runTests)