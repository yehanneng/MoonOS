//
// Created by Tangruiwen on 2017/2/26.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void assertion_failure(char *exp, char *file, char *base_file, int line)
{
    printf("assert(%s) failed: file: %s, base_file: %s, ln%d\n",
           exp, file, base_file, line);

    abort();
}
