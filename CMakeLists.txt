cmake_minimum_required(VERSION 3.20)
project(Mutavol VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(DEBUG false)

configure_file(${CMAKE_SOURCE_DIR}/source/config.hpp.in ${CMAKE_BINARY_DIR}/config.hpp)

file(GLOB SCANNER "source/core/scanner/*.cpp" "source/core/scanner/*/*.cpp")
file(GLOB PARSER "source/core/parser/*.cpp" "source/core/parser/*/*.cpp")
file(GLOB ARGS "source/args/*.cpp" "source/args/*/*.cpp")
file(GLOB UTILS "source/utils/*.cpp" "source/utils/*/*.cpp")
file(GLOB INTERPRETER "source/interpreter/*.cpp" "source/interpreter/*/*.cpp")
file(GLOB CORE ${SCANNER} ${PARSER})
file(GLOB SOURCES "source/*.cpp" ${CORE} ${ARGS} ${UTILS})
file(GLOB MTV "mtv.cpp")
file(GLOB MTVI "mtvi.cpp")

add_executable(mtv ${SOURCES} ${MTV})
add_executable(mtvi ${SOURCES} ${MTVI} ${INTERPRETER})

target_include_directories(mtv PRIVATE ${CMAKE_BINARY_DIR})
target_include_directories(mtvi PRIVATE ${CMAKE_BINARY_DIR})