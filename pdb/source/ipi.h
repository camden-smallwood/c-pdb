#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"
#include "tpi.h"

struct tpi_udt_src_line
{
    uint32_t udt_index;
    uint32_t file_id;
    uint32_t line;
};
static_assert(sizeof(struct tpi_udt_src_line) == 12);

void tpi_udt_src_line_print(struct tpi_udt_src_line *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_udt_mod_src_line
{
    uint32_t udt_index;
    uint32_t file_id;
    uint32_t line;
    uint16_t mod;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_udt_mod_src_line) == 14);

void tpi_udt_mod_src_line_print(struct tpi_udt_mod_src_line *item, uint32_t depth, FILE *stream);

struct tpi_string_id
{
    uint32_t substrings_index;
    char *string;
};

void tpi_string_id_dispose(struct tpi_string_id *item);
void tpi_string_id_print(struct tpi_string_id *item, uint32_t depth, FILE *stream);

struct tpi_substr_list
{
    uint32_t count;
    uint32_t *substring_indices;
};

void tpi_substr_list_dispose(struct tpi_substr_list *item);
void tpi_substr_list_print(struct tpi_substr_list *item, uint32_t depth, FILE *stream);

struct tpi_build_info
{
    uint16_t count;
    uint32_t *argument_indices;
};

void tpi_build_info_dispose(struct tpi_build_info *item);
void tpi_build_info_print(struct tpi_build_info *item, uint32_t depth, FILE *stream);

struct tpi_func_id
{
    uint32_t scope_index;
    uint32_t function_type_index;
    char *name;
};

void tpi_func_id_dispose(struct tpi_func_id *item);
void tpi_func_id_print(struct tpi_func_id *item, uint32_t depth, FILE *stream);

struct tpi_mfunc_id
{
    uint32_t parent;
    uint32_t function_type_index;
    char *name;
};

void tpi_mfunc_id_dispose(struct tpi_mfunc_id *item);
void tpi_mfunc_id_print(struct tpi_mfunc_id *item, uint32_t depth, FILE *stream);

struct ipi_symbol
{
    uint16_t type;
    union
    {
        struct tpi_udt_src_line udt_src_line;
        struct tpi_udt_mod_src_line udt_mod_src_line;
        struct tpi_string_id string_id;
        struct tpi_substr_list substr_list;
        struct tpi_build_info build_info;
        struct tpi_func_id func_id;
        struct tpi_mfunc_id mfunc_id;
    };
};

void ipi_symbol_dispose(struct ipi_symbol *symbol);
void ipi_symbol_print(struct ipi_symbol *symbol, uint32_t depth, FILE *stream);

struct ipi_symbols
{
    uint32_t count;
    struct ipi_symbol *symbols;
};

void ipi_header_read(struct tpi_header *header, struct msf *msf, FILE *stream);

void ipi_symbols_read(struct ipi_symbols *symbols, struct msf *msf, struct msf_stream *msf_stream, struct tpi_header *ipi_header, FILE *file_stream);
void ipi_symbols_dispose(struct ipi_symbols *symbols);
void ipi_symbols_print(struct ipi_symbols *symbols, uint32_t depth, FILE *stream);
