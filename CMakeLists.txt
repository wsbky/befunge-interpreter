cmake_policy(SET CMP0050 NEW)

cmake_minimum_required(VERSION 3.10)

project(BefungeInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_compile_options(-Wall -Werror -Wextra -O2)

add_executable(BefungeInterpreter ${SRC_DIR}/befunge.cpp)
target_include_directories(BefungeInterpreter PUBLIC ${INCLUDE_DIR})
