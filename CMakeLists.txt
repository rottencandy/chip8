cmake_minimum_required(VERSION 3.0)

project(chip8)

set(CMAKE_CXX_STANDARD 11)

#include_directories(include)
add_executable(out src/chip8/memory.c src/chip8/cpu.c src/chip8/opcodes.c src/chip8/screen.c src/main.c)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")
find_package(SDL)
find_package(SDL_gfx)

target_link_libraries (out
    ${SDL_LIBRARY}
    ${SDLGFX_LIBRARY}
)

# Copy assets to build location
#file(COPY "${CMAKE_SOURCE_DIR}/src/shaders" DESTINATION "${CMAKE_BINARY_DIR}")
