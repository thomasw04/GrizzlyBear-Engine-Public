cmake_minimum_required(VERSION 3.16)
project(GrizzlyBear-Engine CXX C)

set(CMAKE_TOOLCHAIN_FILE conan_toolchain.cmake)


add_subdirectory(GrizzlyCore)
add_subdirectory(GrizzlyRuntime)



