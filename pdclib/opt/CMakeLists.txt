#Header Files which will export to public

set(PDC_HEADER_FILES ${PDC_HEADER_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/_PDCLIB_threadconfig.h
        PARENT_SCOPE)

set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/call_once.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/cnd_init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/cnd_signal.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/cnd_wait.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/mtx_destroy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/mtx_init.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/mtx_lock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/mtx_timedlock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/mtx_trylock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/mtx_unlock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/thrd_yield.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/tss_create.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/tss_delete.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/tss_get.c
        ${CMAKE_CURRENT_SOURCE_DIR}/nothread/tss_set.c
        PARENT_SCOPE)

set(PDC_THREAD_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/nothread/ CACHE STRING "thread include path")