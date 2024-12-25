#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char *string_append(char **string, char *suffix)
{
    assert(string);
    assert(suffix);

    if (!*string)
    {
        *string = strdup(suffix);
        assert(*string);
        return *string;
    }

    size_t string_length = strlen(*string);
    size_t suffix_length = strlen(suffix);

    *string = realloc(*string, string_length + suffix_length + 1);
    assert(*string);
    
    sprintf(*string + string_length, "%s", suffix);
    return *string;
}

char *string_prepend(char **string, char *prefix)
{
    assert(string);
    assert(prefix);

    if (!*string)
    {
        *string = strdup(prefix);
        assert(*string);
        return *string;
    }

    size_t prefix_length = strlen(prefix);
    size_t string_length = strlen(*string);
    
    *string = realloc(*string, prefix_length + string_length + 1);
    assert(*string);
    
    memmove(*string + prefix_length, *string, string_length + 1);
    memcpy(*string, prefix, prefix_length);
    return *string;
}

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
    assert(stream);
    
    char *temp = calloc(length + 1, sizeof(char));
    assert(temp);

    snprintf(temp, length, "%s", string);
    
    fprintf(stream, "\"%s\"", temp);

    free(temp);
}

void hex_string_print(
    char *string,
    uint32_t length,
    FILE *stream)
{
    assert(string);
    assert(stream);

    for (uint32_t i = 0; i < length; i++)
        fprintf(stream, "%02X", (unsigned char)string[i]);
}
