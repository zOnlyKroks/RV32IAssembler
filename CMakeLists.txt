cmake_minimum_required(VERSION 3.31)
project(RV32IAssembler)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

configure_file(${CMAKE_SOURCE_DIR}/assembleMe.s ${CMAKE_BINARY_DIR}/assembleMe.s COPYONLY)

add_executable(RV32IAssembler ${SRC_FILES})
