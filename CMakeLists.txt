cmake_minimum_required(VERSION 3.20)
project(Mutavol VERSION 0.1.5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DEBUG true)

configure_file( ${CMAKE_SOURCE_DIR}/source/config.hpp.in ${CMAKE_BINARY_DIR}/config.hpp)

file(GLOB SOURCES "source/*.cpp" "source/*/*.cpp" "source/*/*/*.cpp")
file(GLOB TEST "tests/*.cpp")
file(GLOB MAIN "main.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})