#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "msf.h"
#include "pdbi.h"
#include "utils.h"

void pdb_header_print(struct pdb_header *header, uint32_t depth, FILE *stream)
{
    assert(header);
    assert(stream);
    
    fprintf(stream, "pdb_header {\n");
    fprintf_depth(stream, depth + 1, "version: %u,\n", header->version);
    fprintf_depth(stream, depth + 1, "signature: %u,\n", header->signature);
    fprintf_depth(stream, depth + 1, "age: %u,\n", header->age);
    fprintf_depth(stream, depth, "}");
}

void pdb_header_read(struct pdb_header *header, struct msf *msf, FILE *stream)
{
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], 0, sizeof(*header), header, stream);
}

void pdb_named_stream_print(
    struct pdb_named_stream *named_stream,
    uint32_t depth,
    FILE *file_stream)
{
    assert(named_stream);
    assert(file_stream);

    fprintf(file_stream, "pdb_named_stream {\n");
    fprintf_depth(file_stream, depth + 1, "name_offset: %u,\n", named_stream->name_offset);
    fprintf_depth(file_stream, depth + 1, "stream_index: %u,\n", named_stream->stream_index);
    fprintf_depth(file_stream, depth, "}");
}

void pdb_info_read(
    struct pdb_info *info,
    struct msf *msf,
    FILE *stream)
{
    assert(info);
    assert(msf);
    assert(stream);

    uint32_t pdb_offset = 0;

    pdb_header_read(&info->pdb_header, msf, stream);
    pdb_offset += sizeof(info->pdb_header);

    // TODO: not all versions have a guid, so check the version first!
    if (info->pdb_header.version > 19970604)
    {
        msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, sizeof(info->guid), info->guid, stream);
        pdb_offset += sizeof(info->guid);
    }

    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, sizeof(info->names_size), &info->names_size, stream);
    pdb_offset += sizeof(info->names_size);

    info->names_data = malloc(info->names_size);
    assert(info->names_data);
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, info->names_size, info->names_data, stream);
    pdb_offset += info->names_size;
    
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, sizeof(info->entry_count), &info->entry_count, stream);
    pdb_offset += sizeof(info->entry_count);

    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, sizeof(info->entries_size), &info->entries_size, stream);
    pdb_offset += sizeof(info->entries_size);

    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, sizeof(info->ok_words), &info->ok_words, stream);
    pdb_offset += sizeof(info->ok_words);

    info->ok_bits = malloc(info->ok_words * sizeof(*info->ok_bits));
    assert(info->ok_bits);
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, info->ok_words * sizeof(*info->ok_bits), info->ok_bits, stream);
    pdb_offset += info->ok_words * sizeof(*info->ok_bits);

    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, sizeof(info->deleted_words), &info->deleted_words, stream);
    pdb_offset += sizeof(info->deleted_words);

    info->deleted_bits = malloc(info->deleted_words * sizeof(*info->deleted_bits));
    assert(info->deleted_bits);
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, info->deleted_words * sizeof(*info->deleted_bits), info->deleted_bits, stream);
    pdb_offset += info->deleted_words * sizeof(*info->deleted_bits);

    info->entries = malloc(info->entry_count * sizeof(*info->entries));
    assert(info->entries);
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_PDB], pdb_offset, info->entry_count * sizeof(*info->entries), info->entries, stream);
    pdb_offset += info->entry_count * sizeof(*info->entries);
}

void pdb_info_dispose(struct pdb_info *info)
{
    assert(info);

    free(info->names_data);
    free(info->ok_bits);
    free(info->deleted_bits);
    free(info->entries);
}

void pdb_info_print(struct pdb_info *info, uint32_t depth, FILE *stream)
{
    assert(info);
    assert(stream);

    fprintf(stream, "pdb_info {\n");
    fprintf_depth(stream, depth + 1, "pdb_header: ");
    pdb_header_print(&info->pdb_header, depth + 1, stream);
    fprintf(stream, ",\n");
    
    fprintf_depth(stream, depth + 1, "guid: \"%08X%08X%08X%08X\",\n", info->guid[0], info->guid[1], info->guid[2], info->guid[3]);
    fprintf_depth(stream, depth + 1, "names_size: %u,\n", info->names_size);
    fprintf_depth(stream, depth + 1, "entry_count: %u,\n", info->entry_count);
    fprintf_depth(stream, depth + 1, "entries_size: %u,\n", info->entries_size);
    fprintf_depth(stream, depth + 1, "ok_words: %u,\n", info->ok_words);
    
    fprintf_depth(stream, depth + 1, "ok_bits: [");
    for (uint32_t i = 0; i < info->ok_words; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");
        fprintf(stream, "0x%X", info->ok_bits[i]);
    }
    fprintf(stream, "],\n");

    fprintf_depth(stream, depth + 1, "deleted_words: %u,\n", info->deleted_words);

    fprintf_depth(stream, depth + 1, "deleted_bits: [");
    for (uint32_t i = 0; i < info->deleted_words; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");
        fprintf(stream, "0x%X", info->deleted_bits[i]);
    }
    fprintf(stream, "],\n");

    fprintf_depth(stream, depth + 1, "entries: [\n");
    for (uint32_t i = 0; i < info->entry_count; i++)
    {
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        pdb_named_stream_print(&info->entries[i], depth + 2, stream);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}

void pdb_string_table_header_print(struct pdb_string_table_header *header, uint32_t depth, FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "pdb_string_table_header {\n");
    fprintf_depth(stream, depth + 1, "magic: %u\n", header->magic);
    fprintf_depth(stream, depth + 1, "hash_version: %u\n", header->hash_version);
    fprintf_depth(stream, depth + 1, "names_size: %u\n", header->names_size);
    fprintf_depth(stream, depth, "}");
}

void pdb_string_table_read(struct pdb_string_table *table, struct msf *msf, struct pdb_info *pdb_info, FILE *stream)
{
    assert(table);
    assert(msf);
    assert(pdb_info);

    uint32_t names_stream_index = UINT32_MAX;

    for (uint32_t i = 0; i < pdb_info->entry_count; i++)
    {
        char *stream_name = pdb_info->names_data + pdb_info->entries[i].name_offset;

        if (strcmp(stream_name, "/names") == 0)
        {
            names_stream_index = pdb_info->entries[i].stream_index;
            break;
        }
    }

    assert(names_stream_index < msf->stream_count);
    struct msf_stream *names_stream = &msf->streams[names_stream_index];

    memset(&table->header, 0, sizeof(table->header));
    msf_stream_read_data(msf, names_stream, 0, sizeof(table->header), &table->header, stream);
    assert(table->header.magic == PDB_STRING_TABLE_HEADER_MAGIC);

    table->names_data = malloc(table->header.names_size);
    assert(table->names_data);
    msf_stream_read_data(msf, names_stream, sizeof(table->header), table->header.names_size, table->names_data, stream);
}

void pdb_string_table_dispose(struct pdb_string_table *table)
{
    assert(table);

    free(table->names_data);
}

void pdb_string_table_print(struct pdb_string_table *table, uint32_t depth, FILE *stream)
{
    assert(table);
    assert(stream);

    fprintf(stream, "pdb_string_table {\n");
    
    fprintf_depth(stream, depth + 1, "header: ");
    pdb_string_table_header_print(&table->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "names_data: [\n");
    for (uint32_t i = 0; i < table->header.names_size; i++)
    {
        uint32_t length = strlen(table->names_data + i);
        fprintf_depth(stream, depth + 2, "[0x%X..0x%X] = \"%s\",\n", i, i + length, table->names_data + i);
        i += length;
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}
