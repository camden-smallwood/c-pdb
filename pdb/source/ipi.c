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

    free(item->argument_indices);
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

void ipi_symbols_print(struct ipi_symbols *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    IPI_SYMBOLS_STRUCT
}
