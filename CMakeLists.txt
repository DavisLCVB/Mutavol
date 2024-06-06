cmake_minimum_required(VERSION 3.20)
project(Mutavol VERSION 0.1.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DEBUG true)

configure_file(${CMAKE_SOURCE_DIR}/source/config.hpp.in ${CMAKE_BINARY_DIR}/config.hpp)

file(GLOB SCANNER "source/core/scanner/*.cpp" "source/core/scanner/*/*.cpp")
file(GLOB ARGS "source/args/*.cpp" "source/args/*/*.cpp")
file(GLOB UTILS "source/utils/*.cpp" "source/utils/*/*.cpp")
file(GLOB CORE ${SCANNER})
file(GLOB SOURCES "source/*.cpp" ${CORE} ${ARGS} ${UTILS})
file(GLOB TEST "tests/*.cpp" "main_test.cpp")
file(GLOB MAIN "main.cpp")

add_executable(mtv ${SOURCES} ${MAIN})
add_executable(mtvt ${SOURCES} ${TEST}
        source/utils/format/format.hpp)

target_include_directories(mtv PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(mtvt PRIVATE ${CMAKE_BINARY_DIR})