cmake_minimum_required(VERSION 3.2)

project(example CXX)

add_definitions(-std=c++14)

add_subdirectory(../../../../../../ triangle)

include_directories(../../../../../../src)

add_library(example SHARED example.cpp)

target_include_directories(example PUBLIC /Users/kenney/github/triangle/src)
target_link_libraries(example triangle)