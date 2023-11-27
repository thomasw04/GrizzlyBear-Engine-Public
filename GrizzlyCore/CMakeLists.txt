cmake_minimum_required(VERSION 3.16)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(Boost REQUIRED)
find_package(glm REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(libsodium REQUIRED)
find_package(luajit REQUIRED)
find_package(stb REQUIRED)

file(GLOB_RECURSE cpp_sources 
    src/*.cpp
    src/*.h
    bindings/*.cpp
    bindings/*.c
    bindings/*.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${cpp_sources})

add_library(GrizzlyCore ${cpp_sources})

set_property(TARGET GrizzlyCore PROPERTY CXX_STANDARD 23)

add_compile_definitions(SPDLOG_FMT_EXTERNAL NOMINMAX)
add_definitions(-DBOOST_ALL_NO_LIB)

target_include_directories(GrizzlyCore
    PRIVATE 
    src/
    PUBLIC 
    bindings/ 
    ${glew_INCLUDE_DIRS} 
    ${glfw3_INCLUDE_DIRS} 
    ${imgui_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIRS} 
    ${glm_INCLUDE_DIRS} 
    ${nlohmann_json_INCLUDE_DIRS} 
    ${fmt_INCLUDE_DIRS} 
    ${spdlog_INCLUDE_DIRS} 
    ${libsodium_INCLUDE_DIRS} 
    ${luajit_INCLUDE_DIRS} 
    ${stb_INCLUDE_DIRS}
)

target_precompile_headers(GrizzlyCore PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/GrizzlyCore/src/gbpch.h>")

target_link_libraries(GrizzlyCore
        ${Boost_LIBRARIES} ${glm_LIBRARIES} ${nlohmann_LIBRARIES}
        ${glew_LIBRARIES} ${glfw3_LIBRARIES} ${imgui_LIBRARIES}
        ${fmt_LIBRARIES} ${spdlog_LIBRARIES} ${stb_LIBRARIES}
        ${luajit_LIBRARIES} ${libsodium_LIBRARIES}
)

if( MSVC_VERSION GREATER_EQUAL 1936 AND MSVC_IDE ) # 17.6+
  # When using /std:c++latest, "Build ISO C++23 Standard Library Modules" defaults to "Yes".
  # Default to "No" instead.
  #
  # As of CMake 3.26.4, there isn't a way to control this property
  # (https://gitlab.kitware.com/cmake/cmake/-/issues/24922),
  # We'll use the MSBuild project system instead
  # (https://learn.microsoft.com/en-us/cpp/build/reference/vcxproj-file-structure)
  file( CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
<Project>
  <ItemDefinitionGroup>
    <ClCompile>
      <BuildStlModules>false</BuildStlModules>
    </ClCompile>
  </ItemDefinitionGroup>
</Project>
]==] @ONLY )
endif()
