#pragma once
#include <stdint.h>
#include <stdio.h>

#include "msf.h"
#include "pdbi.h"
#include "tpi.h"
#include "ipi.h"
#include "dbi.h"

struct pdb_data
{
    /* MSF data */

    struct msf msf;
    
    /* PDBI data */

    struct pdb_info pdb_info;
    struct pdb_string_table string_table;

    /* TPI data */

    struct tpi_header tpi_header;
    struct tpi_symbols tpi_symbols;

    /* IPI data */

    struct tpi_header ipi_header;
    struct ipi_symbols ipi_symbols;

    /* DBI data */

    struct dbi_header dbi_header;
    struct dbi_modules modules;
    struct dbi_section_contributions section_contributions;
    struct dbi_extra_streams extra_streams;
    struct dbi_address_map address_map;
};

void pdb_data_read(struct pdb_data *pdb_data, FILE *stream);
void pdb_data_dispose(struct pdb_data *pdb_data);
void pdb_data_print(struct pdb_data *pdb_data, uint32_t depth, FILE *stream);
