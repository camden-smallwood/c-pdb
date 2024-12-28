#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "macros_decl.h"

/* ---------- MSF header V2 */

static const char msf_header_v2_signature[] = "Microsoft C/C++ program database 2.00\r\n\032JG\0";
static_assert(sizeof(msf_header_v2_signature) == 44, "invalid msf_header_v2_signature size");

#define MSF_HEADER_V2_STRUCT                                                                              \
STRUCT_DECL(msf_header_v2)                                                                                \
    FIELD_PRIMITIVE_FIXED_ARRAY_FMT(char, signature, sizeof(msf_header_v2_signature), fixed_string_print) \
    FIELD_PRIMITIVE(uint32_t, page_size, "%u")                                                            \
    FIELD_PRIMITIVE(uint16_t, start_page, "%u")                                                           \
    FIELD_PRIMITIVE(uint16_t, file_page_count, "%u")                                                      \
    FIELD_PRIMITIVE(uint32_t, root_stream_size, "%u")                                                     \
    FIELD_PRIMITIVE(uint32_t, reserved, "%u")                                                             \
STRUCT_END(msf_header_v2)

MSF_HEADER_V2_STRUCT
static_assert(sizeof(struct msf_header_v2) == 60, "invalid msf_header_v2 size");

void msf_header_v2_print(struct msf_header_v2 *item, uint32_t depth, FILE *stream);
int msf_header_v2_read(struct msf_header_v2 *v2, FILE *stream);

/* ---------- MSF header V7 */

static const char msf_header_v7_signature[] = "Microsoft C/C++ MSF 7.00\r\n\032DS\0\0";
static_assert(sizeof(msf_header_v7_signature) == 32, "invalid msf_header_v7_signature size");

#define MSF_HEADER_V7_STRUCT                                                                              \
STRUCT_DECL(msf_header_v7)                                                                                \
    FIELD_PRIMITIVE_FIXED_ARRAY_FMT(char, signature, sizeof(msf_header_v7_signature), fixed_string_print) \
    FIELD_PRIMITIVE(uint32_t, page_size, "%u")                                                            \
    FIELD_PRIMITIVE(uint32_t, allocation_table_pointer, "0x%X")                                           \
    FIELD_PRIMITIVE(uint32_t, file_page_count, "%u")                                                      \
    FIELD_PRIMITIVE(uint32_t, root_stream_size, "%u")                                                     \
    FIELD_PRIMITIVE(uint32_t, reserved, "%u")                                                             \
    FIELD_PRIMITIVE(uint32_t, root_stream_page_list_page_index, "%u")                                     \
STRUCT_END(msf_header_v7)

MSF_HEADER_V7_STRUCT
static_assert(sizeof(struct msf_header_v7) == 56, "invalid msf_header_v7 size");

void msf_header_v7_print(struct msf_header_v7 *item, uint32_t depth, FILE *stream);
int msf_header_v7_read(struct msf_header_v7 *v7, FILE *stream);

/* ---------- MSF header type */

#define MSF_HEADER_TYPE_ENUM       \
ENUM_DECL(msf_header_type)         \
    ENUM_VALUE(MSF_HEADER_UNKNOWN) \
    ENUM_VALUE(MSF_HEADER_V2)      \
    ENUM_VALUE(MSF_HEADER_V7)      \
ENUM_END(msf_header_type)

MSF_HEADER_TYPE_ENUM

void msf_header_type_print(enum msf_header_type type, FILE *stream);

/* ---------- MSF header */

#define MSF_HEADER_STRUCT                                                                        \
STRUCT_DECL(msf_header)                                                                          \
    FIELD_PRIMITIVE_FMT(enum msf_header_type, type, msf_header_type_print)                       \
    FIELD_UNION_DECL()                                                                           \
    FIELD_UNION_FIELD_STRUCT(struct msf_header_v2, v2, type, MSF_HEADER_V2, msf_header_v2_print) \
    FIELD_UNION_FIELD_STRUCT(struct msf_header_v7, v7, type, MSF_HEADER_V7, msf_header_v7_print) \
    FIELD_UNION_END()                                                                            \
STRUCT_END(msf_header)

MSF_HEADER_STRUCT

void msf_header_print(struct msf_header *header, uint32_t depth, FILE *stream);
int msf_header_read(struct msf_header *header, FILE *stream);

/* ---------- MSF stream index */

#define MSF_STREAM_INDEX_ENUM   \
ENUM_DECL(msf_stream_index)     \
    ENUM_VALUE(MSF_STREAM_DIR)  \
    ENUM_VALUE(MSF_STREAM_PDB)  \
    ENUM_VALUE(MSF_STREAM_TPI)  \
    ENUM_VALUE(MSF_STREAM_DBI)  \
    ENUM_VALUE(MSF_STREAM_IPI)  \
ENUM_END(msf_stream_index)

MSF_STREAM_INDEX_ENUM

/* ---------- MSF stream */

#define MSF_STREAM_STRUCT                                                     \
STRUCT_DECL(msf_stream)                                                       \
    FIELD_PRIMITIVE(uint32_t, size, "%u")                                     \
    FIELD_PRIMITIVE(uint32_t, page_count, "%u")                               \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, page_indices, page_count, "%u") \
STRUCT_END(msf_stream)

MSF_STREAM_STRUCT

void msf_stream_dispose(struct msf_stream *msf_stream);
void msf_stream_print(struct msf_stream *msf_stream, uint32_t depth, FILE *file_stream);

/* ---------- MSF */

#define MSF_STRUCT                                                                           \
STRUCT_DECL(msf)                                                                             \
    FIELD_STRUCT(struct msf_header, header, msf_header_print)                                \
    FIELD_STRUCT(struct msf_stream, root_stream, msf_stream_print)                           \
    FIELD_PRIMITIVE(uint32_t, stream_count, "%u")                                            \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct msf_stream *, streams, stream_count, msf_stream_print) \
STRUCT_END(msf)

MSF_STRUCT

void msf_dispose(struct msf *msf);
void msf_print(struct msf *msf, uint32_t depth, FILE *stream);
void msf_read(struct msf *msf, FILE *stream);

uint32_t msf_get_page_count(struct msf *msf);
void msf_verify_page_index(struct msf *msf, uint32_t page_index);

uint32_t msf_get_page_size(struct msf *msf);
uint32_t msf_get_page_offset(struct msf *msf, uint32_t page_index);
uint32_t msf_get_page_count_for_size(struct msf *msf, uint32_t size);

uint32_t msf_get_root_stream_size(struct msf *msf);

void msf_stream_initialize(struct msf *msf, struct msf_stream *stream, uint32_t size);
void msf_stream_dispose(struct msf_stream *stream);

#define MSF_STREAM_READ(msf, msf_stream, out_offset, destination, file_stream)                                        \
    do                                                                                                                \
    {                                                                                                                 \
        msf_stream_read_data((msf), (msf_stream), *(out_offset), sizeof(destination), &(destination), (file_stream)); \
        *(out_offset) += sizeof(destination);                                                                         \
    } while (0)

void msf_stream_read_data(struct msf *msf, struct msf_stream *stream, uint32_t offset, uint32_t size, void *destination, FILE *file_stream);
char *msf_stream_read_cstring(struct msf *msf, struct msf_stream *stream, uint32_t offset, uint32_t *out_length, FILE *file_stream);
char *msf_stream_read_u8_pascal_string(struct msf *msf, struct msf_stream *msf_stream, uint32_t offset, uint32_t *out_length, FILE *file_stream);
uint32_t msf_stream_read_compressed_unsigned(struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void msf_stream_read_padding(struct msf *msf, struct msf_stream *msf_stream, uint32_t end_offset, uint32_t *out_offset, FILE *file_stream);

uint64_t msf_read_tpi_unsigned(struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
char *msf_read_tpi_lf_string(struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, uint16_t leaf, FILE *file_stream);

char *msf_read_cv_symbol_string(struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, uint16_t symbol_type, FILE *file_stream);
