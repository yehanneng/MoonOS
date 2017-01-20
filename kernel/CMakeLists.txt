
message(${BUILD_ARCH})
set(COMMON_C_FLAGS "-O2 -g")

set(KERNEL_C_FLAGS "-ffreestanding -fbuiltin -Wall -Wextra")
set(KERNEL_CXX_FLAGS "-ffreestanding -fbuiltin -Wall -Wextra")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_C_FLAGS} ${KERNEL_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_C_FLAGS} ${CMAKE_CXX_FLAGS} ${KERNEL_CXX_FLAGS}")

set(KERNEL_SRC kernel/kernel.c
                arch/i386/boot.asm
                arch/i386/crti.asm
                arch/i386/crtn.asm
                arch/i386/tty.c)
include_directories(include)
include_directories(${NEWLIB_ROOT_DIR}/i686-elf/include)

add_executable(kernel ${KERNEL_SRC})
