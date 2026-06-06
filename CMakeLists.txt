cmake_minimum_required(VERSION 3.20)
project(SamuBrawl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
