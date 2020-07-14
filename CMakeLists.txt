cmake_minimum_required(VERSION 3.17...3.17)

project(daedalus
  VERSION 0.1
  LANGUAGES CXX
)

include(cmake/common_options.cmake)

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

# Install
install(TARGETS daedalus)
install(DIRECTORY res/ DESTINATION ${CMAKE_INSTALL_DATADIR}/daedalus)
install(DIRECTORY install/share/ DESTINATION ${CMAKE_INSTALL_DATADIR})

# Packaging
include(CPack)

# AppImage
find_program(LINUXDEPLOY_EXECUTABLE linuxdeploy REQUIRED)
mark_as_advanced(LINUXDEPLOY_EXECUTABLE)
set(daedalus_app_dir ${CMAKE_CURRENT_BINARY_DIR}/daedalus.AppDir)

find_package(Git REQUIRED)
execute_process(
  COMMAND ${GIT_EXECUTABLE} rev-list -1 --reverse --abbrev-commit --abbrev=8 HEAD
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  OUTPUT_VARIABLE PROJECT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(daedalus_app_img daedalus-${PROJECT_VERSION}-${PROJECT_HASH}-x86_64)

add_custom_command(
  DEPENDS daedalus
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${daedalus_app_dir}
  COMMAND ${CMAKE_COMMAND} --install ${CMAKE_CURRENT_BINARY_DIR} --prefix ${daedalus_app_dir}/usr
  COMMAND ${CMAKE_COMMAND} -E env VERSION=${PROJECT_VERSION}-${PROJECT_HASH}
    ${LINUXDEPLOY_EXECUTABLE} --verbosity=2 --appdir ${daedalus_app_dir} --output appimage
  COMMAND ${CMAKE_COMMAND} -E rename ${daedalus_app_img}.AppImage ${daedalus_app_img}.run
  OUTPUT ${daedalus_app_img}.run
)
add_custom_target(appimage DEPENDS ${daedalus_app_img}.run)
