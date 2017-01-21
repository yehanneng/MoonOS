project(kernel)
message(${BUILD_ARCH})
set(COMMON_C_FLAGS "-O2 -g")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "--target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib -nostdinc++")


set(KERNEL_C_FLAGS "-ffreestanding -fbuiltin -Wall -Wextra")
set(KERNEL_CXX_FLAGS "-ffreestanding -fbuiltin -Wall -Wextra")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS} ${KERNEL_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_C_FLAGS} ${CMAKE_CXX_FLAGS} ${KERNEL_CXX_FLAGS}")

set(LINK_FLAGS "<CMAKE_LINKER> --script=${CMAKE_SOURCE_DIR}/kernel/arch/i386/linker.ld <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")
set(CMAKE_C_LINK_EXECUTABLE ${LINK_FLAGS})
set(CMAKE_CXX_LINK_EXECUTABLE ${LINK_FLAGS})

set(KERNEL_SRC kernel/kernel.c
                arch/i386/boot.asm
                arch/i386/crti.asm
                arch/i386/crtn.asm
                arch/i386/tty.c)
include_directories(include)
include_directories(${KLIBC_INCLUDE_DIR})

add_executable(kernel ${KERNEL_SRC})

target_link_libraries(kernel klibc)
