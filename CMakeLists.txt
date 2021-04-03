cmake_minimum_required(VERSION 3.2)

project(triangle CXX)

add_definitions(-std=c++14)
add_definitions(-DTINYGLTF_IMPLEMENTATION)
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
add_definitions(-DSTB_IMAGE_WRITE_IMPLEMENTATION)

include_directories(src)

file(GLOB_RECURSE SRCS src/*.cpp)

add_library(triangle SHARED ${SRCS})

find_library(log-lib
              log )

target_link_libraries(triangle log GLESv3)

target_include_directories(
        triangle
        PUBLIC
        third_party/glm
        third_party/tinygltf/include
)