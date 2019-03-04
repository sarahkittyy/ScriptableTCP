cmake_minimum_required(VERSION 3.13)
project(ScriptableTCP)

find_library(SFML 2.5 REQUIRED graphics window audio network system)

file(GLOB_RECURSE main_sources "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE imgui_sources "${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp")

add_compile_options(-Wall -g -Wno-unused-variable)
add_link_options(-g)

add_executable(ScriptableTCP ${main_sources} ${imgui_sources})

set_target_properties(ScriptableTCP PROPERTIES OUTPUT_NAME tcp.out)
set(CMAKE_CXX_STANDARD 17)

target_include_directories(ScriptableTCP PUBLIC "${CMAKE_SOURCE_DIR}/include/" "${CMAKE_SOURCE_DIR}/lib/" "${CMAKE_SOURCE_DIR}/lib/imgui/")

target_link_libraries(ScriptableTCP sfml-graphics sfml-window sfml-network sfml-audio sfml-system GL lua)