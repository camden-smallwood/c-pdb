#include <stdlib.h>
#include <string.h>
#include "dbi.h"
#include "utils.h"

void dbi_machine_type_print(enum dbi_machine_type type, FILE *stream)
{
    assert(stream);
    
    switch (type)
    {
    case DBI_MACHINE_TYPE_UNKNOWN:
        fprintf(stream, "Unknown");
        break;
    case DBI_MACHINE_TYPE_AM33:
        fprintf(stream, "Matsushita AM33");
        break;
    case DBI_MACHINE_TYPE_AMD64:
        fprintf(stream, "x64");
        break;
    case DBI_MACHINE_TYPE_ARM:
        fprintf(stream, "ARM little endian");
        break;
    case DBI_MACHINE_TYPE_ARM64:
        fprintf(stream, "ARM64 little endian");
        break;
    case DBI_MACHINE_TYPE_ARM_NT:
        fprintf(stream, "ARM Thumb-2 little endian");
        break;
    case DBI_MACHINE_TYPE_EBC:
        fprintf(stream, "EFI byte code");
        break;
    case DBI_MACHINE_TYPE_X86:
        fprintf(stream, "Intel 386 or later processors and compatible processors");
        break;
    case DBI_MACHINE_TYPE_IA64:
        fprintf(stream, "Intel Itanium processor family");
        break;
    case DBI_MACHINE_TYPE_M32R:
        fprintf(stream, "Mitsubishi M32R little endian");
        break;
    case DBI_MACHINE_TYPE_MIPS16:
        fprintf(stream, "MIPS16");
        break;
    case DBI_MACHINE_TYPE_MIPS_FPU:
        fprintf(stream, "MIPS with FPU");
        break;
    case DBI_MACHINE_TYPE_MIPS_FPU16:
        fprintf(stream, "MIPS16 with FPU");
        break;
    case DBI_MACHINE_TYPE_POWER_PC:
        fprintf(stream, "Power PC little endian");
        break;
    case DBI_MACHINE_TYPE_POWER_PC_FP:
        fprintf(stream, "Power PC with floating point support");
        break;
    case DBI_MACHINE_TYPE_POWER_PC_ALTIVEC:
        fprintf(stream, "Power PC Altivec (TODO: is this correct?)");
        break;
    case DBI_MACHINE_TYPE_R4000:
        fprintf(stream, "MIPS little endian");
        break;
    case DBI_MACHINE_TYPE_RISC_V32:
        fprintf(stream, "RISC-V 32-bit address space");
        break;
    case DBI_MACHINE_TYPE_RISC_V64:
        fprintf(stream, "RISC-V 64-bit address space");
        break;
    case DBI_MACHINE_TYPE_RISC_V128:
        fprintf(stream, "RISC-V 128-bit address space");
        break;
    case DBI_MACHINE_TYPE_SH3:
        fprintf(stream, "Hitachi SH3");
        break;
    case DBI_MACHINE_TYPE_SH3_DSP:
        fprintf(stream, "Hitachi SH3 DSP");
        break;
    case DBI_MACHINE_TYPE_SH4:
        fprintf(stream, "Hitachi SH4");
        break;
    case DBI_MACHINE_TYPE_SH5:
        fprintf(stream, "Hitachi SH5");
        break;
    case DBI_MACHINE_TYPE_THUMB:
        fprintf(stream, "Thumb");
        break;
    case DBI_MACHINE_TYPE_WCE_MIPS_V2:
        fprintf(stream, "MIPS little-endian WCE v2");
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: invalid DBI machine type value: %i\n", __FILE__, __LINE__, type);
        exit(EXIT_FAILURE);
    }
}

void dbi_header_read(struct dbi_header *header, struct msf *msf, FILE *stream)
{
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_DBI], 0, sizeof(*header), header, stream);
}

void dbi_header_print(struct dbi_header *header, uint32_t depth, FILE *stream)
{
    assert(header);
    assert(stream);
    
    fprintf(stream, "dbi_header {\n");
    fprintf_depth(stream, depth + 1, "signature: %u,\n", header->signature);
    fprintf_depth(stream, depth + 1, "version: %u,\n", header->version);
    fprintf_depth(stream, depth + 1, "age: %u,\n", header->age);
    fprintf_depth(stream, depth + 1, "gs_symbols_stream: %u,\n", header->gs_symbols_stream);
    fprintf_depth(stream, depth + 1, "internal_version: %u,\n", header->internal_version);
    fprintf_depth(stream, depth + 1, "ps_symbols_stream: %u,\n", header->ps_symbols_stream);
    fprintf_depth(stream, depth + 1, "pdb_dll_build_version: %u,\n", header->pdb_dll_build_version);
    fprintf_depth(stream, depth + 1, "symbol_records_stream: %u,\n", header->symbol_records_stream);
    fprintf_depth(stream, depth + 1, "pdb_dll_rbld_version: %u,\n", header->pdb_dll_rbld_version);
    fprintf_depth(stream, depth + 1, "module_list_size: %u,\n", header->module_list_size);
    fprintf_depth(stream, depth + 1, "section_contribution_size: %u,\n", header->section_contribution_size);
    fprintf_depth(stream, depth + 1, "section_map_size: %u,\n", header->section_map_size);
    fprintf_depth(stream, depth + 1, "file_info_size: %u,\n", header->file_info_size);
    fprintf_depth(stream, depth + 1, "type_server_map_size: %u,\n", header->type_server_map_size);
    fprintf_depth(stream, depth + 1, "mfc_type_server_index: %u,\n", header->mfc_type_server_index);
    fprintf_depth(stream, depth + 1, "debug_header_size: %u,\n", header->debug_header_size);
    fprintf_depth(stream, depth + 1, "ec_substream_size: %u,\n", header->ec_substream_size);
    fprintf_depth(stream, depth + 1, "flags: %u,\n", header->flags);
    
    fprintf_depth(stream, depth + 1, "machine_type: ");
    dbi_machine_type_print(header->machine_type, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "reserved: %u,\n", header->reserved);
    fprintf_depth(stream, depth, "}");
}

void dbi_section_contribution_print(struct dbi_section_contribution *contribution, uint32_t depth, FILE *stream)
{
    assert(contribution);
    assert(stream);

    fprintf(stream, "dbi_section_contribution {\n");
    fprintf_depth(stream, depth + 1, "section: %u,\n", contribution->section);
    fprintf_depth(stream, depth + 1, "padding1: %u,\n", contribution->padding1);
    fprintf_depth(stream, depth + 1, "offset: %u,\n", contribution->offset);
    fprintf_depth(stream, depth + 1, "size: %u,\n", contribution->size);
    fprintf_depth(stream, depth + 1, "characteristics: %u,\n", contribution->characteristics);
    fprintf_depth(stream, depth + 1, "module: %u,\n", contribution->module);
    fprintf_depth(stream, depth + 1, "padding2: %u,\n", contribution->padding2);
    fprintf_depth(stream, depth + 1, "data_crc: %u,\n", contribution->data_crc);
    fprintf_depth(stream, depth + 1, "reloc_crc: %u,\n", contribution->reloc_crc);
    fprintf_depth(stream, depth, "}");
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

void dbi_section_contributions_print(struct dbi_section_contributions *contributions, uint32_t depth, FILE *stream)
{
    assert(contributions);
    assert(stream);

    fprintf(stream, "dbi_section_contributions {\n");
    fprintf_depth(stream, depth + 1, "version: %u,\n", contributions->version);
    fprintf_depth(stream, depth + 1, "count: %u,\n", contributions->count);
    fprintf_depth(stream, depth + 1, "entries: [\n");
    for (uint32_t i = 0; i < contributions->count; i++)
    {
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        dbi_section_contribution_print(&contributions->entries[i], depth + 2, stream);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}

void dbi_module_header_print(struct dbi_module_header *header, uint32_t depth, FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "dbi_module_header {\n");
    fprintf_depth(stream, depth + 1, "opened: %u,\n", header->opened);
    
    fprintf_depth(stream, depth + 1, "section: ");
    dbi_section_contribution_print(&header->section, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "flags: %u,\n", header->flags);
    fprintf_depth(stream, depth + 1, "stream: %u,\n", header->stream);
    fprintf_depth(stream, depth + 1, "symbols_size: %u,\n", header->symbols_size);
    fprintf_depth(stream, depth + 1, "lines_size: %u,\n", header->lines_size);
    fprintf_depth(stream, depth + 1, "c13_lines_size: %u,\n", header->c13_lines_size);
    fprintf_depth(stream, depth + 1, "files: %u,\n", header->files);
    fprintf_depth(stream, depth + 1, "padding: %u,\n", header->padding);
    fprintf_depth(stream, depth + 1, "filename_offsets: %u,\n", header->filename_offsets);
    fprintf_depth(stream, depth + 1, "source: %u,\n", header->source);
    fprintf_depth(stream, depth + 1, "compiler: %u,\n", header->compiler);
    fprintf_depth(stream, depth, "}");
}

void dbi_module_dispose(struct dbi_module *module)
{
    assert(module);

    free(module->module_name);
    free(module->object_file_name);
}

void dbi_module_print(struct dbi_module *module, uint32_t depth, FILE *stream)
{
    assert(module);
    assert(stream);

    fprintf(stream, "dbi_module {\n");
    
    fprintf_depth(stream, depth + 1, "header: ");
    dbi_module_header_print(&module->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "module_name: \"%s\",\n", module->module_name);
    fprintf_depth(stream, depth + 1, "object_file_name: \"%s\",\n", module->object_file_name);
    fprintf_depth(stream, depth, "}");
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
}

void dbi_modules_dispose(struct dbi_modules *modules)
{
    assert(modules);

    free(modules->modules);
}

void dbi_modules_print(struct dbi_modules *modules, uint32_t depth, FILE *stream)
{
    assert(modules);
    assert(stream);

    fprintf(stream, "dbi_modules {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", modules->count);
    fprintf_depth(stream, depth + 1, "modules: [\n");
    for (uint32_t i = 0; i < modules->count; i++)
    {
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        dbi_module_print(&modules->modules[i], depth + 2, stream);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}

void dbi_extra_stream_index_print(enum dbi_extra_stream_index index, FILE *stream)
{
    switch (index)
    {
    case DBI_EXTRA_STREAM_FPO:
        fprintf(stream, "fpo");
        break;
    case DBI_EXTRA_STREAM_EXCEPTION:
        fprintf(stream, "exception");
        break;
    case DBI_EXTRA_STREAM_FIXUP:
        fprintf(stream, "fixup");
        break;
    case DBI_EXTRA_STREAM_OMAP_TO_SRC:
        fprintf(stream, "omap_to_src");
        break;
    case DBI_EXTRA_STREAM_OMAP_FROM_SRC:
        fprintf(stream, "omap_from_src");
        break;
    case DBI_EXTRA_STREAM_SECTION_HEADERS:
        fprintf(stream, "section_headers");
        break;
    case DBI_EXTRA_STREAM_TOKEN_RID_MAP:
        fprintf(stream, "token_rid_map");
        break;
    case DBI_EXTRA_STREAM_XDATA:
        fprintf(stream, "xdata");
        break;
    case DBI_EXTRA_STREAM_PDATA:
        fprintf(stream, "pdata");
        break;
    case DBI_EXTRA_STREAM_FRAMEDATA:
        fprintf(stream, "framedata");
        break;
    case DBI_EXTRA_STREAM_ORIGINAL_SECTION_HEADERS:
        fprintf(stream, "original_section_headers");
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: invalid DBI stream index value: %i\n", __FILE__, __LINE__, index);
        exit(EXIT_FAILURE);
    }
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

void dbi_extra_streams_print(struct dbi_extra_streams *extra_streams, uint32_t depth, FILE *stream)
{
    assert(extra_streams);
    assert(stream);

    fprintf(stream, "dbi_extra_streams {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", extra_streams->count);
    fprintf_depth(stream, depth + 1, "indices: [\n");
    for (uint32_t i = 0; i < extra_streams->count; i++)
    {
        fprintf_depth(stream, depth + 2, "");
        dbi_extra_stream_index_print(i, stdout);
        fprintf(stream, "_stream_index: ");
        if (extra_streams->indices[i] == UINT16_MAX)
            fprintf(stream, "<none>");
        else
            fprintf(stream, "%u", extra_streams->indices[i]);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}

void dbi_section_header_print(struct dbi_section_header *header, uint32_t depth, FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "dbi_section_header {\n");
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", header->name);
    fprintf_depth(stream, depth + 1, "virtual_size: %u,\n", header->virtual_size);
    fprintf_depth(stream, depth + 1, "virtual_address: %u,\n", header->virtual_address);
    fprintf_depth(stream, depth + 1, "size_of_raw_data: %u,\n", header->size_of_raw_data);
    fprintf_depth(stream, depth + 1, "pointer_to_raw_data: %u,\n", header->pointer_to_raw_data);
    fprintf_depth(stream, depth + 1, "pointer_to_relocations: %u,\n", header->pointer_to_relocations);
    fprintf_depth(stream, depth + 1, "pointer_to_line_numbers: %u,\n", header->pointer_to_line_numbers);
    fprintf_depth(stream, depth + 1, "number_of_relocations: %u,\n", header->number_of_relocations);
    fprintf_depth(stream, depth + 1, "number_of_line_numbers: %u,\n", header->number_of_line_numbers);
    fprintf_depth(stream, depth + 1, "characteristics: %u,\n", header->characteristics);
    fprintf_depth(stream, depth, "}");
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

void dbi_omap_record_print(struct dbi_omap_record *record, uint32_t depth, FILE *stream)
{
    assert(record);
    assert(stream);

    fprintf(stream, "dbi_omap_record {\n");
    fprintf_depth(stream, depth + 1, "source_address: %u,\n", record->source_address);
    fprintf_depth(stream, depth + 1, "target_address: %u,\n", record->target_address);
    fprintf_depth(stream, depth, "}");
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

void dbi_address_map_print(struct dbi_address_map *map, uint32_t depth, FILE *stream)
{
    assert(map);
    assert(stream);

    fprintf(stream, "dbi_address_map {\n");
    fprintf_depth(stream, depth + 1, "omap_to_src_record_count: %u,\n", map->omap_to_src_record_count);
    fprintf_depth(stream, depth + 1, "omap_from_src_record_count: %u,\n", map->omap_from_src_record_count);
    fprintf_depth(stream, depth + 1, "section_header_count: %u,\n", map->section_header_count);
    fprintf_depth(stream, depth + 1, "original_section_header_count: %u,\n", map->original_section_header_count);

    fprintf_depth(stream, depth + 1, "omap_to_src_records: [");
    for (uint32_t i = 0; i < map->omap_to_src_record_count; i++)
    {
        if (i == 0)
            fprintf(stream, "\n");
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        dbi_omap_record_print(&map->omap_to_src_records[i], depth + 2, stream);
        fprintf(stream, ",\n");
        if (i == map->omap_to_src_record_count - 1)
            fprintf_depth(stream, depth + 1, "");
    }
    fprintf(stream, "],\n");
    
    fprintf_depth(stream, depth + 1, "omap_from_src_records: [");
    for (uint32_t i = 0; i < map->omap_from_src_record_count; i++)
    {
        if (i == 0)
            fprintf(stream, "\n");
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        dbi_omap_record_print(&map->omap_from_src_records[i], depth + 2, stream);
        fprintf(stream, ",\n");
        if (i == map->omap_from_src_record_count - 1)
            fprintf_depth(stream, depth + 1, "");
    }
    fprintf(stream, "],\n");
    
    fprintf_depth(stream, depth + 1, "section_headers: [");
    for (uint32_t i = 0; i < map->section_header_count; i++)
    {
        if (i == 0)
            fprintf(stream, "\n");
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        dbi_section_header_print(&map->section_headers[i], depth + 2, stream);
        fprintf(stream, ",\n");
        if (i == map->section_header_count - 1)
            fprintf_depth(stream, depth + 1, "");
    }
    fprintf(stream, "],\n");

    fprintf_depth(stream, depth + 1, "original_section_headers: [");
    for (uint32_t i = 0; i < map->original_section_header_count; i++)
    {
        if (i == 0)
            fprintf(stream, "\n");
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        dbi_section_header_print(&map->original_section_headers[i], depth + 2, stream);
        fprintf(stream, ",\n");
        if (i == map->original_section_header_count - 1)
            fprintf_depth(stream, depth + 1, "");
    }
    fprintf(stream, "],\n");

    fprintf_depth(stream, depth, "}");
}
