cmake_minimum_required(VERSION 3.17...3.17)

project(daedalus
  VERSION 0.1
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(common_options)

add_executable(daedalus
  src/cell.hpp
  src/generator.hpp
  src/generator.cpp
  src/main.cpp
  src/maze.hpp
  src/maze.cpp
  src/run_state.hpp
  src/run_state.cpp
  src/screen/level_screen.hpp
  src/screen/level_screen.cpp
  src/screen/next_level_screen.hpp
  src/screen/next_level_screen.cpp
  src/screen/screen.hpp
  src/screen/screen.cpp
  src/screen/start_screen.hpp
  src/screen/start_screen.cpp
)
target_include_directories(daedalus PRIVATE src/)

find_package(SFML COMPONENTS graphics REQUIRED)
find_package(ImGui-SFML REQUIRED)
target_link_libraries(daedalus PRIVATE
  sfml-graphics
  ImGui-SFML::ImGui-SFML
)
