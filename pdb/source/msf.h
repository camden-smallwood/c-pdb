#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static const char msf_header_v2_signature[] = "Microsoft C/C++ program database 2.00\r\n\032JG\0";
static_assert(sizeof(msf_header_v2_signature) == 44);

struct msf_header_v2
{
    char signature[sizeof(msf_header_v2_signature)];
    uint32_t page_size;
    uint16_t start_page;
    uint16_t file_page_count;
    uint32_t root_stream_size;
    uint32_t reserved;
};
static_assert(sizeof(struct msf_header_v2) == 60);

static const char msf_header_v7_signature[] = "Microsoft C/C++ MSF 7.00\r\n\032DS\0\0";
static_assert(sizeof(msf_header_v7_signature) == 32);

struct msf_header_v7
{
    char signature[sizeof(msf_header_v7_signature)];
    uint32_t page_size;
    uint32_t allocation_table_pointer;
    uint32_t file_page_count;
    uint32_t root_stream_size;
    uint32_t reserved;
    uint32_t root_stream_page_list_page_index;
};
static_assert(sizeof(struct msf_header_v7) == 56);

enum msf_header_type
{
    MSF_HEADER_UNKNOWN,
    MSF_HEADER_V2,
    MSF_HEADER_V7,
};

struct msf_header
{
    enum msf_header_type type;
    union
    {
        struct msf_header_v2 v2;
        struct msf_header_v7 v7;
    };
};

int msf_header_read(struct msf_header *header, FILE *stream);
void msf_header_print(struct msf_header *header, uint32_t depth, FILE *stream);

enum msf_stream_index
{
    MSF_STREAM_DIR,
    MSF_STREAM_PDB,
    MSF_STREAM_TPI,
    MSF_STREAM_DBI,
    MSF_STREAM_IPI,
};

struct msf_stream
{
    uint32_t size;
    uint32_t page_count;
    uint32_t *page_indices;
};

void msf_stream_dispose(struct msf_stream *msf_stream);
void msf_stream_print(struct msf_stream *msf_stream, uint32_t depth, FILE *file_stream);

struct msf
{
    struct msf_header header;

    struct msf_stream root_stream;

    uint32_t stream_count;
    struct msf_stream *streams;
};

void msf_dispose(struct msf *msf);
void msf_print(struct msf *msf, uint32_t depth, FILE *stream);

uint32_t msf_get_page_count(struct msf *msf);
void msf_verify_page_index(struct msf *msf, uint32_t page_index);

uint32_t msf_get_page_size(struct msf *msf);
uint32_t msf_get_page_offset(struct msf *msf, uint32_t page_index);
uint32_t msf_get_page_index_for_offset(struct msf *msf, uint32_t offset);
uint32_t msf_get_page_count_for_size(struct msf *msf, uint32_t size);

uint32_t msf_get_root_stream_size(struct msf *msf);

void msf_stream_initialize(struct msf *msf, struct msf_stream *stream, uint32_t size);
void msf_stream_dispose(struct msf_stream *stream);

#define MSF_STREAM_READ(msf, msf_stream, out_offset, destination, file_stream)                                        \
    do                                                                                                                \
    {                                                                                                                 \
        msf_stream_read_data((msf), (msf_stream), *(out_offset), sizeof(destination), &(destination), (file_stream)); \
        *(out_offset) += sizeof(destination);                                                                         \
    } while (0)

void msf_stream_read_data(struct msf *msf, struct msf_stream *stream, uint32_t offset, uint32_t size, void *destination, FILE *file_stream);
char *msf_stream_read_cstring(struct msf *msf, struct msf_stream *stream, uint32_t offset, uint32_t *out_length, FILE *file_stream);
char *msf_stream_read_u8_pascal_string(struct msf *msf, struct msf_stream *msf_stream, uint32_t offset, uint32_t *out_length, FILE *file_stream);
uint64_t msf_read_tpi_lf_unsigned(struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
char *msf_read_tpi_lf_string(struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, uint16_t leaf, FILE *file_stream);
void msf_read_padding(struct msf *msf, struct msf_stream *msf_stream, uint32_t end_offset, uint32_t *out_offset, FILE *file_stream);

void msf_read(struct msf *msf, FILE *stream);
