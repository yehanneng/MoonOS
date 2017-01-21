# this is required
SET(CMAKE_SYSTEM_NAME Generic)

set(TOOL_CHAIN_ROOT "/Users/tangruiwen/opt/cross/bin")

# specify the cross compiler
SET(CMAKE_C_COMPILER ${TOOL_CHAIN_ROOT}/clang)

SET(CMAKE_CXX_COMPILER ${TOOL_CHAIN_ROOT}/clang++)

set(CMAKE_AR ${TOOL_CHAIN_ROOT}/llvm-ar CACHE STRING "llvm-ar")
set(CMAKE_LINKER ${TOOL_CHAIN_ROOT}/ld.lld CACHE STRING "ld.lld")
set(CMAKE_NM ${TOOL_CHAIN_ROOT}/llvm-nm CACHE STRING "llvm-nm")
set(CMAKE_OBJDUMP ${TOOL_CHAIN_ROOT}/llvm-objdump CACHE STRING "llvm-objdump")
set(CMAKE_RANLIB  ${TOOL_CHAIN_ROOT}/llvm-ranlib CACHE STRING "llvm-ranlib")


# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /Users/tangruiwen/opt/cross/lib/5.0.0)

# search for programs in the build host directories (not necessary)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
