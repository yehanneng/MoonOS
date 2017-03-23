
#functions
set(PDC_PLATFORM_SRC_FILES ${PDC_PLATFORM_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_allocpages.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_Exit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_fileops.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_freepages.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_open.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_rename.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/_PDCLIB/_PDCLIB_stdinit.c
        )

#stdio
set(PDC_PLATFORM_SRC_FILES ${PDC_PLATFORM_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/stdio/remove.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/stdio/tmpfile.c
        )

#stdlib
set(PDC_PLATFORM_SRC_FILES ${PDC_PLATFORM_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/stdlib/getenv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/stdlib/system.c
        )

#message
set(PDC_PLATFORM_SRC_FILES ${PDC_PLATFORM_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/message/sendrecv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/message/getticket.c
        ${CMAKE_CURRENT_SOURCE_DIR}/functions/message/sendrecv.asm
        )

set(PDC_PLATFORM_SRC_FILES ${PDC_PLATFORM_SRC_FILES} PARENT_SCOPE)

set(PDC_PLATFORM_HEADER_FILES ${PDC_PLATFORM_HEADER_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/internals/_PDCLIB_config.h
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/signal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/includes/message.h
        )

set(PDC_PLATFORM_HEADER_FILES ${PDC_PLATFORM_HEADER_FILES} PARENT_SCOPE)

set(PDC_PLATFORM_INCLUDES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/includes/ CACHE STRING "internal include path")
set(PDC_PLATFORM_INTERNALS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/internals/ CACHE STRING "internal include path")