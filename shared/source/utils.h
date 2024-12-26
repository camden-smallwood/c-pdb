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

#define DYNARRAY_POP(array, count, element_size)              \
    do                                                        \
    {                                                         \
        (count)--;                                            \
        (array) = realloc((array), (count) * (element_size)); \
        assert(array);                                        \
    } while (0)

/* ---------- string functions */

char *string_append(char **string, char *suffix);
char *string_prepend(char **string, char *prefix);

/* ---------- print functions */

#if defined(__GNUC__) || defined(__clang__)
__attribute__((__format__(__printf__, 3, 4)))
#endif
void fprintf_depth(FILE *stream, uint32_t depth, char *fmt, ...);

void fixed_string_print(char *string, uint32_t length, FILE *stream);

void hex_string_print(char *string, uint32_t length, FILE *stream);
