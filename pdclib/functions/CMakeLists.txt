#source Files

#_dlmalloc
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/_dlmalloc/dlmalloc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/_dlmalloc/dlmalloc.h
        )

# _PDCLIB source files
set(PDC_SRC_FILES ${PDC_SRC_FILES}
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_ascii.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_atomax.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_closeall.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_digits.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_initclocale.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_latin1.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_seed.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_strtox_main.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_strtox_prelim.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/_PDCLIB_utf8.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/assert.c
            ${CMAKE_CURRENT_SOURCE_DIR}/_PDCLIB/stdarg.c
            )

# ctype source files
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isalnum.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isalpha.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isblank.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/iscntrl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isdigit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isgraph.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/islower.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isprint.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/ispunct.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isspace.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isupper.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/isxdigit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/tolower.c
        ${CMAKE_CURRENT_SOURCE_DIR}/ctype/toupper.c
        )

#ERRNO
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/errno/errno.c
        )

#inttypes
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/inttypes/imaxabs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/inttypes/imaxdiv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/inttypes/strtoimax.c
        ${CMAKE_CURRENT_SOURCE_DIR}/inttypes/strtoumax.c
        )
#local
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/locale/_PDCLIB_mb_cur_max.c
        ${CMAKE_CURRENT_SOURCE_DIR}/locale/_PDCLIB_unicodedata.c
        ${CMAKE_CURRENT_SOURCE_DIR}/locale/freelocale.c
        ${CMAKE_CURRENT_SOURCE_DIR}/locale/localeconv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/locale/setlocale.c
        ${CMAKE_CURRENT_SOURCE_DIR}/locale/uselocale.c
        )

#signal
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/signal/raise.c
        ${CMAKE_CURRENT_SOURCE_DIR}/signal/signal.c
        )

#stdio
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_cbprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_filemode.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_fillbuffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_flushbuffer.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_ftell64.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_fvopen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_prepread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_prepwrite.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_print.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_scan.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_PDCLIB_seek.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/_vcbprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/clearerr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fclose.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/feof.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/ferror.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fflush.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fgetc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fgetpos.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fgets.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/flockfile.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fopen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fputc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fputs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fread.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/freopen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fscanf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fseek.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fsetpos.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/ftell.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/ftrylockfile.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/funlockfile.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/fwrite.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/getc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/getchar.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/gets.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/perror.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/printf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/putc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/putchar.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/puts.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/rename.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/rewind.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/scanf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/setbuf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/setvbuf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/snprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/sprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/sscanf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/tmpnam.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/ungetc.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vfprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vfscanf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vscanf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vsnprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vsprintf.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdio/vsscanf.c
        )

#stdlib
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/_Exit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/abort.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/abs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/at_quick_exit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/atexit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/atoi.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/atol.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/atoll.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/bsearch.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/div.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/exit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/labs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/ldiv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/llabs.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/lldiv.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/qsort.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/quick_exit.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/rand.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/srand.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/strtol.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/strtoll.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/strtoul.c
        ${CMAKE_CURRENT_SOURCE_DIR}/stdlib/strtoull.c
        )

#string
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/string/memchr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/memcmp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/memcpy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/memmove.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/memset.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strcat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strchr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strcmp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strcoll.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strcpy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strcspn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strdup.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strerror.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strerror.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strlcat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strlcpy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strlen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strncat.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strncmp.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strncpy.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strndup.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strnlen.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strpbrk.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strrchr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strspn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strstr.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strtok.c
        ${CMAKE_CURRENT_SOURCE_DIR}/string/strxfrm.c
        )

#time
set(PDC_SRC_FILES ${PDC_SRC_FILES}
        ${CMAKE_CURRENT_SOURCE_DIR}/time/clock.c
        ${CMAKE_CURRENT_SOURCE_DIR}/time/time.c
        ${CMAKE_CURRENT_SOURCE_DIR}/time/timespec_get.c
        )

set(PDC_SRC_FILES ${PDC_SRC_FILES} PARENT_SCOPE)

