cmake_minimum_required(VERSION 3.6.0)
project(MoonOS)

SET(BUILD_ARCH "i386")

set(CMAKE_TOOLCHAIN_FILE "./toolchain.cmake" cache string "toolchain file")

enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS nasm asm)

set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f elf <FLAGS> -o <OBJECT> <SOURCE>")

add_subdirectory(libc)
add_subdirectory(kernel)
