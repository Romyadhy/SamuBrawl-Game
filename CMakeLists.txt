cmake_minimum_required(VERSION 3.17)
project(SamuBrawl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(app src/main.cpp)

target_include_directories(app PRIVATE
  libs/header
)

target_include_directories(app PRIVATE
  libs/raylib/include
)

target_link_directories(app PRIVATE
  libs/raylib/lib
)

target_link_libraries(app
  raylib
  winmm
  opengl32
)
