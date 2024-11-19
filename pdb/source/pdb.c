#include <assert.h>
#include <string.h>
#include "pdb.h"
#include "utils.h"

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

void pdb_data_print(struct pdb_data *pdb_data, uint32_t depth, FILE *stream)
{
    assert(pdb_data);
    assert(stream);

    fprintf(stream, "pdb_data {\n");

    fprintf_depth(stream, depth + 1, "pdb_info: ");
    pdb_info_print(&pdb_data->pdb_info, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "string_table: ");
    pdb_string_table_print(&pdb_data->string_table, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "tpi_header: ");
    tpi_header_print(&pdb_data->tpi_header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "tpi_symbols: ");
    tpi_symbols_print(&pdb_data->tpi_symbols, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "ipi_header: ");
    tpi_header_print(&pdb_data->ipi_header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "ipi_symbols: ");
    ipi_symbols_print(&pdb_data->ipi_symbols, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "dbi_header: ");
    dbi_header_print(&pdb_data->dbi_header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "modules: ");
    dbi_modules_print(&pdb_data->modules, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "section_contributions: ");
    dbi_section_contributions_print(&pdb_data->section_contributions, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "extra_streams: ");
    dbi_extra_streams_print(&pdb_data->extra_streams, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "address_map: ");
    dbi_address_map_print(&pdb_data->address_map, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth, "}");
}
