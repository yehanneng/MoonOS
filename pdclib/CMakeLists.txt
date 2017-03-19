cmake_minimum_required(VERSION 3.6.0)

project(plibc)
set(COMMON_C_FLAGS "-O2 -g")
#source file to compile
set(PDC_SRC_FILES "")

# header file to include
set(PDC_HEADER_FILES "")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS} --target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib -ffreestanding -fbuiltin -Wall -Wextra -D_PDCLIB_EXTENSIONS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_C_FLAGS} --target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib  -ffreestanding -fbuiltin -Wall -Wextra -D_PDCLIB_EXTENSIONS")

include_directories(includes)
include_directories(internals)

add_subdirectory(includes)
add_subdirectory(internals)
add_subdirectory(platform)
add_subdirectory(opt)
add_subdirectory(functions)

include_directories(${PDC_PLATFORM_INCLUDES_PATH})
include_directories(${PDC_PLATFORM_INTERNALS_PATH})
include_directories(${PDC_THREAD_INCLUDE_PATH})

set(PDCLIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/includes CACHE STRING "PDCLIB INCLUDE DIR")
set(PDCLIB_INTERNAL_DIR ${PROJECT_SOURCE_DIR}/internals CACHE STRING "PDCLIB INCLUDE DIR")

add_library(plibc ${PDC_HEADER_FILES} ${PDC_SRC_FILES} ${PDC_PLATFORM_SRC_FILES} ${PDC_PLATFORM_HEADER_FILES})