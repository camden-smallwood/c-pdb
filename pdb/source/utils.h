#pragma once
#include <stdint.h>
#include <stdio.h>

/* ---------- dynamic array macros */

#define DYNARRAY_PUSH(array, count, element)                          \
    do                                                                \
    {                                                                 \
        (count)++;                                                    \
        (array) = realloc((array), (count) * sizeof(element));        \
        assert(array);                                                \
        memcpy((array) + ((count) - 1), &(element), sizeof(element)); \
    } while (0)

/* ---------- string functions */

char *string_append(char **string, char *suffix);
char *string_prepend(char **string, char *prefix);

/* ---------- print functions */

__attribute__((__format__(__printf__, 3, 4))) void fprintf_depth(FILE *stream, uint32_t depth, char *fmt, ...);

void fixed_string_print(char *string, uint32_t length, FILE *stream);
