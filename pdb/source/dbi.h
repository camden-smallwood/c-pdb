#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"

enum dbi_machine_type
{
    /// The contents of this field are assumed to be applicable to any machine type.
    DBI_MACHINE_TYPE_UNKNOWN = 0x0,
    /// Matsushita AM33
    DBI_MACHINE_TYPE_AM33 = 0x13,
    /// x64
    DBI_MACHINE_TYPE_AMD64 = 0x8664,
    /// ARM little endian
    DBI_MACHINE_TYPE_ARM = 0x1C0,
    /// ARM64 little endian
    DBI_MACHINE_TYPE_ARM64 = 0xAA64,
    /// ARM Thumb-2 little endian
    DBI_MACHINE_TYPE_ARM_NT = 0x1C4,
    /// EFI byte code
    DBI_MACHINE_TYPE_EBC = 0xEBC,
    /// Intel 386 or later processors and compatible processors
    DBI_MACHINE_TYPE_X86 = 0x14C,
    /// Intel Itanium processor family
    DBI_MACHINE_TYPE_IA64 = 0x200,
    /// Mitsubishi M32R little endian
    DBI_MACHINE_TYPE_M32R = 0x9041,
    /// MIPS16
    DBI_MACHINE_TYPE_MIPS16 = 0x266,
    /// MIPS with FPU
    DBI_MACHINE_TYPE_MIPS_FPU = 0x366,
    /// MIPS16 with FPU
    DBI_MACHINE_TYPE_MIPS_FPU16 = 0x466,
    /// Power PC little endian
    DBI_MACHINE_TYPE_POWER_PC = 0x1F0,
    /// Power PC with floating point support
    DBI_MACHINE_TYPE_POWER_PC_FP = 0x1F1,
    /// Power PC Altivec (TODO: is this correct?)
    DBI_MACHINE_TYPE_POWER_PC_ALTIVEC = 0x1F2,
    /// MIPS little endian
    DBI_MACHINE_TYPE_R4000 = 0x166,
    /// RISC-V 32-bit address space
    DBI_MACHINE_TYPE_RISC_V32 = 0x5032,
    /// RISC-V 64-bit address space
    DBI_MACHINE_TYPE_RISC_V64 = 0x5064,
    /// RISC-V 128-bit address space
    DBI_MACHINE_TYPE_RISC_V128 = 0x5128,
    /// Hitachi SH3
    DBI_MACHINE_TYPE_SH3 = 0x1A2,
    /// Hitachi SH3 DSP
    DBI_MACHINE_TYPE_SH3_DSP = 0x1A3,
    /// Hitachi SH4
    DBI_MACHINE_TYPE_SH4 = 0x1A6,
    /// Hitachi SH5
    DBI_MACHINE_TYPE_SH5 = 0x1A8,
    /// Thumb
    DBI_MACHINE_TYPE_THUMB = 0x1C2,
    /// MIPS little-endian WCE v2
    DBI_MACHINE_TYPE_WCE_MIPS_V2 = 0x169,
    /// Invalid value
    DBI_MACHINE_TYPE_INVALID = 0xffff,
};

void dbi_machine_type_print(enum dbi_machine_type type, FILE *stream);

struct dbi_header
{
    uint32_t signature;
    uint32_t version;
    uint32_t age;
    uint16_t gs_symbols_stream;
    uint16_t internal_version;
    uint16_t ps_symbols_stream;
    uint16_t pdb_dll_build_version;
    uint16_t symbol_records_stream;
    uint16_t pdb_dll_rbld_version;
    uint32_t module_list_size;
    uint32_t section_contribution_size;
    uint32_t section_map_size;
    uint32_t file_info_size;
    uint32_t type_server_map_size;
    uint32_t mfc_type_server_index;
    uint32_t debug_header_size;
    uint32_t ec_substream_size;
    uint16_t flags;
    uint16_t machine_type;
    uint32_t reserved;
};
static_assert(sizeof(struct dbi_header) == 64);

void dbi_header_read(struct dbi_header *header, struct msf *msf, FILE *stream);
void dbi_header_print(struct dbi_header *header, uint32_t depth, FILE *stream);

struct dbi_section_contribution
{
    uint16_t section;
    uint16_t padding1;
    uint32_t offset;
    uint32_t size;
    uint32_t characteristics;
    uint16_t module;
    uint16_t padding2;
    uint32_t data_crc;
    uint32_t reloc_crc;
};
static_assert(sizeof(struct dbi_section_contribution) == 28);

void dbi_section_contribution_print(struct dbi_section_contribution *contribution, uint32_t depth, FILE *stream);

struct dbi_section_contributions
{
    uint32_t version;
    uint32_t count;
    struct dbi_section_contribution *entries;
};

void dbi_section_contributions_read(struct dbi_section_contributions *contributions, struct msf *msf, struct dbi_header *dbi_header, FILE *stream);
void dbi_section_contributions_dispose(struct dbi_section_contributions *contributions);
void dbi_section_contributions_print(struct dbi_section_contributions *contributions, uint32_t depth, FILE *stream);

struct dbi_module_header
{
    uint32_t opened;
    struct dbi_section_contribution section;
    uint16_t flags;
    uint16_t stream;
    uint32_t symbols_size;
    uint32_t lines_size;
    uint32_t c13_lines_size;
    uint16_t files;
    uint16_t padding;
    uint32_t filename_offsets;
    uint32_t source;
    uint32_t compiler;
};
static_assert(sizeof(struct dbi_module_header) == 64);

void dbi_module_header_print(struct dbi_module_header *header, uint32_t depth, FILE *stream);

struct dbi_module
{
    struct dbi_module_header header;
    char *module_name;
    char *object_file_name;
};

void dbi_module_dispose(struct dbi_module *module);
void dbi_module_print(struct dbi_module *module, uint32_t depth, FILE *stream);

struct dbi_modules
{
    uint32_t count;
    struct dbi_module *modules;
};

void dbi_modules_read(struct dbi_modules *modules, struct msf *msf, struct dbi_header *dbi_header, FILE *stream);
void dbi_modules_dispose(struct dbi_modules *modules);
void dbi_modules_print(struct dbi_modules *modules, uint32_t depth, FILE *stream);

enum dbi_extra_stream_index
{

    DBI_EXTRA_STREAM_FPO,
    DBI_EXTRA_STREAM_EXCEPTION,
    DBI_EXTRA_STREAM_FIXUP,
    DBI_EXTRA_STREAM_OMAP_TO_SRC,
    DBI_EXTRA_STREAM_OMAP_FROM_SRC,
    DBI_EXTRA_STREAM_SECTION_HEADERS,
    DBI_EXTRA_STREAM_TOKEN_RID_MAP,
    DBI_EXTRA_STREAM_XDATA,
    DBI_EXTRA_STREAM_PDATA,
    DBI_EXTRA_STREAM_FRAMEDATA,
    DBI_EXTRA_STREAM_ORIGINAL_SECTION_HEADERS,
};

void dbi_extra_stream_index_print(enum dbi_extra_stream_index index, FILE *stream);

struct dbi_extra_streams
{
    uint32_t count;
    uint16_t *indices;
};

void dbi_extra_streams_read(struct dbi_extra_streams *extra_streams, struct msf *msf, struct dbi_header *dbi_header, FILE *stream);
void dbi_extra_streams_dispose(struct dbi_extra_streams *extra_streams);
void dbi_extra_streams_print(struct dbi_extra_streams *extra_streams, uint32_t depth, FILE *stream);

struct dbi_section_header
{
    char name[8];
    uint32_t virtual_size;
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_line_numbers;
    uint16_t number_of_relocations;
    uint16_t number_of_line_numbers;
    uint32_t characteristics;
};
static_assert(sizeof(struct dbi_section_header) == 40);

void dbi_section_header_print(struct dbi_section_header *header, uint32_t depth, FILE *stream);
void dbi_section_headers_read(struct msf *msf, struct msf_stream *stream, struct dbi_section_header **out_headers, uint32_t *out_count, FILE *file_stream);

struct dbi_omap_record
{
    uint32_t source_address;
    uint32_t target_address;
};
static_assert(sizeof(struct dbi_omap_record) == 8);

void dbi_omap_record_print(struct dbi_omap_record *record, uint32_t depth, FILE *stream);
void dbi_omap_records_read(struct msf *msf, struct msf_stream *stream, struct dbi_omap_record **out_records, uint32_t *out_count, FILE *file_stream);

struct dbi_address_map
{
    uint32_t omap_to_src_record_count;
    uint32_t omap_from_src_record_count;
    uint32_t section_header_count;
    uint32_t original_section_header_count;

    struct dbi_omap_record *omap_to_src_records;
    struct dbi_omap_record *omap_from_src_records;
    struct dbi_section_header *section_headers;
    struct dbi_section_header *original_section_headers;
};

void dbi_address_map_read(struct dbi_address_map *map, struct msf *msf, struct dbi_extra_streams *extra_streams, FILE *file_stream);
void dbi_address_map_dispose(struct dbi_address_map *map);
void dbi_address_map_print(struct dbi_address_map *map, uint32_t depth, FILE *stream);
