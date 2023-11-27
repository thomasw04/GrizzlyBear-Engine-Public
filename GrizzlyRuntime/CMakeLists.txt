cmake_minimum_required(VERSION 3.16)

file(GLOB_RECURSE cpp_sources src/*.cpp src/*.h)
add_executable(GrizzlyRuntime ${cpp_sources})
target_include_directories(GrizzlyRuntime PUBLIC ${PROJECT_SOURCE_DIR}/GrizzlyCore/src)

add_compile_definitions(SPDLOG_FMT_EXTERNAL NOMINMAX)

set_property(TARGET GrizzlyRuntime PROPERTY CXX_STANDARD 23)

target_link_libraries(GrizzlyRuntime GrizzlyCore)