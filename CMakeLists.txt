cmake_minimum_required(VERSION 3.29)
project(sdl2_basic C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/SDL2-2.30.7/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

#add_executable(3drenderer src/main.c src/display.c src/display.h src/matrix.h src/vector.h)
add_executable(
        sdl2_basic
        src/main.c
        src/display.c
        src/display.h
        src/matrix.c
        src/matrix.h
        src/vector.h
        src/matrix.c
        src/vector.c
        src/triangle.h
        src/triangle.c
        src/stb_ds.h
        src/mesh.h
        src/mesh.c
        src/array.c
        src/array.h
        src/lighting.h
        src/lighting.c
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})