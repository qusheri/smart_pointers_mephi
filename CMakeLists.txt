cmake_minimum_required(VERSION 3.27)
project(smart_pointers_mephi)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.7.2/mingw_64")
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)
add_executable(smart_pointers_mephi
        src/main.cpp
        tests/smart_ptrs_tests.cpp
        tests/smart_ptrs_tests.h
        src/smart_ptrs.h
        tests/memory_checker.h
        tests/memory_checker.cpp
        tests/test_structure.h
)

target_link_libraries(smart_pointers_mephi Qt6::Widgets Qt6::Charts)