project(klibc)
message(${BUILD_ARCH})
set(COMMON_C_FLAGS "-O2 -g")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "--target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib -nostdinc++")

set(KERNEL_C_FLAGS "-ffreestanding -fbuiltin -Wall -Wextra -D__is_myos_kernel")
set(KERNEL_CXX_FLAGS "-ffreestanding -fbuiltin -Wall -Wextra -D__is_myos_kernel")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS} ${KERNEL_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_C_FLAGS} ${CMAKE_CXX_FLAGS} ${KERNEL_CXX_FLAGS}")

set(KLIBC_SRC stdio/printf.c
                stdio/putchar.c
                stdio/puts.c
                stdlib/abort.c
                string/memcmp.c
                string/memcpy.c
                string/memmove.c
                string/memset.c
                string/strlen.c
                )
include_directories(include)
include_directories(../kernel/include)

set(KLIBC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include CACHE STRING "KLIBC INCLUDE DIR")
add_library(klibc ${KLIBC_SRC})
