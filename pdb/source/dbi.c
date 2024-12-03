#include <stdlib.h>
#include <string.h>
#include "dbi.h"
#include "cv.h"
#include "utils.h"

#include "macros_print.h"

void dbi_cv_signature_print(enum dbi_cv_signature item, FILE *stream)
{
    assert(stream);

    DBI_CV_SIGNATURE_ENUM
}

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

    uint32_t start_offset = sizeof(struct dbi_header)
        + dbi_header->module_list_size;
    
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
            current_offset += sizeof(uint32_t); // ???
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
    FILE *stream)
{
    assert(msf);
    assert(dbi_header);
    assert(modules);
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

        struct cv_symbols symbols;
        cv_symbols_read(&symbols, msf, module_stream, module->header.symbols_size, &current_offset, stream);

        // TODO:
        // module->header.lines_size
        // module->header.c13_lines_size
        // module->header.file_count
        // module->header.filename_offsets
        // module->header.source_file_index

        // TODO: remove this VVV
        cv_symbols_dispose(&symbols);
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

    uint32_t start_offset = sizeof(struct dbi_header)
        + dbi_header->module_list_size
        + dbi_header->section_contribution_size
        + dbi_header->section_map_size
        + dbi_header->file_info_size
        + dbi_header->type_server_map_size
        + dbi_header->ec_substream_size;
    
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
