#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include "utils.h"

void fprintf_depth(FILE *stream, uint32_t depth, char *fmt, ...)
{
    assert(stream);
    assert(fmt);
    
    for (uint32_t i = 0; i < depth; i++)
        fprintf(stream, "    ");
    
    va_list va;
    va_start(va, fmt);
    vfprintf(stream, fmt, va);
    va_end(va);
}

void fixed_string_print(
    char *string,
    uint32_t length,
    FILE *stream)
{
    assert(string);
    assert(length);
    
    char *temp = calloc(length + 1, sizeof(char));
    assert(temp);

    snprintf(temp, length, "%s", string);
    
    fprintf(stream, "\"%s\"", temp);

    free(temp);
}
