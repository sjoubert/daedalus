cmake_minimum_required(VERSION 3.17...3.17)

project(daedalus
  VERSION 0.1
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(common_options)

add_executable(daedalus
  src/cell.hpp
  src/constants.hpp
  src/generator.hpp
  src/generator.cpp
  src/main.cpp
  src/maze.hpp
  src/maze.cpp
)

find_package(SFML COMPONENTS graphics REQUIRED)
target_link_libraries(daedalus PRIVATE
  sfml-graphics
)
