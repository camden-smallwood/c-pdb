#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"

#include "memory_stream.h"
#include "macros_decl.h"

/* ---------- PDB header */

#define PDB_HEADER_STRUCT \
STRUCT_DECL(pdb_header) \
    FIELD_PRIMITIVE(uint32_t, version, "%u") \
    FIELD_PRIMITIVE(uint32_t, signature, "%u") \
    FIELD_PRIMITIVE(uint32_t, age, "%u") \
STRUCT_END(pdb_header)

PDB_HEADER_STRUCT
static_assert(sizeof(struct pdb_header) == 12, "invalid pdb_header size");

void pdb_header_read(struct pdb_header *header, struct msf *msf, struct memory_stream *stream);
void pdb_header_print(struct pdb_header *header, uint32_t depth, FILE *stream);

/* ---------- PDB named streams */

#define PDB_NAMED_STREAM_STRUCT \
STRUCT_DECL(pdb_named_stream) \
    FIELD_PRIMITIVE(uint32_t, name_offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, stream_index, "%u") \
STRUCT_END(pdb_named_stream)

PDB_NAMED_STREAM_STRUCT
static_assert(sizeof(struct pdb_named_stream) == 8, "invalid pdb_named_stream size");

void pdb_named_stream_print(struct pdb_named_stream *named_stream, uint32_t depth, FILE *file_stream);

/* ---------- PDB information */

#define PDB_INFO_STRUCT \
STRUCT_DECL(pdb_info) \
    FIELD_STRUCT(struct pdb_header, pdb_header, pdb_header_print) \
    FIELD_PRIMITIVE_FIXED_ARRAY(uint32_t, guid, 4, "0x%X") \
    FIELD_PRIMITIVE(uint32_t, names_size, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY_FMT(char *, names_data, names_size, fixed_string_print) \
    FIELD_PRIMITIVE(uint32_t, entry_count, "%u") \
    FIELD_PRIMITIVE(uint32_t, entries_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, ok_words, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, ok_bits, ok_words, "%u") \
    FIELD_PRIMITIVE(uint32_t, deleted_words, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, deleted_bits, deleted_words, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct pdb_named_stream *, entries, entry_count, pdb_named_stream_print) \
STRUCT_END(pdb_info)

PDB_INFO_STRUCT

void pdb_info_read(struct pdb_info *info, struct msf *msf, struct memory_stream *stream);
void pdb_info_dispose(struct pdb_info *info);
void pdb_info_print(struct pdb_info *info, uint32_t depth, FILE *stream);

/* ---------- PDB string tables */

enum
{
    PDB_STRING_TABLE_HEADER_MAGIC = 0xEFFEEFFE,
};

#define PDB_STRING_TABLE_HASH_VERSION_ENUM \
ENUM_DECL(pdb_string_table_hash_version) \
    ENUM_VALUE(PDB_STRING_TABLE_HASH_V1, 1) \
    ENUM_VALUE(PDB_STRING_TABLE_HASH_V2, 2) \
ENUM_END(pdb_string_table_hash_version)

PDB_STRING_TABLE_HASH_VERSION_ENUM

void pdb_string_table_hash_version_print(enum pdb_string_table_hash_version version, FILE *stream);

#define PDB_STRING_TABLE_HEADER_STRUCT \
STRUCT_DECL(pdb_string_table_header) \
    FIELD_PRIMITIVE(uint32_t, magic, "%u") \
    FIELD_PRIMITIVE_FMT(uint32_t, hash_version, pdb_string_table_hash_version_print) \
    FIELD_PRIMITIVE(uint32_t, names_size, "%u") \
STRUCT_END(pdb_string_table_header)

PDB_STRING_TABLE_HEADER_STRUCT
static_assert(sizeof(struct pdb_string_table_header) == 12, "invalid pdb_string_table_header size");

void pdb_string_table_header_print(struct pdb_string_table_header *header, uint32_t depth, FILE *stream);

#define PDB_STRING_TABLE_STRUCT \
STRUCT_DECL(pdb_string_table) \
    FIELD_STRUCT(struct pdb_string_table_header, header, pdb_string_table_header_print) \
    FIELD_PRIMITIVE(char *, names_data, "\"%s\"") \
STRUCT_END(pdb_string_table)

PDB_STRING_TABLE_STRUCT

void pdb_string_table_read(struct pdb_string_table *table, struct msf *msf, struct pdb_info *pdb_info, struct memory_stream *stream);
void pdb_string_table_dispose(struct pdb_string_table *table);
void pdb_string_table_print(struct pdb_string_table *table, uint32_t depth, FILE *stream);
