cmake_minimum_required(VERSION 3.17...3.17)

project(daedalus
  VERSION 0.1 
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

add_executable(daedalus
  src/cell.hpp
  src/main.cpp
  src/maze.hpp
  src/maze.cpp
)

find_package(SFML COMPONENTS graphics REQUIRED)
target_link_libraries(daedalus PRIVATE
  sfml-graphics
)
