#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"

#include "macros_decl.h"

#define DBI_MACHINE_TYPE_ENUM \
ENUM_DECL(dbi_machine_type) \
    ENUM_VALUE(DBI_MACHINE_TYPE_UNKNOWN, 0x0) \
    ENUM_VALUE(DBI_MACHINE_TYPE_AM33, 0x13) \
    ENUM_VALUE(DBI_MACHINE_TYPE_AMD64, 0x8664) \
    ENUM_VALUE(DBI_MACHINE_TYPE_ARM, 0x1C0) \
    ENUM_VALUE(DBI_MACHINE_TYPE_ARM64, 0xAA64) \
    ENUM_VALUE(DBI_MACHINE_TYPE_ARM_NT, 0x1C4) \
    ENUM_VALUE(DBI_MACHINE_TYPE_EBC, 0xEBC) \
    ENUM_VALUE(DBI_MACHINE_TYPE_X86, 0x14C) \
    ENUM_VALUE(DBI_MACHINE_TYPE_IA64, 0x200) \
    ENUM_VALUE(DBI_MACHINE_TYPE_M32R, 0x9041) \
    ENUM_VALUE(DBI_MACHINE_TYPE_MIPS16, 0x266) \
    ENUM_VALUE(DBI_MACHINE_TYPE_MIPS_FPU, 0x366) \
    ENUM_VALUE(DBI_MACHINE_TYPE_MIPS_FPU16, 0x466) \
    ENUM_VALUE(DBI_MACHINE_TYPE_POWER_PC, 0x1F0) \
    ENUM_VALUE(DBI_MACHINE_TYPE_POWER_PC_FP, 0x1F1) \
    ENUM_VALUE(DBI_MACHINE_TYPE_POWER_PC_ALTIVEC, 0x1F2) \
    ENUM_VALUE(DBI_MACHINE_TYPE_R4000, 0x166) \
    ENUM_VALUE(DBI_MACHINE_TYPE_RISC_V32, 0x5032) \
    ENUM_VALUE(DBI_MACHINE_TYPE_RISC_V64, 0x5064) \
    ENUM_VALUE(DBI_MACHINE_TYPE_RISC_V128, 0x5128) \
    ENUM_VALUE(DBI_MACHINE_TYPE_SH3, 0x1A2) \
    ENUM_VALUE(DBI_MACHINE_TYPE_SH3_DSP, 0x1A3) \
    ENUM_VALUE(DBI_MACHINE_TYPE_SH4, 0x1A6) \
    ENUM_VALUE(DBI_MACHINE_TYPE_SH5, 0x1A8) \
    ENUM_VALUE(DBI_MACHINE_TYPE_THUMB, 0x1C2) \
    ENUM_VALUE(DBI_MACHINE_TYPE_WCE_MIPS_V2, 0x169) \
    ENUM_VALUE(DBI_MACHINE_TYPE_INVALID, 0xffff) \
ENUM_END(dbi_machine_type)

DBI_MACHINE_TYPE_ENUM

void dbi_machine_type_print(enum dbi_machine_type type, FILE *stream);

#define DBI_HEADER_STRUCT \
STRUCT_DECL(dbi_header) \
    FIELD_PRIMITIVE(uint32_t, signature, "%u") \
    FIELD_PRIMITIVE(uint32_t, version, "%u") \
    FIELD_PRIMITIVE(uint32_t, age, "%u") \
    FIELD_PRIMITIVE(uint16_t, gs_symbols_stream, "%u") \
    FIELD_PRIMITIVE(uint16_t, internal_version, "%u") \
    FIELD_PRIMITIVE(uint16_t, ps_symbols_stream, "%u") \
    FIELD_PRIMITIVE(uint16_t, pdb_dll_build_version, "%u") \
    FIELD_PRIMITIVE(uint16_t, symbol_records_stream, "%u") \
    FIELD_PRIMITIVE(uint16_t, pdb_dll_rbld_version, "%u") \
    FIELD_PRIMITIVE(uint32_t, module_list_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, section_contribution_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, section_map_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, file_info_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, type_server_map_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, mfc_type_server_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, debug_header_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, ec_substream_size, "%u") \
    FIELD_PRIMITIVE(uint16_t, flags, "%u") \
    FIELD_PRIMITIVE_FMT(uint16_t, machine_type, dbi_machine_type_print) \
    FIELD_PRIMITIVE(uint32_t, reserved, "%u") \
STRUCT_END(dbi_header)

DBI_HEADER_STRUCT
static_assert(sizeof(struct dbi_header) == 64);

void dbi_header_read(struct dbi_header *header, struct msf *msf, FILE *stream);
void dbi_header_print(struct dbi_header *header, uint32_t depth, FILE *stream);

#define DBI_SECTION_CONTRIBUTION_STRUCT \
STRUCT_DECL(dbi_section_contribution) \
    FIELD_PRIMITIVE(uint16_t, section, "%u") \
    FIELD_PRIMITIVE(uint16_t, padding1, "%u") \
    FIELD_PRIMITIVE(uint32_t, offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, size, "%u") \
    FIELD_PRIMITIVE(uint32_t, characteristics, "%u") \
    FIELD_PRIMITIVE(uint16_t, module, "%u") \
    FIELD_PRIMITIVE(uint16_t, padding2, "%u") \
    FIELD_PRIMITIVE(uint32_t, data_crc, "%u") \
    FIELD_PRIMITIVE(uint32_t, reloc_crc, "%u") \
STRUCT_END(dbi_section_contribution)

DBI_SECTION_CONTRIBUTION_STRUCT
static_assert(sizeof(struct dbi_section_contribution) == 28);

void dbi_section_contribution_print(struct dbi_section_contribution *contribution, uint32_t depth, FILE *stream);

#define DBI_SECTION_CONTRIBUTIONS_STRUCT \
STRUCT_DECL(dbi_section_contributions) \
    FIELD_PRIMITIVE(uint32_t, version, "%u") \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct dbi_section_contribution *, entries, count, dbi_section_contribution_print) \
STRUCT_END(dbi_section_contributions)

DBI_SECTION_CONTRIBUTIONS_STRUCT

void dbi_section_contributions_read(struct dbi_section_contributions *contributions, struct msf *msf, struct dbi_header *dbi_header, FILE *stream);
void dbi_section_contributions_dispose(struct dbi_section_contributions *contributions);
void dbi_section_contributions_print(struct dbi_section_contributions *contributions, uint32_t depth, FILE *stream);

#define DBI_MODULE_HEADER_STRUCT \
STRUCT_DECL(dbi_module_header) \
    FIELD_PRIMITIVE(uint32_t, opened, "%u") \
    FIELD_STRUCT(struct dbi_section_contribution, section, dbi_section_contribution_print) \
    FIELD_PRIMITIVE(uint16_t, flags, "%u") \
    FIELD_PRIMITIVE(uint16_t, stream, "%u") \
    FIELD_PRIMITIVE(uint32_t, symbols_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, lines_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, c13_lines_size, "%u") \
    FIELD_PRIMITIVE(uint16_t, files, "%u") \
    FIELD_PRIMITIVE(uint16_t, padding, "%u") \
    FIELD_PRIMITIVE(uint32_t, filename_offsets, "%u") \
    FIELD_PRIMITIVE(uint32_t, source, "%u") \
    FIELD_PRIMITIVE(uint32_t, compiler, "%u") \
STRUCT_END(dbi_module_header)

DBI_MODULE_HEADER_STRUCT
static_assert(sizeof(struct dbi_module_header) == 64);

void dbi_module_header_print(struct dbi_module_header *header, uint32_t depth, FILE *stream);

#define DBI_MODULE_STRUCT \
STRUCT_DECL(dbi_module) \
    FIELD_STRUCT(struct dbi_module_header, header, dbi_module_header_print) \
    FIELD_PRIMITIVE(char *, module_name, "\"%s\"") \
    FIELD_PRIMITIVE(char *, object_file_name, "\"%s\"") \
STRUCT_END(dbi_module)

DBI_MODULE_STRUCT

void dbi_module_dispose(struct dbi_module *module);
void dbi_module_print(struct dbi_module *module, uint32_t depth, FILE *stream);

#define DBI_MODULES_STRUCT \
STRUCT_DECL(dbi_modules) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct dbi_module *, modules, count, dbi_module_print) \
STRUCT_END(dbi_modules)

DBI_MODULES_STRUCT

void dbi_modules_read(struct dbi_modules *modules, struct msf *msf, struct dbi_header *dbi_header, FILE *stream);
void dbi_modules_dispose(struct dbi_modules *modules);
void dbi_modules_print(struct dbi_modules *modules, uint32_t depth, FILE *stream);

#define DBI_EXTRA_STREAM_INDEX_ENUM \
ENUM_DECL(dbi_extra_stream_index) \
    ENUM_VALUE(DBI_EXTRA_STREAM_FPO) \
    ENUM_VALUE(DBI_EXTRA_STREAM_EXCEPTION) \
    ENUM_VALUE(DBI_EXTRA_STREAM_FIXUP) \
    ENUM_VALUE(DBI_EXTRA_STREAM_OMAP_TO_SRC) \
    ENUM_VALUE(DBI_EXTRA_STREAM_OMAP_FROM_SRC) \
    ENUM_VALUE(DBI_EXTRA_STREAM_SECTION_HEADERS) \
    ENUM_VALUE(DBI_EXTRA_STREAM_TOKEN_RID_MAP) \
    ENUM_VALUE(DBI_EXTRA_STREAM_XDATA) \
    ENUM_VALUE(DBI_EXTRA_STREAM_PDATA) \
    ENUM_VALUE(DBI_EXTRA_STREAM_FRAMEDATA) \
    ENUM_VALUE(DBI_EXTRA_STREAM_ORIGINAL_SECTION_HEADERS) \
ENUM_END(dbi_extra_stream_index)

DBI_EXTRA_STREAM_INDEX_ENUM

void dbi_extra_stream_index_print(enum dbi_extra_stream_index index, FILE *stream);

#define DBI_EXTRA_STREAMS_STRUCT \
STRUCT_DECL(dbi_extra_streams) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint16_t *, indices, count, "%u") \
STRUCT_END(dbi_extra_streams)

DBI_EXTRA_STREAMS_STRUCT

void dbi_extra_streams_read(struct dbi_extra_streams *extra_streams, struct msf *msf, struct dbi_header *dbi_header, FILE *stream);
void dbi_extra_streams_dispose(struct dbi_extra_streams *extra_streams);
void dbi_extra_streams_print(struct dbi_extra_streams *extra_streams, uint32_t depth, FILE *stream);

#define DBI_SECTION_HEADER_STRUCT \
STRUCT_DECL(dbi_section_header) \
    FIELD_PRIMITIVE_FIXED_ARRAY_FMT(char, name, 8, fixed_string_print) \
    FIELD_PRIMITIVE(uint32_t, virtual_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, virtual_address, "%u") \
    FIELD_PRIMITIVE(uint32_t, size_of_raw_data, "%u") \
    FIELD_PRIMITIVE(uint32_t, pointer_to_raw_data, "%u") \
    FIELD_PRIMITIVE(uint32_t, pointer_to_relocations, "%u") \
    FIELD_PRIMITIVE(uint32_t, pointer_to_line_numbers, "%u") \
    FIELD_PRIMITIVE(uint16_t, number_of_relocations, "%u") \
    FIELD_PRIMITIVE(uint16_t, number_of_line_numbers, "%u") \
    FIELD_PRIMITIVE(uint32_t, characteristics, "%u") \
STRUCT_END(dbi_section_header)

DBI_SECTION_HEADER_STRUCT
static_assert(sizeof(struct dbi_section_header) == 40);

void dbi_section_header_print(struct dbi_section_header *header, uint32_t depth, FILE *stream);
void dbi_section_headers_read(struct msf *msf, struct msf_stream *stream, struct dbi_section_header **out_headers, uint32_t *out_count, FILE *file_stream);

#define DBI_OMAP_RECORD_STRUCT \
STRUCT_DECL(dbi_omap_record) \
    FIELD_PRIMITIVE(uint32_t, source_address, "0x%X") \
    FIELD_PRIMITIVE(uint32_t, target_address, "0x%X") \
STRUCT_END(dbi_omap_record)

DBI_OMAP_RECORD_STRUCT
static_assert(sizeof(struct dbi_omap_record) == 8);

void dbi_omap_record_print(struct dbi_omap_record *record, uint32_t depth, FILE *stream);
void dbi_omap_records_read(struct msf *msf, struct msf_stream *stream, struct dbi_omap_record **out_records, uint32_t *out_count, FILE *file_stream);

#define DBI_ADDRESS_MAP_STRUCT \
STRUCT_DECL(dbi_address_map) \
    FIELD_PRIMITIVE(uint32_t, omap_to_src_record_count, "%u") \
    FIELD_PRIMITIVE(uint32_t, omap_from_src_record_count, "%u") \
    FIELD_PRIMITIVE(uint32_t, section_header_count, "%u") \
    FIELD_PRIMITIVE(uint32_t, original_section_header_count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct dbi_omap_record *, omap_to_src_records, omap_to_src_record_count, dbi_omap_record_print) \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct dbi_omap_record *, omap_from_src_records, omap_from_src_record_count, dbi_omap_record_print) \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct dbi_section_header *, section_headers, section_header_count, dbi_section_header_print) \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct dbi_section_header *, original_section_headers, original_section_header_count, dbi_section_header_print) \
STRUCT_END(dbi_address_map)

DBI_ADDRESS_MAP_STRUCT

void dbi_address_map_read(struct dbi_address_map *map, struct msf *msf, struct dbi_extra_streams *extra_streams, FILE *file_stream);
void dbi_address_map_dispose(struct dbi_address_map *map);
void dbi_address_map_print(struct dbi_address_map *map, uint32_t depth, FILE *stream);
