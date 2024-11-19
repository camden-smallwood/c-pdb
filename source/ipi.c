#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "ipi.h"

void tpi_udt_src_line_print(struct tpi_udt_src_line *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_udt_src_line {\n");
    fprintf_depth(stream, depth + 1, "udt_index: %u,\n", item->udt_index);
    fprintf_depth(stream, depth + 1, "file_id: %u,\n", item->file_id);
    fprintf_depth(stream, depth + 1, "line: %u,\n", item->line);
    fprintf_depth(stream, depth, "}");
}

void tpi_udt_mod_src_line_print(struct tpi_udt_mod_src_line *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_udt_mod_src_line {\n");
    fprintf_depth(stream, depth + 1, "udt_index: %u,\n", item->udt_index);
    fprintf_depth(stream, depth + 1, "file_id: %u,\n", item->file_id);
    fprintf_depth(stream, depth + 1, "line: %u,\n", item->line);
    fprintf_depth(stream, depth + 1, "mod: %u,\n", item->mod);
    fprintf_depth(stream, depth, "}");
}

void tpi_string_id_dispose(struct tpi_string_id *item)
{
    assert(item);

    free(item->string);
}

void tpi_string_id_print(struct tpi_string_id *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_string_id {\n");
    fprintf_depth(stream, depth + 1, "substrings_index: %u,\n", item->substrings_index);
    fprintf_depth(stream, depth + 1, "string: \"%s\",\n", item->string);
    fprintf_depth(stream, depth, "}");
}

void tpi_substr_list_dispose(struct tpi_substr_list *item)
{
    assert(item);

    free(item->substring_indices);
}

void tpi_substr_list_print(struct tpi_substr_list *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_substr_list {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", item->count);
    fprintf_depth(stream, depth + 1, "substring_indices: [");
    for (uint32_t i = 0; i < item->count; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");
        fprintf(stream, "%u", item->substring_indices[i]);
    }
    fprintf(stream, "],\n");
    fprintf_depth(stream, depth, "}");
}

void tpi_build_info_dispose(struct tpi_build_info *item)
{
    assert(item);

    free(item->argument_indices);
}

void tpi_build_info_print(struct tpi_build_info *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_build_info {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", item->count);
    fprintf_depth(stream, depth + 1, "argument_indices: [");
    for (uint32_t i = 0; i < item->count; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");
        fprintf(stream, "%u", item->argument_indices[i]);
    }
    fprintf(stream, "],\n");
    fprintf_depth(stream, depth, "}");
}

void tpi_func_id_dispose(struct tpi_func_id *item)
{
    assert(item);

    free(item->name);
}

void tpi_func_id_print(struct tpi_func_id *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_func_id {\n");
    fprintf_depth(stream, depth + 1, "scope_index: %u,\n", item->scope_index);
    fprintf_depth(stream, depth + 1, "function_type_index: %u,\n", item->function_type_index);
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_mfunc_id_dispose(struct tpi_mfunc_id *item)
{
    assert(item);

    free(item->name);
}

void tpi_mfunc_id_print(struct tpi_mfunc_id *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_mfunc_id {\n");
    fprintf_depth(stream, depth + 1, "parent: %u,\n", item->parent);
    fprintf_depth(stream, depth + 1, "function_type_index: %u,\n", item->function_type_index);
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void ipi_symbol_dispose(struct ipi_symbol *symbol)
{
    assert(symbol);

    switch (symbol->type)
    {
    case LF_UDT_SRC_LINE:
        break;
    
    case LF_UDT_MOD_SRC_LINE:
        break;
    
    case LF_STRING_ID:
        tpi_string_id_dispose(&symbol->string_id);
        break;
    
    case LF_SUBSTR_LIST:
        tpi_substr_list_dispose(&symbol->substr_list);
        break;

    case LF_BUILDINFO:
        tpi_build_info_dispose(&symbol->build_info);
        break;

    case LF_FUNC_ID:
        tpi_func_id_dispose(&symbol->func_id);
        break;

    case LF_MFUNC_ID:
        tpi_mfunc_id_dispose(&symbol->mfunc_id);
        break;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled IPI symbol type: %u\n", __FILE__, __LINE__, symbol->type);
        exit(EXIT_FAILURE);
    }
}

void ipi_symbol_print(struct ipi_symbol *symbol, uint32_t depth, FILE *stream)
{
    assert(symbol);
    assert(stream);

    fprintf(stream, "ipi_symbol {\n");
    
    fprintf_depth(stream, depth + 1, "type: ");
    tpi_leaf_print(symbol->type, stream);
    fprintf(stream, ",\n");

    switch (symbol->type)
    {
    case LF_UDT_SRC_LINE:
        fprintf_depth(stream, depth + 1, "udt_src_line: ");
        tpi_udt_src_line_print(&symbol->udt_src_line, depth + 1, stream);
        fprintf(stream, ",\n");
        break;
    
    case LF_UDT_MOD_SRC_LINE:
        fprintf_depth(stream, depth + 1, "udt_mod_src_line: ");
        tpi_udt_mod_src_line_print(&symbol->udt_mod_src_line, depth + 1, stream);
        fprintf(stream, ",\n");
        break;
    
    case LF_STRING_ID:
        fprintf_depth(stream, depth + 1, "string_id: ");
        tpi_string_id_print(&symbol->string_id, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_SUBSTR_LIST:
        fprintf_depth(stream, depth + 1, "substr_list: ");
        tpi_substr_list_print(&symbol->substr_list, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_BUILDINFO:
        fprintf_depth(stream, depth + 1, "build_info: ");
        tpi_build_info_print(&symbol->build_info, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_FUNC_ID:
        fprintf_depth(stream, depth + 1, "func_id: ");
        tpi_func_id_print(&symbol->func_id, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_MFUNC_ID:
        fprintf_depth(stream, depth + 1, "mfunc_id: ");
        tpi_mfunc_id_print(&symbol->mfunc_id, depth + 1, stream);
        fprintf(stream, ",\n");
        break;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled IPI symbol type: ", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    fprintf_depth(stream, depth, "}");
}

void ipi_header_read(struct tpi_header *header, struct msf *msf, FILE *stream)
{
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_IPI], 0, sizeof(*header), header, stream);
}

void ipi_symbols_read(
    struct ipi_symbols *symbols,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct tpi_header *ipi_header,
    FILE *file_stream)
{
    assert(symbols);
    assert(msf);
    assert(msf_stream);
    assert(ipi_header);
    assert(file_stream);

    uint32_t offset = ipi_header->header_size;

    while (offset < msf_stream->size)
    {
        symbols->count++;
        symbols->symbols = realloc(symbols->symbols, symbols->count * sizeof(*symbols->symbols));
        assert(symbols->symbols);

        struct ipi_symbol *symbol = &symbols->symbols[symbols->count - 1];
        memset(symbol, 0, sizeof(*symbol));

        uint16_t size;
        msf_stream_read_data(msf, msf_stream, offset, sizeof(size), &size, file_stream);
        offset += sizeof(size);
        assert(size >= sizeof(uint16_t));

        msf_stream_read_data(msf, msf_stream, offset, sizeof(symbol->type), &symbol->type, file_stream);
        offset += sizeof(symbol->type);
        size -= sizeof(symbol->type);

        switch (symbol->type)
        {
        case LF_UDT_SRC_LINE:
            msf_stream_read_data(msf, msf_stream, offset, sizeof(symbol->udt_src_line), &symbol->udt_src_line, file_stream);
            break;
        
        case LF_UDT_MOD_SRC_LINE:
            msf_stream_read_data(msf, msf_stream, offset, sizeof(symbol->udt_mod_src_line), &symbol->udt_mod_src_line, file_stream);
            break;
        
        case LF_STRING_ID:
            {
                uint32_t temp_offset = offset;
                
                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(symbol->string_id.substrings_index), &symbol->string_id.substrings_index, file_stream);
                temp_offset += sizeof(symbol->string_id.substrings_index);

                symbol->string_id.string = msf_read_tpi_lf_string(msf, msf_stream, &temp_offset, symbol->type, file_stream);
                break;
            }
        
        case LF_SUBSTR_LIST:
            {
                uint32_t temp_offset = offset;
                
                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(symbol->substr_list.count), &symbol->substr_list.count, file_stream);
                temp_offset += sizeof(symbol->substr_list.count);

                symbol->substr_list.substring_indices = malloc(symbol->substr_list.count * sizeof(*symbol->substr_list.substring_indices));
                assert(symbol->substr_list.substring_indices);
                msf_stream_read_data(msf, msf_stream, temp_offset, symbol->substr_list.count * sizeof(*symbol->substr_list.substring_indices), symbol->substr_list.substring_indices, file_stream);
                break;
            }
        
        case LF_BUILDINFO:
            {
                uint32_t temp_offset = offset;
                
                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(symbol->build_info.count), &symbol->build_info.count, file_stream);
                temp_offset += sizeof(symbol->build_info.count);

                symbol->build_info.argument_indices = malloc(symbol->build_info.count * sizeof(*symbol->build_info.argument_indices));
                assert(symbol->build_info.argument_indices);
                msf_stream_read_data(msf, msf_stream, temp_offset, symbol->build_info.count * sizeof(*symbol->build_info.argument_indices), symbol->build_info.argument_indices, file_stream);
                break;
            }
        
        case LF_FUNC_ID:
            {
                uint32_t temp_offset = offset;
                
                struct tpi_func_id item;
                memset(&item, 0, sizeof(item));

                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(item.scope_index), &item.scope_index, file_stream);
                temp_offset += sizeof(item.scope_index);

                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(item.function_type_index), &item.function_type_index, file_stream);
                temp_offset += sizeof(item.function_type_index);

                item.name = msf_read_tpi_lf_string(msf, msf_stream, &temp_offset, symbol->type, file_stream);
                break;
            }
        
        case LF_MFUNC_ID:
            {
                uint32_t temp_offset = offset;
                
                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(symbol->mfunc_id.parent), &symbol->mfunc_id.parent, file_stream);
                temp_offset += sizeof(symbol->mfunc_id.parent);

                msf_stream_read_data(msf, msf_stream, temp_offset, sizeof(symbol->mfunc_id.function_type_index), &symbol->mfunc_id.function_type_index, file_stream);
                temp_offset += sizeof(symbol->mfunc_id.function_type_index);

                symbol->mfunc_id.name = msf_read_tpi_lf_string(msf, msf_stream, &temp_offset, symbol->type, file_stream);
                break;
            }

        default:
            fprintf(stderr, "%s:%i: ERROR: Unsupported IPI symbol leaf: ", __FILE__, __LINE__);
            tpi_leaf_print(symbol->type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        offset += size;
    }
}

void ipi_symbols_dispose(struct ipi_symbols *symbols)
{
    assert(symbols);

    for (uint32_t i = 0; i < symbols->count; i++)
        ipi_symbol_dispose(&symbols->symbols[i]);
    
    free(symbols->symbols);
}

void ipi_symbols_print(struct ipi_symbols *symbols, uint32_t depth, FILE *stream)
{
    assert(symbols);
    assert(stream);

    fprintf(stream, "[\n");

    for (uint32_t i = 0; i < symbols->count; i++)
    {
        fprintf_depth(stream, depth + 1, "");
        ipi_symbol_print(&symbols->symbols[i], depth + 1, stream);
        fprintf(stream, ",\n");
    }

    fprintf_depth(stream, depth, "]");
}
