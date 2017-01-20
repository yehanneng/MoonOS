cmake_minimum_required(VERSION 3.6.0)
project(MoonOS)

SET(BUILD_ARCH "i386")

#set(LLVM_PATH "/Users/tangruiwen/opt/cross/bin")

#option(LLVM_PATH "llvm root path dir")
if(LLVM_PATH)
    set(CMAKE_C_COMPILER ${LLVM_PATH}/clang)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "--target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib")

    set(CMAKE_CXX_COMPILER ${LLVM_PATH}/clang++)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "--target=i686-pc-none-elf -march=i686 -fno-builtin -nostdlib -nostdinc++")


    set(CMAKE_AR ${LLVM_PATH}/llvm-ar)
    set(CMAKE_LINKER ${LLVM_PATH}/ld.lld)
    set(CMAKE_NM ${LLVM_PATH}/llvm-nm)
    set(CMAKE_OBJDUMP ${LLVM_PATH}/llvm-objdump)
    set(CMAKE_RANLIB  ${LLVM_PATH}/llvm-ranlib)

    set(CMAKE_EXE_LINKER_FLAGS "-m elf_i386")

    set(LINK_FLAGS "<CMAKE_LINKER> --script=${CMAKE_SOURCE_DIR}/kernel/arch/i386/linker.ld <OBJECTS> <LINK_LIBRARIES> -o <TARGET>")

    set(CMAKE_C_LINK_EXECUTABLE ${LINK_FLAGS})
    set(CMAKE_CXX_LINK_EXECUTABLE ${LINK_FLAGS})

    enable_language(ASM_NASM)
    set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS nasm asm)

    set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <FLAGS> -felf32 -o <OBJECT> <SOURCE>")

    add_subdirectory(libc)
    add_subdirectory(kernel)
else()
    message(FATAL_ERROR "LLVM_PATH is not set")

endif()
