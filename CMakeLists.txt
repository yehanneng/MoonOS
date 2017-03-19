cmake_minimum_required(VERSION 3.6.0)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/toolchain.cmake")
project(MoonOS)

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS nasm asm)

set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f elf <FLAGS> -o <OBJECT> <SOURCE>")

add_subdirectory(pdclib)
add_subdirectory(kernel)
