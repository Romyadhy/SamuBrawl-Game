cmake_minimum_required(VERSION 3.20)
project(SamuBrawl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
  src/main.cpp
  src/animation.cpp
)

add_executable(app ${SOURCES})

# add_executable(app WIN32
#   src/main.cpp
#   src/animation.cpp
# )

target_include_directories(app PRIVATE
  ${CMAKE_SOURCE_DIR}/libs/header
  ${CMAKE_SOURCE_DIR}/libs/raylib/include
)

target_link_directories(app PRIVATE
  ${CMAKE_SOURCE_DIR}/libs/raylib/lib
)

target_link_libraries(app
  ${CMAKE_SOURCE_DIR}/libs/raylib/lib/libraylibdll.a
  # raylib
  winmm
  opengl32
  gdi32
)

add_custom_command(TARGET app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/src/assets"
  "$<TARGET_FILE_DIR:app>/assets"
  COMMENT "Copying assets..."
)

add_custom_command(TARGET app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_SOURCE_DIR}/libs/raylib/lib/raylib.dll"
  "$<TARGET_FILE_DIR:app>/raylib.dll"
  COMMENT "Copying raylib.dll..."
)

#NOTE: ReleasePackage
set(RELEASE_DIR "${CMAKE_SOURCE_DIR}/release/SamuBrawlGame")

add_custom_target(release
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${RELEASE_DIR}"

  COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"

  COMMAND ${CMAKE_COMMAND} -E copy
  # "$<TARGET_FILE:app>"
  "${CMAKE_BINARY_DIR}/app.exe"
  "${RELEASE_DIR}/app.exe"

  COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_SOURCE_DIR}/libs/raylib/lib/raylib.dll"
  "${RELEASE_DIR}/raylib.dll"

  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/src/assets"
  "${RELEASE_DIR}/assets"

  DEPENDS app

  COMMENT "Creating SamuBrawl release package..."
)
