#pragma once
#include <stdint.h>
#include <stdio.h>

#include "msf.h"
#include "pdbi.h"
#include "tpi.h"
#include "ipi.h"
#include "dbi.h"

#include "macros_decl.h"

/* ---------- PDB data */

#define PDB_DATA_STRUCT \
STRUCT_DECL(pdb_data) \
    FIELD_STRUCT(struct msf, msf, msf_print) \
    FIELD_STRUCT(struct pdb_info, pdb_info, pdb_info_print) \
    FIELD_STRUCT(struct pdb_string_table, string_table, pdb_string_table_print) \
    FIELD_STRUCT(struct tpi_header, tpi_header, tpi_header_print) \
    FIELD_STRUCT(struct tpi_symbols, tpi_symbols, tpi_symbols_print) \
    FIELD_STRUCT(struct tpi_header, ipi_header, tpi_header_print) \
    FIELD_STRUCT(struct ipi_symbols, ipi_symbols, ipi_symbols_print) \
    FIELD_STRUCT(struct dbi_header, dbi_header, dbi_header_print) \
    FIELD_STRUCT(struct dbi_modules, modules, dbi_modules_print) \
    FIELD_STRUCT(struct dbi_section_contributions, section_contributions, dbi_section_contributions_print) \
    FIELD_STRUCT(struct dbi_extra_streams, extra_streams, dbi_extra_streams_print) \
    FIELD_STRUCT(struct dbi_address_map, address_map, dbi_address_map_print) \
    FIELD_STRUCT(struct cv_symbols, symbol_records, cv_symbols_print) \
STRUCT_END()

PDB_DATA_STRUCT

void pdb_data_read(struct pdb_data *pdb_data, FILE *stream);
void pdb_data_dispose(struct pdb_data *pdb_data);
void pdb_data_print(struct pdb_data *pdb_data, uint32_t depth, FILE *stream);
