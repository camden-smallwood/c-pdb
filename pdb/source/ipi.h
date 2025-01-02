#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"
#include "tpi.h"

#include "macros_decl.h"

/* ---------- UDT source lines */

#define TPI_UDT_SRC_LINE_STRUCT \
STRUCT_DECL(tpi_udt_src_line) \
    FIELD_PRIMITIVE(uint32_t, udt_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, file_id_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, line, "%u") \
STRUCT_END(tpi_udt_src_line)

TPI_UDT_SRC_LINE_STRUCT
static_assert(sizeof(struct tpi_udt_src_line) == 12, "invalid tpi_udt_src_line size");

void tpi_udt_src_line_print(struct tpi_udt_src_line *item, uint32_t depth, FILE *stream);

/* ---------- UDT module source lines */

#define TPI_UDT_MOD_SRC_LINE_STRUCT \
STRUCT_DECL(tpi_udt_mod_src_line) \
    FIELD_PRIMITIVE(uint32_t, udt_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, file_string_offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, line, "%u") \
    FIELD_PRIMITIVE(uint16_t, mod, "%u") \
STRUCT_END(tpi_udt_mod_src_line)

#pragma pack(push, 1)
TPI_UDT_MOD_SRC_LINE_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_udt_mod_src_line) == 14, "invalid tpi_udt_mod_src_line size");

void tpi_udt_mod_src_line_print(struct tpi_udt_mod_src_line *item, uint32_t depth, FILE *stream);

/* ---------- string IDs */

#define TPI_STRING_ID_STRUCT \
STRUCT_DECL(tpi_string_id) \
    FIELD_PRIMITIVE(uint32_t, substrings_index, "%u") \
    FIELD_PRIMITIVE(char *, string, "\"%s\"") \
STRUCT_END(tpi_string_id)

TPI_STRING_ID_STRUCT

void tpi_string_id_dispose(struct tpi_string_id *item);
void tpi_string_id_print(struct tpi_string_id *item, uint32_t depth, FILE *stream);

/* ---------- substring lists */

#define TPI_SUBSTR_LIST_STRUCT \
STRUCT_DECL(tpi_substr_list) \
    FIELD_PRIMITIVE(uint32_t , count, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, substring_indices, count, "%u") \
STRUCT_END(tpi_substr_list)

TPI_SUBSTR_LIST_STRUCT

void tpi_substr_list_dispose(struct tpi_substr_list *item);
void tpi_substr_list_print(struct tpi_substr_list *item, uint32_t depth, FILE *stream);

/* ---------- build info */

#define TPI_BUILD_INFO_STRUCT \
STRUCT_DECL(tpi_build_info) \
    FIELD_PRIMITIVE(uint16_t, argument_count, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, argument_id_indices, argument_count, "%u") \
STRUCT_END(tpi_build_info)

TPI_BUILD_INFO_STRUCT

void tpi_build_info_dispose(struct tpi_build_info *item);
void tpi_build_info_print(struct tpi_build_info *item, uint32_t depth, FILE *stream);

/* ---------- function IDs */

#define TPI_FUNC_ID_STRUCT \
STRUCT_DECL(tpi_func_id) \
    FIELD_PRIMITIVE(uint32_t, scope_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, function_type_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_func_id)

TPI_FUNC_ID_STRUCT

void tpi_func_id_dispose(struct tpi_func_id *item);
void tpi_func_id_print(struct tpi_func_id *item, uint32_t depth, FILE *stream);

/* ---------- member function IDs */

#define TPI_MFUNC_ID_STRUCT \
STRUCT_DECL(tpi_mfunc_id) \
    FIELD_PRIMITIVE(uint32_t , parent, "%u") \
    FIELD_PRIMITIVE(uint32_t , function_type_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_mfunc_id)

TPI_MFUNC_ID_STRUCT

void tpi_mfunc_id_dispose(struct tpi_mfunc_id *item);
void tpi_mfunc_id_print(struct tpi_mfunc_id *item, uint32_t depth, FILE *stream);

/* ---------- symbols */

#define IPI_SYMBOL_STRUCT \
STRUCT_DECL(ipi_symbol) \
    FIELD_PRIMITIVE_FMT(uint16_t, type, tpi_leaf_print) \
    FIELD_UNION_DECL() \
        FIELD_UNION_FIELD_STRUCT(struct tpi_udt_src_line, udt_src_line, type, LF_UDT_SRC_LINE, tpi_udt_src_line_print) \
        FIELD_UNION_FIELD_STRUCT(struct tpi_udt_mod_src_line, udt_mod_src_line, type, LF_UDT_MOD_SRC_LINE, tpi_udt_mod_src_line_print) \
        FIELD_UNION_FIELD_STRUCT(struct tpi_string_id, string_id, type, LF_STRING_ID, tpi_string_id_print) \
        FIELD_UNION_FIELD_STRUCT(struct tpi_substr_list, substr_list, type, LF_SUBSTR_LIST, tpi_substr_list_print) \
        FIELD_UNION_FIELD_STRUCT(struct tpi_build_info, build_info, type, LF_BUILDINFO, tpi_build_info_print) \
        FIELD_UNION_FIELD_STRUCT(struct tpi_func_id, func_id, type, LF_FUNC_ID, tpi_func_id_print) \
        FIELD_UNION_FIELD_STRUCT(struct tpi_mfunc_id, mfunc_id, type, LF_MFUNC_ID, tpi_mfunc_id_print) \
    FIELD_UNION_END() \
STRUCT_END(ipi_symbol)

IPI_SYMBOL_STRUCT

void ipi_symbol_dispose(struct ipi_symbol *symbol);
void ipi_symbol_print(struct ipi_symbol *symbol, uint32_t depth, FILE *stream);

#define IPI_SYMBOLS_STRUCT \
STRUCT_DECL(ipi_symbols) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct ipi_symbol *, symbols, count, ipi_symbol_print) \
STRUCT_END(ipi_symbols)

IPI_SYMBOLS_STRUCT

void ipi_header_read(struct tpi_header *header, struct msf *msf, FILE *stream);

void ipi_symbols_read(struct ipi_symbols *symbols, struct msf *msf, struct msf_stream *msf_stream, struct tpi_header *ipi_header, FILE *file_stream);
void ipi_symbols_dispose(struct ipi_symbols *symbols);
void ipi_symbols_print(struct ipi_symbols *symbols, uint32_t depth, FILE *stream);

uint32_t ipi_index_to_absolute_index(struct tpi_header *ipi_header, struct ipi_symbols *ipi_symbols, uint32_t index);
struct ipi_symbol *ipi_symbol_get(struct tpi_header *ipi_header, struct ipi_symbols *ipi_symbols, uint32_t index);
char *ipi_string_id_to_string(struct tpi_header *ipi_header, struct ipi_symbols *ipi_symbols, uint32_t index);
