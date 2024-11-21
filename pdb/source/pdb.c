#include <assert.h>
#include <string.h>
#include "pdb.h"
#include "utils.h"

#include "macros_print.h"

void pdb_data_read(struct pdb_data *pdb_data, FILE *stream)
{
    assert(pdb_data);
    assert(stream);

    memset(pdb_data, 0, sizeof(*pdb_data));

    msf_read(&pdb_data->msf, stream);

    pdb_info_read(&pdb_data->pdb_info, &pdb_data->msf, stream);
    pdb_string_table_read(&pdb_data->string_table, &pdb_data->msf, &pdb_data->pdb_info, stream);
    
    if (pdb_data->msf.streams[MSF_STREAM_TPI].size)
    {
        tpi_header_read(&pdb_data->msf, &pdb_data->tpi_header, stream);
        tpi_symbols_read(&pdb_data->tpi_symbols, &pdb_data->msf, &pdb_data->msf.streams[MSF_STREAM_TPI], &pdb_data->tpi_header, stream);
    }

    if (pdb_data->msf.streams[MSF_STREAM_IPI].size)
    {
        ipi_header_read(&pdb_data->ipi_header, &pdb_data->msf, stream);
        ipi_symbols_read(&pdb_data->ipi_symbols, &pdb_data->msf, &pdb_data->msf.streams[MSF_STREAM_IPI], &pdb_data->ipi_header, stream);
    }

    dbi_header_read(&pdb_data->dbi_header, &pdb_data->msf, stream);
    dbi_modules_read(&pdb_data->modules, &pdb_data->msf, &pdb_data->dbi_header, stream);
    dbi_section_contributions_read(&pdb_data->section_contributions, &pdb_data->msf, &pdb_data->dbi_header, stream);
    dbi_extra_streams_read(&pdb_data->extra_streams, &pdb_data->msf, &pdb_data->dbi_header, stream);
    dbi_address_map_read(&pdb_data->address_map, &pdb_data->msf, &pdb_data->extra_streams, stream);
}

void pdb_data_dispose(struct pdb_data *pdb_data)
{
    assert(pdb_data);

    dbi_address_map_dispose(&pdb_data->address_map);
    dbi_extra_streams_dispose(&pdb_data->extra_streams);
    dbi_section_contributions_dispose(&pdb_data->section_contributions);
    dbi_modules_dispose(&pdb_data->modules);
    
    ipi_symbols_dispose(&pdb_data->ipi_symbols);
    
    tpi_symbols_dispose(&pdb_data->tpi_symbols);
    
    pdb_string_table_dispose(&pdb_data->string_table);
    pdb_info_dispose(&pdb_data->pdb_info);

    msf_dispose(&pdb_data->msf);
}

void pdb_data_print(struct pdb_data *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);
    
    PDB_DATA_STRUCT
}
