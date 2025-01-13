#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "ipi.h"
#include "utils.h"
#include "tpi.h"
#include "cv.h"

#include "macros_print.h"

void msf_header_v2_print(struct msf_header_v2 *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    MSF_HEADER_V2_STRUCT
}

int msf_header_v2_read(struct msf_header_v2 *v2, FILE *stream)
{
    assert(v2);
    assert(stream);

    fseek(stream, 0, SEEK_END);
    long size = ftell(stream);
    
    if ((unsigned long)size < sizeof(struct msf_header_v2))
        return 0;

    fseek(stream, 0, SEEK_SET);
    fread(v2->signature, sizeof(v2->signature), 1, stream);

    if (memcmp(v2->signature, msf_header_v2_signature, sizeof(msf_header_v2_signature)) != 0)
        return 0;
    
    fread(&v2->page_size, sizeof(*v2) - sizeof(v2->signature), 1, stream);

    return 1;
}

void msf_header_v7_print(struct msf_header_v7 *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);
    
    MSF_HEADER_V7_STRUCT
}

int msf_header_v7_read(struct msf_header_v7 *v7, FILE *stream)
{   
    assert(v7);
    assert(stream);

    fseek(stream, 0, SEEK_END);
    long size = ftell(stream);
    
    if ((unsigned long)size < sizeof(struct msf_header_v7))
        return 0;

    fseek(stream, 0, SEEK_SET);
    fread(v7->signature, sizeof(v7->signature), 1, stream);

    if (memcmp(v7->signature, msf_header_v7_signature, sizeof(msf_header_v7_signature)) != 0)
        return 0;
    
    fread(&v7->page_size, sizeof(*v7) - sizeof(v7->signature), 1, stream);

    return 1;
}

void msf_header_type_print(enum msf_header_type item, FILE *stream)
{
    assert(stream);

    MSF_HEADER_TYPE_ENUM
}

void msf_header_print(struct msf_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    MSF_HEADER_STRUCT
}

int msf_header_read(struct msf_header *header, FILE *stream)
{
    header->type = MSF_HEADER_UNKNOWN;
    
    if (msf_header_v7_read(&header->v7, stream))
    {
        header->type = MSF_HEADER_V7;
        return 1;
    }
    
    if (msf_header_v2_read(&header->v2, stream))
    {
        header->type = MSF_HEADER_V2;
        return 1;
    }
    
    return 0;
}

void msf_stream_dispose(struct msf_stream *msf_stream)
{
    assert(msf_stream);

    free(msf_stream->page_indices);
}

void msf_stream_print(struct msf_stream *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    MSF_STREAM_STRUCT
}

void msf_dispose(struct msf *msf)
{
    assert(msf);

    msf_stream_dispose(&msf->root_stream);

    for (uint32_t i = 0; i < msf->stream_count; i++)
        msf_stream_dispose(&msf->streams[i]);
    
    free(msf->streams);
}

void msf_print(struct msf *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    MSF_STRUCT
}

void msf_read(struct msf *msf, FILE *stream)
{
    assert(msf);
    assert(stream);

    memset(msf, 0, sizeof(*msf));

    if (!msf_header_read(&msf->header, stream))
    {
        fprintf(stderr, "ERROR: Failed to read MSF header\n");
        exit(EXIT_FAILURE);
    }
    
    msf_stream_initialize(msf, &msf->root_stream, msf_get_root_stream_size(msf));

    switch (msf->header.type)
    {
    case MSF_HEADER_V2:
        {
            uint16_t *page_indices_v2 = malloc(sizeof(uint16_t) * msf->root_stream.page_count);
            assert(page_indices_v2);

            fread(page_indices_v2, sizeof(uint16_t), msf->root_stream.page_count, stream);

            for (uint32_t i = 0; i < msf->root_stream.page_count; i++)
                msf->root_stream.page_indices[i] = (uint32_t)page_indices_v2[i];
            
            free(page_indices_v2);
            break;
        }
    
    case MSF_HEADER_V7:
        {
            uint32_t page_list_offset = msf_get_page_offset(msf, msf->header.v7.root_stream_page_list_page_index);
            fseek(stream, page_list_offset, SEEK_SET);
            fread(msf->root_stream.page_indices, sizeof(uint32_t), msf->root_stream.page_count, stream);
            break;
        }
    
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled msf header type: %i\n", __FILE__, __LINE__, msf->header.type);
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < msf->root_stream.page_count; i++)
        msf_verify_page_index(msf, msf->root_stream.page_indices[i]);
    
    char *root_stream_data = malloc(msf->root_stream.size);
    assert(root_stream_data);

    msf_stream_read_data(msf, &msf->root_stream, 0, msf->root_stream.size, root_stream_data, stream);

    switch (msf->header.type)
    {
    case MSF_HEADER_V2:
        {
            msf->stream_count = *(uint16_t *)(root_stream_data + 0);
            
            // uint16_t _reserved = *(uint16_t *)(root_stream_data + 2);
            
            msf->streams = malloc(msf->stream_count * sizeof(*msf->streams));
            assert(msf->streams);

            uint32_t offset = 4;

            for (uint32_t i = 0; i < msf->stream_count; i++)
            {
                uint32_t stream_size = *(uint32_t *)(root_stream_data + offset);
                msf_stream_initialize(msf, &msf->streams[i], stream_size);
                offset += sizeof(uint32_t);

                // uint32_t _reserved2 = *(uint32_t *)(root_stream_data + offset);
                offset += sizeof(uint32_t);
            }

            for (uint32_t i = 0; i < msf->stream_count; i++)
            {
                for (uint32_t j = 0; j < msf->streams[i].page_count; j++)
                {
                    msf->streams[i].page_indices[j] = *(uint16_t *)(root_stream_data + offset);
                    offset += sizeof(uint16_t);
                }
            }

            break;
        }

    case MSF_HEADER_V7:
        {
            msf->stream_count = *(uint32_t *)(root_stream_data + 0);
            
            msf->streams = malloc(msf->stream_count * sizeof(*msf->streams));
            assert(msf->streams);

            uint32_t offset = 4;

            for (uint32_t i = 0; i < msf->stream_count; i++)
            {
                uint32_t stream_size = *(uint32_t *)(root_stream_data + offset);
                msf_stream_initialize(msf, &msf->streams[i], stream_size);
                offset += sizeof(uint32_t);
            }

            for (uint32_t i = 0; i < msf->stream_count; i++)
            {
                for (uint32_t j = 0; j < msf->streams[i].page_count; j++)
                {
                    msf->streams[i].page_indices[j] = *(uint32_t *)(root_stream_data + offset);
                    offset += sizeof(uint32_t);
                }
            }

            break;
        }
        
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled msf header type: %i\n", __FILE__, __LINE__, msf->header.type);
        exit(EXIT_FAILURE);
    }

    free(root_stream_data);
}

uint32_t msf_get_page_count(struct msf *msf)
{
    assert(msf);

    switch (msf->header.type)
    {
    case MSF_HEADER_V2:
        return msf->header.v2.file_page_count;
    
    case MSF_HEADER_V7:
        return msf->header.v7.file_page_count;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled msf header type: %i\n", __FILE__, __LINE__, msf->header.type);
        exit(EXIT_FAILURE);
    }
}

void msf_verify_page_index(struct msf *msf, uint32_t page_index)
{
    assert(msf);
    assert(page_index != 0);
    assert(page_index < msf_get_page_count(msf));
}

uint32_t msf_get_page_size(struct msf *msf)
{
    assert(msf);

    switch (msf->header.type)
    {
    case MSF_HEADER_V2:
        return msf->header.v2.page_size;
    
    case MSF_HEADER_V7:
        return msf->header.v7.page_size;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled msf header type: %i\n", __FILE__, __LINE__, msf->header.type);
        exit(EXIT_FAILURE);
    }
}

uint32_t msf_get_page_offset(struct msf *msf, uint32_t page_index)
{
    msf_verify_page_index(msf, page_index);
    return page_index * msf_get_page_size(msf);
}

uint32_t msf_get_page_count_for_size(struct msf *msf, uint32_t size)
{
    uint32_t page_size = msf_get_page_size(msf);
    return (size + (page_size - 1)) / page_size;
}

uint32_t msf_get_root_stream_size(struct msf *msf)
{
    assert(msf);

    switch (msf->header.type)
    {
    case MSF_HEADER_V2:
        return msf->header.v2.root_stream_size;
    
    case MSF_HEADER_V7:
        return msf->header.v7.root_stream_size;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled msf header type: %i\n", __FILE__, __LINE__, msf->header.type);
        exit(EXIT_FAILURE);
    }
}

void msf_stream_initialize(struct msf *msf, struct msf_stream *stream, uint32_t size)
{
    stream->size = size;
    stream->page_count = msf_get_page_count_for_size(msf, size);

    if (stream->page_count)
    {
        stream->page_indices = malloc(sizeof(uint32_t) * stream->page_count);
        assert(stream->page_indices);
    }
}

void msf_stream_read_data(
    struct msf *msf,
    struct msf_stream *stream,
    uint32_t offset,
    uint32_t size,
    void *destination,
    FILE *file_stream)
{
    assert(msf);
    assert(stream);
    assert(destination);
    assert(file_stream);
    assert(offset + size <= stream->size);

    uint32_t page_size = msf_get_page_size(msf);
    uint32_t start_page_index = offset / page_size;
    uint32_t inset_offset = offset % page_size;
    
    uint32_t write_offset = 0;
    uint32_t data_remaining = size;

    for (uint32_t i = 0; data_remaining; i++)
    {
        uint32_t read_offset = msf_get_page_offset(msf, stream->page_indices[start_page_index + i]);
        uint32_t read_size = data_remaining > (page_size - inset_offset) ? (page_size - inset_offset) : data_remaining;

        if (inset_offset)
        {
            read_offset += inset_offset;
            inset_offset = 0;
        }
        
        fseek(file_stream, read_offset, SEEK_SET);
        fread((char *)destination + write_offset, read_size, 1, file_stream);

        write_offset += read_size;
        data_remaining -= read_size;
    }
}

char *msf_stream_read_cstring(
    struct msf *msf,
    struct msf_stream *stream,
    uint32_t read_offset,
    uint32_t *out_length,
    FILE *file_stream)
{
    uint32_t length = 0;
    char *string = NULL;

    char c;
    
    do
    {
        msf_stream_read_data(
            msf,
            stream,
            read_offset,
            sizeof(char),
            &c,
            file_stream);

        read_offset += sizeof(char);
        
        string = realloc(string, ++length * sizeof(char));
        assert(string);

        string[length - 1] = c;
    }
    while (c != '\0');

    if (out_length)
        *out_length = length;
    
    return string;
}

char *msf_stream_read_u8_pascal_string(
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t offset,
    uint32_t *out_length,
    FILE *file_stream)
{
    uint8_t length;
    msf_stream_read_data(msf, msf_stream, offset, sizeof(length), &length, file_stream);
    offset += sizeof(length);

    char *string = malloc(length + 1);
    assert(string);
    msf_stream_read_data(msf, msf_stream, offset, length, string, file_stream);
    string[length - 1] = '\0';

    if (out_length)
        *out_length = length;
    
    return string;
}

uint32_t msf_stream_read_compressed_unsigned(
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint8_t b1;
    MSF_STREAM_READ(msf, msf_stream, out_offset, b1, file_stream);
    
    if ((b1 & 0x80) == 0x00)
        return (uint32_t)b1;

    uint8_t b2;
    MSF_STREAM_READ(msf, msf_stream, out_offset, b2, file_stream);
    
    if ((b1 & 0xc0) == 0x80)
        return (((uint32_t)b1 & 0x3f) << 8) | (uint32_t)b2;

    uint8_t b3, b4;
    MSF_STREAM_READ(msf, msf_stream, out_offset, b3, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, b4, file_stream);
    
    if ((b1 & 0xe0) == 0xc0)
        return (((uint32_t)b1 & 0x1f) << 24) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 8) | (uint32_t)b4;

    fprintf(stderr, "%s:%i: ERROR: invalid compressed annotation: %02x%02x%02x%02x\n", __FILE__, __LINE__, b1, b2, b3, b4);
    exit(EXIT_FAILURE);
}

uint64_t msf_read_tpi_unsigned(
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint16_t size_leaf = 0;
    msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(size_leaf), &size_leaf, file_stream);
    *out_offset += sizeof(size_leaf);

    if (size_leaf < LF_NUMERIC)
        return (uint64_t)size_leaf;

    switch (size_leaf)
    {
    case LF_CHAR:
        {
            uint8_t value;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(value), &value, file_stream);
            *out_offset += sizeof(value);
            
            return (uint64_t)value;
        }
        
    case LF_USHORT:
        {
            uint16_t value;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(value), &value, file_stream);
            *out_offset += sizeof(value);
            
            return (uint64_t)value;
        }
        
    case LF_ULONG:
        {
            uint32_t value;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(value), &value, file_stream);
            *out_offset += sizeof(value);
            
            return (uint64_t)value;
        }
        
    case LF_UQUADWORD:
        {
            uint64_t value;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(value), &value, file_stream);
            *out_offset += sizeof(value);
            
            return value;
        }
        
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled class size leaf type: ", __FILE__, __LINE__);
        tpi_leaf_print(size_leaf, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
}

char *msf_read_tpi_lf_string(
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint16_t leaf,
    FILE *file_stream)
{
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);
    
    uint32_t length;
    char *result;

    if (leaf < LF_ST_MAX)
    {
        result = msf_stream_read_u8_pascal_string(msf, msf_stream, *out_offset, &length, file_stream);
        *out_offset += length + 1;
    }
    else
    {
        result = msf_stream_read_cstring(msf, msf_stream, *out_offset, &length, file_stream);
        *out_offset += length;
    }

    return result;
}

char *msf_read_cv_symbol_string(
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint16_t symbol_type,
    FILE *file_stream)
{
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t length;
    char *result;

    if (symbol_type < S_ST_MAX)
    {
        result = msf_stream_read_u8_pascal_string(msf, msf_stream, *out_offset, &length, file_stream);
        *out_offset += length + 1;
    }
    else
    {
        result = msf_stream_read_cstring(msf, msf_stream, *out_offset, &length, file_stream);
        *out_offset += length;
    }

    return result;
}
