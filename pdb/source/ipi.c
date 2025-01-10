#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "ipi.h"

#include "macros_print.h"

void tpi_udt_src_line_print(struct tpi_udt_src_line *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_UDT_SRC_LINE_STRUCT
}

void tpi_udt_mod_src_line_print(struct tpi_udt_mod_src_line *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_UDT_MOD_SRC_LINE_STRUCT
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

    TPI_STRING_ID_STRUCT
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

    TPI_SUBSTR_LIST_STRUCT
}

void tpi_build_info_dispose(struct tpi_build_info *item)
{
    assert(item);

    free(item->argument_id_indices);
}

void tpi_build_info_print(struct tpi_build_info *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_BUILD_INFO_STRUCT
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

    TPI_FUNC_ID_STRUCT
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

    TPI_MFUNC_ID_STRUCT
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

void ipi_symbol_print(struct ipi_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    IPI_SYMBOL_STRUCT
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

    //
    // Collect info about where each IPI symbol is located
    //

    struct ipi_symbol_info
    {
        uint32_t offset;
        uint16_t size;
    };

    uint32_t symbol_info_count = 0;
    struct ipi_symbol_info *symbol_info = NULL;

    uint32_t offset = ipi_header->header_size;

    while (offset < msf_stream->size)
    {
        struct ipi_symbol_info info;
        
        MSF_STREAM_READ(msf, msf_stream, &offset, info.size, file_stream);
        assert(info.size >= sizeof(uint16_t));

        info.offset = offset;

        DYNARRAY_PUSH(symbol_info, symbol_info_count, info);

        offset += info.size;
    }

    //
    // Pre-allocate all of the IPI symbols
    //

    symbols->count = symbol_info_count;
    symbols->symbols = calloc(symbols->count, sizeof(*symbols->symbols));
    assert(symbols->symbols);

    //
    // Read each IPI symbol
    //

    for (uint32_t symbol_index = 0; symbol_index < symbol_info_count; symbol_index++)
    {
        offset = symbol_info[symbol_index].offset;
        
        struct ipi_symbol *symbol = &symbols->symbols[symbol_index];

        MSF_STREAM_READ(msf, msf_stream, &offset, symbol->type, file_stream);

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

            MSF_STREAM_READ(msf, msf_stream, &temp_offset, symbol->string_id.substrings_index, file_stream);
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

            MSF_STREAM_READ(msf, msf_stream, &temp_offset, symbol->build_info.argument_count, file_stream);

            symbol->build_info.argument_id_indices = malloc(symbol->build_info.argument_count * sizeof(*symbol->build_info.argument_id_indices));
            assert(symbol->build_info.argument_id_indices);

            for (uint32_t i = 0; i < symbol->build_info.argument_count; i++)
            {
                MSF_STREAM_READ(msf, msf_stream, &temp_offset, symbol->build_info.argument_id_indices[i], file_stream);
                assert(symbol->build_info.argument_id_indices[i] <= ipi_header->maximum_index);
            }
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
    }

    //
    // Cleanup
    //

    free(symbol_info);
}

void ipi_symbols_dispose(struct ipi_symbols *symbols)
{
    assert(symbols);

    for (uint32_t i = 0; i < symbols->count; i++)
        ipi_symbol_dispose(&symbols->symbols[i]);

    free(symbols->symbols);
}

void ipi_symbols_print(struct ipi_symbols *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    IPI_SYMBOLS_STRUCT
}

uint32_t ipi_index_to_absolute_index(struct tpi_header *ipi_header, struct ipi_symbols *ipi_symbols, uint32_t index)
{
    assert(ipi_header);
    assert(ipi_symbols);
    
    if (index < ipi_header->minimum_index || index >= ipi_header->maximum_index)
        return UINT32_MAX;
    
    uint32_t absolute_index = index - ipi_header->minimum_index;
    assert(absolute_index < ipi_symbols->count);

    return absolute_index;
}

struct ipi_symbol *ipi_symbol_get(struct tpi_header *ipi_header, struct ipi_symbols *ipi_symbols, uint32_t index)
{
    assert(ipi_header);
    assert(ipi_symbols);

    uint32_t absolute_index = ipi_index_to_absolute_index(ipi_header, ipi_symbols, index);

    if (absolute_index == UINT32_MAX)
        return NULL;
    
    return &ipi_symbols->symbols[absolute_index];
}

char *ipi_string_id_to_string(struct tpi_header *ipi_header, struct ipi_symbols *ipi_symbols, uint32_t index)
{
    assert(ipi_header);
    assert(ipi_symbols);

    struct ipi_symbol *argument_symbol = ipi_symbol_get(ipi_header, ipi_symbols, index);

    assert(argument_symbol);
    assert(argument_symbol->type == LF_STRING_ID);

    char *string = calloc(1, sizeof(char));
    assert(string);

    struct ipi_symbol *substrings_symbol = ipi_symbol_get(ipi_header, ipi_symbols, argument_symbol->string_id.substrings_index);
    
    if (substrings_symbol)
    {
        assert(substrings_symbol->type = LF_SUBSTR_LIST);

        for (uint32_t i = 0; i < substrings_symbol->substr_list.count; i++)
        {
            char *substring = ipi_string_id_to_string(ipi_header, ipi_symbols, substrings_symbol->substr_list.substring_indices[i]);
            string_append(&string, substring);
        }
    }

    string_append(&string, argument_symbol->string_id.string);

    return string;
}
