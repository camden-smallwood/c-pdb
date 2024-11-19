#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"

struct pdb_header
{
    uint32_t version;
    uint32_t signature;
    uint32_t age;
};
static_assert(sizeof(struct pdb_header) == 12);

void pdb_header_read(struct pdb_header *header, struct msf *msf, FILE *stream);
void pdb_header_print(struct pdb_header *header, uint32_t depth, FILE *stream);

struct pdb_named_stream
{
    uint32_t name_offset;
    uint32_t stream_index;
};
static_assert(sizeof(struct pdb_named_stream) == 8);

void pdb_named_stream_print(struct pdb_named_stream *named_stream, uint32_t depth, FILE *file_stream);

struct pdb_info
{
    struct pdb_header pdb_header;
    
    uint32_t guid[4];

    uint32_t names_size;
    char *names_data;

    uint32_t entry_count;
    uint32_t entries_size;

    uint32_t ok_words;
    uint32_t *ok_bits;

    uint32_t deleted_words;
    uint32_t *deleted_bits;

    struct pdb_named_stream *entries;
};

void pdb_info_read(struct pdb_info *info, struct msf *msf, FILE *stream);
void pdb_info_dispose(struct pdb_info *info);
void pdb_info_print(struct pdb_info *info, uint32_t depth, FILE *stream);

enum
{
    PDB_STRING_TABLE_HEADER_MAGIC = 0xEFFEEFFE,

    PDB_STRING_TABLE_HASH_V1 = 1,
    PDB_STRING_TABLE_HASH_V2 = 2,
};

struct pdb_string_table_header
{
    uint32_t magic;
    uint32_t hash_version;
    uint32_t names_size;
};
static_assert(sizeof(struct pdb_string_table_header) == 12);

void pdb_string_table_header_print(struct pdb_string_table_header *header, uint32_t depth, FILE *stream);

struct pdb_string_table
{
    struct pdb_string_table_header header;
    char *names_data;
};

void pdb_string_table_read(struct pdb_string_table *table, struct msf *msf, struct pdb_info *pdb_info, FILE *stream);
void pdb_string_table_dispose(struct pdb_string_table *table);
void pdb_string_table_print(struct pdb_string_table *table, uint32_t depth, FILE *stream);
