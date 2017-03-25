# this is required
SET(CMAKE_SYSTEM_NAME Generic)

set(TOOL_CHAIN_ROOT "/Workspace/LLVMHOME/install/bin")

# specify the cross compiler
if(WIN32)
	SET(CMAKE_C_COMPILER ${TOOL_CHAIN_ROOT}/clang.exe)

	SET(CMAKE_CXX_COMPILER ${TOOL_CHAIN_ROOT}/clang++.exe)
	set(CMAKE_LINKER ${TOOL_CHAIN_ROOT}/ld.lld.exe CACHE STRING "ld-link.exe")

	set(CMAKE_AR ${TOOL_CHAIN_ROOT}/llvm-ar.exe CACHE STRING "llvm-ar")
	set(CMAKE_NM ${TOOL_CHAIN_ROOT}/llvm-nm.exe CACHE STRING "llvm-nm")
	set(CMAKE_OBJDUMP ${TOOL_CHAIN_ROOT}/llvm-objdump.exe CACHE STRING "llvm-objdump")
	set(CMAKE_RANLIB  ${TOOL_CHAIN_ROOT}/llvm-ranlib.exe CACHE STRING "llvm-ranlib")
else()
	SET(CMAKE_C_COMPILER ${TOOL_CHAIN_ROOT}/clang)

	SET(CMAKE_CXX_COMPILER ${TOOL_CHAIN_ROOT}/clang++)
	set(CMAKE_LINKER ${TOOL_CHAIN_ROOT}/ld.lld CACHE STRING "ld-link.exe")

	set(CMAKE_AR ${TOOL_CHAIN_ROOT}/llvm-ar CACHE STRING "llvm-ar")
	set(CMAKE_NM ${TOOL_CHAIN_ROOT}/llvm-nm CACHE STRING "llvm-nm")
	set(CMAKE_OBJDUMP ${TOOL_CHAIN_ROOT}/llvm-objdump CACHE STRING "llvm-objdump")
	set(CMAKE_RANLIB  ${TOOL_CHAIN_ROOT}/llvm-ranlib CACHE STRING "llvm-ranlib")
endif()

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /Users/tangruiwen/opt/cross/lib/5.0.0)

# search for programs in the build host directories (not necessary)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
