#include <stdarg.h>
#include "utils.h"

void fprintf_depth(FILE *stream, uint32_t depth, char *fmt, ...)
{
    for (uint32_t i = 0; i < depth; i++)
        fprintf(stream, "    ");
    
    va_list va;
    va_start(va, fmt);
    vfprintf(stream, fmt, va);
    va_end(va);
}
