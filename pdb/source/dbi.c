#include <stdlib.h>
#include <string.h>
#include "dbi.h"
#include "utils.h"
#include "macros_print.h"

void dbi_machine_type_print(enum dbi_machine_type item, FILE *stream)
{
    assert(stream);

    DBI_MACHINE_TYPE_ENUM
}

void dbi_header_read(struct dbi_header *header, struct msf *msf, FILE *stream)
{
    assert(header);
    assert(msf);
    assert(stream);

    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_DBI], 0, sizeof(*header), header, stream);
}

void dbi_header_print(struct dbi_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_HEADER_STRUCT
}

void dbi_section_contribution_print(struct dbi_section_contribution *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_SECTION_CONTRIBUTION_STRUCT
}

void dbi_section_contributions_read(
    struct dbi_section_contributions *contributions,
    struct msf *msf,
    struct dbi_header *dbi_header,
    FILE *stream)
{
    assert(msf);
    assert(dbi_header);
    assert(contributions);

    uint32_t start_offset = sizeof(struct dbi_header) + dbi_header->module_list_size;

    msf_stream_read_data(
        msf,
        &msf->streams[MSF_STREAM_DBI],
        start_offset,
        sizeof(contributions->version),
        &contributions->version,
        stream);
    start_offset += sizeof(contributions->version);

    contributions->count = 0;
    contributions->entries = NULL;

    uint32_t current_offset = 0;

    while (current_offset < dbi_header->section_contribution_size)
    {
        contributions->count++;

        contributions->entries = realloc(
            contributions->entries,
            contributions->count * sizeof(*contributions->entries));

        assert(contributions->entries);

        struct dbi_section_contribution *contribution = &contributions->entries[contributions->count - 1];

        msf_stream_read_data(
            msf,
            &msf->streams[MSF_STREAM_DBI],
            start_offset + current_offset,
            sizeof(*contribution),
            contribution,
            stream);

        current_offset += sizeof(*contribution);

        if (contributions->version == 4046541284) // V2
            current_offset += sizeof(uint32_t);   // ???
    }
}

void dbi_section_contributions_dispose(struct dbi_section_contributions *contributions)
{
    assert(contributions);

    free(contributions->entries);
}

void dbi_section_contributions_print(struct dbi_section_contributions *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_SECTION_CONTRIBUTIONS_STRUCT
}

void dbi_module_flags_print(struct dbi_module_flags *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_MODULE_FLAGS_STRUCT
}

void dbi_module_header_print(struct dbi_module_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_MODULE_HEADER_STRUCT
}

void dbi_module_dispose(struct dbi_module *module)
{
    assert(module);

    free(module->module_name);
    free(module->object_file_name);

    cv_symbols_dispose(&module->symbols);

    for (uint32_t i = 0; i < module->c11_lines_subsection_count; i++)
        dbi_subsection_dispose(&module->c11_lines_subsections[i]);

    free(module->c11_lines_subsections);

    for (uint32_t i = 0; i < module->c13_lines_subsection_count; i++)
        dbi_subsection_dispose(&module->c13_lines_subsections[i]);

    free(module->c13_lines_subsections);
}

void dbi_module_print(struct dbi_module *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_MODULE_STRUCT
}

void dbi_modules_read(
    struct dbi_modules *modules,
    struct msf *msf,
    struct dbi_header *dbi_header,
    struct pdb_string_table *string_table,
    FILE *stream)
{
    assert(modules);
    assert(msf);
    assert(dbi_header);
    assert(string_table);
    assert(stream);

    modules->count = 0;
    modules->modules = NULL;

    uint32_t start_offset = sizeof(struct dbi_header);
    uint32_t current_offset = 0;

    while (current_offset < dbi_header->module_list_size)
    {
        modules->modules = realloc(modules->modules, ++modules->count * sizeof(*modules->modules));
        assert(modules->modules);

        struct dbi_module *module = &modules->modules[modules->count - 1];
        memset(module, 0, sizeof(*module));

        msf_stream_read_data(
            msf,
            &msf->streams[MSF_STREAM_DBI],
            start_offset + current_offset,
            sizeof(module->header),
            &module->header,
            stream);
        current_offset += sizeof(module->header);

        uint32_t length = 0;
        module->module_name = msf_stream_read_cstring(
            msf,
            &msf->streams[MSF_STREAM_DBI],
            start_offset + current_offset,
            &length,
            stream);
        current_offset += length;

        length = 0;
        module->object_file_name = msf_stream_read_cstring(
            msf,
            &msf->streams[MSF_STREAM_DBI],
            start_offset + current_offset,
            &length,
            stream);
        current_offset += length;

        uint32_t padding = 4 - (current_offset % 4);

        if (padding < 4)
            current_offset += padding;
    }

    for (uint32_t module_index = 0; module_index < modules->count; module_index++)
    {
        struct dbi_module *module = &modules->modules[module_index];

        if (module->header.stream_index == UINT16_MAX)
            continue;

        assert(module->header.stream_index < msf->stream_count);
        struct msf_stream *module_stream = &msf->streams[module->header.stream_index];

        current_offset = 0;

        //
        // Read the symbols subsection
        //

        uint32_t signature = 0;
        MSF_STREAM_READ(msf, module_stream, &current_offset, signature, stream);

        // TODO: support other versions
        if (signature != CV_SIGNATURE_C13)
        {
            fprintf(stderr, "%s:%i: ERROR: Unsupported CV signature: %u; Expected %u\n", __FILE__, __LINE__, signature, CV_SIGNATURE_C13);
            exit(EXIT_FAILURE);
        }

        cv_symbols_read(&module->symbols, msf, module_stream, module->header.symbols_size, &current_offset, stream);

        //
        // Read C11 lines subsections
        //

        uint32_t c11_lines_start_offset = current_offset;

        while (current_offset < c11_lines_start_offset + module->header.lines_size)
        {
            struct dbi_subsection subsection;
            dbi_subsection_read(&subsection, msf, module_stream, string_table, &current_offset, stream);
            DYNARRAY_PUSH(module->c11_lines_subsections, module->c11_lines_subsection_count, subsection);
        }

        //
        // Read C13 lines subsections
        //

        uint32_t c13_lines_start_offset = current_offset;

        while (current_offset < c13_lines_start_offset + module->header.c13_lines_size)
        {
            struct dbi_subsection subsection;
            dbi_subsection_read(&subsection, msf, module_stream, string_table, &current_offset, stream);
            DYNARRAY_PUSH(module->c13_lines_subsections, module->c13_lines_subsection_count, subsection);
        }
    }
}

void dbi_modules_dispose(struct dbi_modules *modules)
{
    assert(modules);

    free(modules->modules);
}

void dbi_modules_print(struct dbi_modules *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_MODULES_STRUCT
}

void dbi_extra_stream_index_print(enum dbi_extra_stream_index item, FILE *stream)
{
    assert(stream);

    DBI_EXTRA_STREAM_INDEX_ENUM
}

void dbi_extra_streams_read(
    struct dbi_extra_streams *extra_streams,
    struct msf *msf,
    struct dbi_header *dbi_header,
    FILE *stream)
{
    assert(extra_streams);
    assert(msf);
    assert(dbi_header);

    uint32_t start_offset = sizeof(struct dbi_header) + dbi_header->module_list_size + dbi_header->section_contribution_size + dbi_header->section_map_size + dbi_header->file_info_size + dbi_header->type_server_map_size + dbi_header->ec_substream_size;

    extra_streams->count = dbi_header->debug_header_size / sizeof(uint16_t);

    extra_streams->indices = malloc(extra_streams->count * sizeof(uint16_t));
    assert(extra_streams->indices);

    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_DBI], start_offset, dbi_header->debug_header_size, extra_streams->indices, stream);
}

void dbi_extra_streams_dispose(struct dbi_extra_streams *extra_streams)
{
    assert(extra_streams);

    free(extra_streams->indices);
}

void dbi_extra_streams_print(struct dbi_extra_streams *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_EXTRA_STREAMS_STRUCT
}

void dbi_section_header_print(struct dbi_section_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_SECTION_HEADER_STRUCT
}

void dbi_section_headers_read(
    struct msf *msf,
    struct msf_stream *stream,
    struct dbi_section_header **out_headers,
    uint32_t *out_count,
    FILE *file_stream)
{
    assert(stream);

    uint32_t count = stream->size / sizeof(struct dbi_section_header);
    struct dbi_section_header *headers = malloc(count * sizeof(struct dbi_section_header));
    assert(headers);

    msf_stream_read_data(msf, stream, 0, stream->size, headers, file_stream);

    if (out_headers)
        *out_headers = headers;

    if (out_count)
        *out_count = count;
}

void dbi_omap_record_print(struct dbi_omap_record *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_OMAP_RECORD_STRUCT
}

void dbi_omap_records_read(
    struct msf *msf,
    struct msf_stream *stream,
    struct dbi_omap_record **out_records,
    uint32_t *out_count,
    FILE *file_stream)
{
    assert(stream);

    uint32_t count = stream->size / sizeof(struct dbi_omap_record);
    struct dbi_omap_record *records = malloc(count * sizeof(struct dbi_omap_record));
    assert(records);

    msf_stream_read_data(msf, stream, 0, stream->size, records, file_stream);

    if (out_records)
        *out_records = records;

    if (out_count)
        *out_count = count;
}

void dbi_address_map_read(
    struct dbi_address_map *map,
    struct msf *msf,
    struct dbi_extra_streams *extra_streams,
    FILE *file_stream)
{
    assert(map);
    assert(msf);
    assert(extra_streams);
    assert(file_stream);

    memset(map, 0, sizeof(*map));

    if (extra_streams->count > DBI_EXTRA_STREAM_OMAP_TO_SRC &&
        extra_streams->indices[DBI_EXTRA_STREAM_OMAP_TO_SRC] != UINT16_MAX)
    {
        struct msf_stream *stream = &msf->streams[extra_streams->indices[DBI_EXTRA_STREAM_OMAP_TO_SRC]];
        dbi_omap_records_read(msf, stream, &map->omap_to_src_records, &map->omap_to_src_record_count, file_stream);
    }

    if (extra_streams->count > DBI_EXTRA_STREAM_OMAP_FROM_SRC &&
        extra_streams->indices[DBI_EXTRA_STREAM_OMAP_FROM_SRC] != UINT16_MAX)
    {
        struct msf_stream *stream = &msf->streams[extra_streams->indices[DBI_EXTRA_STREAM_OMAP_FROM_SRC]];
        dbi_omap_records_read(msf, stream, &map->omap_from_src_records, &map->omap_from_src_record_count, file_stream);
    }

    if (extra_streams->count > DBI_EXTRA_STREAM_SECTION_HEADERS &&
        extra_streams->indices[DBI_EXTRA_STREAM_SECTION_HEADERS] != UINT16_MAX)
    {
        struct msf_stream *stream = &msf->streams[extra_streams->indices[DBI_EXTRA_STREAM_SECTION_HEADERS]];
        dbi_section_headers_read(msf, stream, &map->section_headers, &map->section_header_count, file_stream);
    }

    if (extra_streams->count > DBI_EXTRA_STREAM_ORIGINAL_SECTION_HEADERS &&
        extra_streams->indices[DBI_EXTRA_STREAM_ORIGINAL_SECTION_HEADERS] != UINT16_MAX)
    {
        struct msf_stream *stream = &msf->streams[extra_streams->indices[DBI_EXTRA_STREAM_ORIGINAL_SECTION_HEADERS]];
        dbi_section_headers_read(msf, stream, &map->original_section_headers, &map->original_section_header_count, file_stream);
    }
}

void dbi_address_map_dispose(struct dbi_address_map *map)
{
    assert(map);

    free(map->omap_to_src_records);
    free(map->omap_from_src_records);
    free(map->section_headers);
    free(map->original_section_headers);
}

void dbi_address_map_print(struct dbi_address_map *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_ADDRESS_MAP_STRUCT
}

void dbi_line_print(struct dbi_line *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_LINE_STRUCT
}

void dbi_line_column_print(struct dbi_line_column *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_LINE_COLUMN_STRUCT
}

void dbi_lines_block_header_print(struct dbi_lines_block_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_LINES_BLOCK_HEADER_STRUCT
}

void dbi_lines_block_dispose(struct dbi_lines_block *item)
{
    assert(item);

    free(item->lines);
    free(item->columns);
}

void dbi_lines_block_print(struct dbi_lines_block *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_LINES_BLOCK_STRUCT
}

void dbi_lines_block_read(
    struct dbi_lines_block *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct dbi_lines_header *lines_header,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(lines_header);
    assert(out_offset);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

    item->line_count = item->header.line_count;
    item->lines = malloc(item->line_count * sizeof(*item->lines));
    assert(item->lines);
    msf_stream_read_data(msf, msf_stream, *out_offset, item->line_count * sizeof(*item->lines), item->lines, file_stream);
    *out_offset += item->line_count * sizeof(*item->lines);

    if (lines_header->flags & CV_LINES_HAVE_COLUMNS)
    {
        item->column_count = item->header.line_count;
        item->columns = malloc(item->column_count * sizeof(*item->columns));
        assert(item->columns);
        msf_stream_read_data(msf, msf_stream, *out_offset, item->column_count * sizeof(*item->columns), item->columns, file_stream);
        *out_offset += item->column_count * sizeof(*item->columns);
    }

    *out_offset = start_offset + item->header.block_size;
}

void dbi_lines_header_print(struct dbi_lines_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_LINES_HEADER_STRUCT
}

void dbi_lines_header_read(
    struct dbi_lines_header *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    cv_pe_section_offset_read(&item->offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->code_size, file_stream);
}

void dbi_lines_dispose(struct dbi_lines *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->block_count; i++)
        dbi_lines_block_dispose(&item->blocks[i]);

    free(item->blocks);
}

void dbi_lines_print(struct dbi_lines *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_LINES_STRUCT
}

void dbi_lines_read(
    struct dbi_lines *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint32_t size,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    dbi_lines_header_read(&item->header, msf, msf_stream, out_offset, file_stream);

    while (*out_offset < start_offset + size)
    {
        struct dbi_lines_block block;
        dbi_lines_block_read(&block, msf, msf_stream, &item->header, out_offset, file_stream);
        DYNARRAY_PUSH(item->blocks, item->block_count, block);
    }

    *out_offset = start_offset + size;
}

void dbi_file_checksum_type_print(enum dbi_file_checksum_type item, FILE *stream)
{
    assert(stream);

    DBI_FILE_CHECKSUM_TYPE_ENUM
}

void dbi_file_checksum_header_print(struct dbi_file_checksum_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_FILE_CHECKSUM_HEADER_STRUCT
}

void dbi_file_checksum_dispose(struct dbi_file_checksum *item)
{
    assert(item);

    free(item->data);
}

void dbi_file_checksum_print(struct dbi_file_checksum *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_FILE_CHECKSUM_STRUCT
}

void dbi_file_checksum_read(
    struct dbi_file_checksum *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct pdb_string_table *string_table,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(string_table);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->header.name_offset, file_stream);
    assert(item->header.name_offset < string_table->header.names_size);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->header.size, file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->header.type, file_stream);

    item->data = malloc(item->header.size);
    assert(item->data);
    msf_stream_read_data(msf, msf_stream, *out_offset, item->header.size, item->data, file_stream);
    *out_offset += item->header.size;

    uint32_t padding = 4 - (*out_offset % 4);

    if (padding < 4)
        *out_offset += padding;
}

void dbi_file_checksums_dispose(struct dbi_file_checksums *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->count; i++)
        dbi_file_checksum_dispose(&item->entries[i]);

    free(item->entries);
}

void dbi_file_checksums_print(struct dbi_file_checksums *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_FILE_CHECKSUMS_STRUCT
}

void dbi_file_checksums_read(
    struct dbi_file_checksums *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct pdb_string_table *string_table,
    uint32_t *out_offset,
    uint32_t size,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(string_table);
    assert(out_offset);
    assert(size);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    uint32_t start_offset = *out_offset;

    while (*out_offset < (start_offset + size))
    {
        struct dbi_file_checksum entry;
        dbi_file_checksum_read(&entry, msf, msf_stream, string_table, out_offset, file_stream);
        DYNARRAY_PUSH(item->entries, item->count, entry);
    }

    *out_offset = start_offset + size;
}

void dbi_inlinee_line_dispose(struct dbi_inlinee_line *item)
{
    assert(item);

    free(item->file_name_offsets);
}

void dbi_inlinee_line_print(struct dbi_inlinee_line *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_INLINEE_LINE_STRUCT
}

void dbi_inlinee_line_read(
    struct dbi_inlinee_line *item,
    uint32_t signature,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->inlinee_id_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->file_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->line, file_stream);

    if (signature == CV_INLINEE_SOURCE_LINE_SIGNATURE_EX)
    {
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->file_count, file_stream);

        item->file_name_offsets = malloc(item->file_count * sizeof(*item->file_name_offsets));
        assert(item->file_name_offsets);

        msf_stream_read_data(msf, msf_stream, *out_offset, item->file_count * sizeof(*item->file_name_offsets), item->file_name_offsets, file_stream);
        *out_offset += item->file_count * sizeof(*item->file_name_offsets);
    }
}

void dbi_inlinee_lines_dispose(struct dbi_inlinee_lines *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->count; i++)
        dbi_inlinee_line_dispose(&item->lines[i]);

    free(item->lines);
}

void dbi_inlinee_lines_print(struct dbi_inlinee_lines *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_INLINEE_LINES_STRUCT
}

void dbi_inlinee_lines_read(
    struct dbi_inlinee_lines *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint32_t size,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->signature, file_stream);

    while (*out_offset < start_offset + size)
    {
        struct dbi_inlinee_line line;
        dbi_inlinee_line_read(&line, item->signature, msf, msf_stream, out_offset, file_stream);
        DYNARRAY_PUSH(item->lines, item->count, line);
    }
}

void dbi_subsection_type_print(enum dbi_subsection_type item, FILE *stream)
{
    assert(stream);

    DBI_SUBSECTION_TYPE_ENUM
}

void dbi_subsection_dispose(struct dbi_subsection *item)
{
    assert(item);

    switch (item->type)
    {
        // case DEBUG_S_SYMBOLS:
        //     break;

        case DEBUG_S_LINES:
            dbi_lines_dispose(&item->lines);
            break;

        // case DEBUG_S_STRINGTABLE:
        //     break;

    case DEBUG_S_FILECHKSMS:
        dbi_file_checksums_dispose(&item->file_checksums);
        break;

        // case DEBUG_S_FRAMEDATA:
        //     break;

    case DEBUG_S_INLINEELINES:
        dbi_inlinee_lines_dispose(&item->inlinee_lines);
        break;

        // case DEBUG_S_CROSSSCOPEIMPORTS:
        //     break;

        // case DEBUG_S_CROSSSCOPEEXPORTS:
        //     break;

        // case DEBUG_S_IL_LINES:
        //     break;

        // case DEBUG_S_FUNC_MDTOKEN_MAP:
        //     break;

        // case DEBUG_S_TYPE_MDTOKEN_MAP:
        //     break;

        // case DEBUG_S_MERGED_ASSEMBLYINPUT:
        //     break;

        // case DEBUG_S_COFF_SYMBOL_RVA:
        //     break;

    default:
        // fprintf(stderr, "%s:%i: ERROR: unhandled dbi_subsection_type value: ", __FILE__, __LINE__);
        // dbi_subsection_type_print(item->type, stderr);
        // fprintf(stderr, "\n");
        // exit(EXIT_FAILURE);
    }
}

void dbi_subsection_print(struct dbi_subsection *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    DBI_SUBSECTION_STRUCT
}

void dbi_subsection_read(
    struct dbi_subsection *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct pdb_string_table *string_table,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(string_table);
    assert(out_offset);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->size, file_stream);

    uint32_t start_offset = *out_offset;

    switch (item->type)
    {
        // case DEBUG_S_SYMBOLS:
        //     break;

    case DEBUG_S_LINES:
        dbi_lines_read(&item->lines, msf, msf_stream, out_offset, item->size, file_stream);
        break;

        // case DEBUG_S_STRINGTABLE:
        //     break;

    case DEBUG_S_FILECHKSMS:
        dbi_file_checksums_read(&item->file_checksums, msf, msf_stream, string_table, out_offset, item->size, file_stream);
        break;

        // case DEBUG_S_FRAMEDATA:
        //     break;

    case DEBUG_S_INLINEELINES:
        dbi_inlinee_lines_read(&item->inlinee_lines, msf, msf_stream, out_offset, item->size, file_stream);
        break;

        // case DEBUG_S_CROSSSCOPEIMPORTS:
        //     break;

        // case DEBUG_S_CROSSSCOPEEXPORTS:
        //     break;

        // case DEBUG_S_IL_LINES:
        //     break;

        // case DEBUG_S_FUNC_MDTOKEN_MAP:
        //     break;

        // case DEBUG_S_TYPE_MDTOKEN_MAP:
        //     break;

        // case DEBUG_S_MERGED_ASSEMBLYINPUT:
        //     break;

        // case DEBUG_S_COFF_SYMBOL_RVA:
        //     break;

    default:
        // fprintf(stderr, "%s:%i: ERROR: unhandled dbi_subsection_type value: ", __FILE__, __LINE__);
        // dbi_subsection_type_print(item->type, stderr);
        // fprintf(stderr, "\n");
        // exit(EXIT_FAILURE);
    }

    *out_offset = start_offset + item->size;
}
