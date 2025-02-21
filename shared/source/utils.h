#pragma once
#include <stdint.h>
#include <stdio.h>
#include <time.h>

/* ---------- timing macros */

#define TIMED(x) \
    do \
    { \
        clock_t start_time = clock(); \
        x; \
        clock_t end_time = clock(); \
        double cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; \
        printf("Took %f seconds:\n    " #x "\n", cpu_time_used); \
    } while(0)

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
        assert(count);                                        \
        (count)--;                                            \
        (array) = realloc((array), (count) * (element_size)); \
        if (count)                                            \
        {                                                     \
            assert(array);                                    \
        }                                                     \
    } while (0)

/* ---------- min/max macros */

#ifndef __min
#define __min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef __max
#define __max(a,b) (((a) > (b)) ? (a) : (b))
#endif

/* ---------- string functions */

char *string_append(char **string, char *suffix);
char *string_prepend(char **string, char *prefix);
int string_starts_with(char *string, char *starts_with);
int string_ends_with(char *string, char *ends_with);
void string_split(char *string, char c, size_t *out_count, char ***out_strings);
void string_lower(char *string);
void string_upper(char *string);
char *string_to_lower(const char *string);
char *string_to_upper(const char *string);

/* ---------- print functions */

#if defined(__GNUC__) || defined(__clang__)
__attribute__((__format__(__printf__, 3, 4)))
#endif
void fprintf_depth(FILE *stream, uint32_t depth, char *fmt, ...);

void fixed_string_print(char *string, uint32_t length, FILE *stream);

void hex_string_print(char *string, uint32_t length, FILE *stream);
