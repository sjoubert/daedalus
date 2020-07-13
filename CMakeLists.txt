cmake_minimum_required(VERSION 3.17...3.17)

project(daedalus
  VERSION 0.1
  LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(common_options)

include(GNUInstallDirs)
# Setup build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_BINDIR})
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_DATADIR})
file(CREATE_LINK
  ${CMAKE_CURRENT_SOURCE_DIR}/res
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_DATADIR}/daedalus
  SYMBOLIC
)

add_executable(daedalus
  src/cell.hpp
  src/generator.hpp
  src/generator.cpp
  src/gui.hpp
  src/item.hpp
  src/item.cpp
  src/items.cpp
  src/main.cpp
  src/maze.hpp
  src/maze.cpp
  src/resources.hpp
  src/resources.cpp
  src/run_state.hpp
  src/run_state.cpp
  src/screen/level_screen.hpp
  src/screen/level_screen.cpp
  src/screen/lost_screen.hpp
  src/screen/lost_screen.cpp
  src/screen/next_level_screen.hpp
  src/screen/next_level_screen.cpp
  src/screen/screen.hpp
  src/screen/screen.cpp
  src/screen/start_screen.hpp
  src/screen/start_screen.cpp
)
target_include_directories(daedalus PRIVATE src/)

find_package(SFML COMPONENTS graphics audio REQUIRED)
mark_as_advanced(SFML_DIR SFML_DOC_DIR)
find_package(ImGui-SFML REQUIRED)
mark_as_advanced(ImGui-SFML_DIR)
target_link_libraries(daedalus PRIVATE
  sfml-graphics
  sfml-audio
  ImGui-SFML::ImGui-SFML
)

install(TARGETS daedalus)
install(DIRECTORY res/ DESTINATION ${CMAKE_INSTALL_DATADIR}/daedalus)

include(CPack)
