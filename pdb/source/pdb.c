#include <assert.h>
#include <string.h>

#include <pthread.h>

#include "pdb.h"
#include "utils.h"

#include "macros_print.h"

struct pdbi_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct pdb_info *pdb_info;
    struct pdb_string_table *string_table;
};

void *pdbi_thread_proc(void *address)
{
    assert(address);
    struct pdbi_thread_input *pdb_data = (struct pdbi_thread_input *)address;

    pdb_info_read(pdb_data->pdb_info, pdb_data->msf, &pdb_data->file_stream);
    pdb_string_table_read(pdb_data->string_table, pdb_data->msf, pdb_data->pdb_info, &pdb_data->file_stream);

    return NULL;
}

struct tpi_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct tpi_header *tpi_header;
    struct tpi_symbols *tpi_symbols;
};

void *tpi_thread_proc(void *address)
{
    assert(address);
    struct tpi_thread_input *input = (struct tpi_thread_input *)address;

    if (input->msf->streams[MSF_STREAM_TPI].size)
    {
        tpi_header_read(input->msf, input->tpi_header, &input->file_stream);
        tpi_symbols_read(input->tpi_symbols, input->msf, &input->msf->streams[MSF_STREAM_TPI], input->tpi_header, &input->file_stream);
    }

    return NULL;
}

struct ipi_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct tpi_header *ipi_header;
    struct ipi_symbols *ipi_symbols;
};

void *ipi_thread_proc(void *address)
{
    assert(address);
    struct ipi_thread_input *input = (struct ipi_thread_input *)address;

    if (input->msf->streams[MSF_STREAM_IPI].size)
    {
        ipi_header_read(input->ipi_header, input->msf, &input->file_stream);
        ipi_symbols_read(input->ipi_symbols, input->msf, &input->msf->streams[MSF_STREAM_IPI], input->ipi_header, &input->file_stream);
    }

    return NULL;
}

struct dbi_modules_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct pdb_string_table *string_table;
    struct dbi_header *dbi_header;
    struct dbi_modules *modules;
};

void *dbi_modules_thread_proc(void *address)
{
    assert(address);
    struct dbi_modules_thread_input *input = (struct dbi_modules_thread_input *)address;
    dbi_modules_read(input->modules, input->msf, input->dbi_header, input->string_table, &input->file_stream);
    return NULL;
}

struct dbi_section_contributions_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct dbi_header *dbi_header;
    struct dbi_section_contributions *section_contributions;
};

void *dbi_section_contributions_thread_proc(void *address)
{
    assert(address);
    struct dbi_section_contributions_thread_input *input = (struct dbi_section_contributions_thread_input *)address;
    dbi_section_contributions_read(input->section_contributions, input->msf, input->dbi_header, &input->file_stream);
    return NULL;
}

struct dbi_extra_streams_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct dbi_header *dbi_header;
    struct dbi_extra_streams *extra_streams;
};

void *dbi_extra_streams_thread_proc(void *address)
{
    assert(address);
    struct dbi_extra_streams_thread_input *input = (struct dbi_extra_streams_thread_input *)address;
    dbi_extra_streams_read(input->extra_streams, input->msf, input->dbi_header, &input->file_stream);
    return NULL;
}

struct dbi_address_map_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct dbi_extra_streams *extra_streams;
    struct dbi_address_map *address_map;
};

void *dbi_address_map_thread_proc(void *address)
{
    assert(address);
    struct dbi_address_map_thread_input *input = (struct dbi_address_map_thread_input *)address;
    dbi_address_map_read(input->address_map, input->msf, input->extra_streams, &input->file_stream);
    return NULL;
}

struct cv_symbols_thread_input
{
    struct memory_stream file_stream;
    struct msf *msf;
    struct dbi_header *dbi_header;
    struct cv_symbols *symbol_records;
};

void *cv_symbols_thread_proc(void *address)
{
    assert(address);
    struct cv_symbols_thread_input *input = (struct cv_symbols_thread_input *)address;
    
    if (input->msf->streams[input->dbi_header->symbol_records_stream].size)
    {
        uint32_t dbi_symbols_offset = 0;
        
        cv_symbols_read(
            input->symbol_records,
            input->msf,
            &input->msf->streams[input->dbi_header->symbol_records_stream],
            input->msf->streams[input->dbi_header->symbol_records_stream].size,
            &dbi_symbols_offset,
            &input->file_stream);
    }

    return NULL;
}

void pdb_data_read(struct pdb_data *pdb_data, struct memory_stream *stream)
{
    assert(pdb_data);
    assert(stream);

    memset(pdb_data, 0, sizeof(*pdb_data));

    // Read the MSF data
    msf_read(&pdb_data->msf, stream);

    // Create the PDBI thread
    struct pdbi_thread_input pdbi_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .pdb_info = &pdb_data->pdb_info,
        .string_table = &pdb_data->string_table,
    };
    pthread_t pdbi_thread = NULL;
    pthread_create(&pdbi_thread, NULL, pdbi_thread_proc, &pdbi_thread_input);
    
    // Create the TPI thread
    struct tpi_thread_input tpi_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .tpi_header = &pdb_data->tpi_header,
        .tpi_symbols = &pdb_data->tpi_symbols,
    };
    pthread_t tpi_thread = NULL;
    pthread_create(&tpi_thread, NULL, tpi_thread_proc, &tpi_thread_input);

    // Create the IPI thread
    struct ipi_thread_input ipi_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .ipi_header = &pdb_data->ipi_header,
        .ipi_symbols = &pdb_data->ipi_symbols,
    };
    pthread_t ipi_thread = NULL;
    pthread_create(&ipi_thread, NULL, ipi_thread_proc, &ipi_thread_input);

    // Wait for the PDBI thread to finish so we can get the string table
    pthread_join(pdbi_thread, NULL);

    // Read the debug header
    dbi_header_read(&pdb_data->dbi_header, &pdb_data->msf, stream);

    // Create the DBI modules thread
    struct dbi_modules_thread_input dbi_modules_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .string_table = &pdb_data->string_table,
        .dbi_header = &pdb_data->dbi_header,
        .modules = &pdb_data->modules,
    };
    pthread_t dbi_modules_thread = NULL;
    pthread_create(&dbi_modules_thread, NULL, dbi_modules_thread_proc, &dbi_modules_thread_input);

    // Create the DBI section contributions thread
    struct dbi_section_contributions_thread_input dbi_section_contributions_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .dbi_header = &pdb_data->dbi_header,
        .section_contributions = &pdb_data->section_contributions,
    };
    pthread_t dbi_section_contributions_thread = NULL;
    pthread_create(&dbi_section_contributions_thread, NULL, dbi_section_contributions_thread_proc, &dbi_section_contributions_thread_input);

    //
    // TODO: read these:
    // pdb_data->dbi_header.section_map_size
    // pdb_data->dbi_header.file_info_size
    // pdb_data->dbi_header.type_server_map_size
    // pdb_data->dbi_header.debug_header_size
    // pdb_data->dbi_header.ec_substream_size
    //

    // Create the DBI extra streams thread
    struct dbi_extra_streams_thread_input dbi_extra_streams_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .dbi_header = &pdb_data->dbi_header,
        .extra_streams = &pdb_data->extra_streams,
    };
    pthread_t dbi_extra_streams_thread = NULL;
    pthread_create(&dbi_extra_streams_thread, NULL, dbi_extra_streams_thread_proc, &dbi_extra_streams_thread_input);

    // Wait for the DBI extra streams thread to finish
    pthread_join(dbi_extra_streams_thread, NULL);

    // Create the DBI address map thread
    struct dbi_address_map_thread_input dbi_address_map_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .extra_streams = &pdb_data->extra_streams,
        .address_map = &pdb_data->address_map,
    };
    pthread_t dbi_address_map_thread = NULL;
    pthread_create(&dbi_address_map_thread, NULL, dbi_address_map_thread_proc, &dbi_address_map_thread_input);

    // Create the CV symbols thread
    struct cv_symbols_thread_input cv_symbols_thread_input = {
        .file_stream = {
            .address = stream->address,
            .size = stream->size,
            .position = 0,
        },
        .msf = &pdb_data->msf,
        .dbi_header = &pdb_data->dbi_header,
        .symbol_records = &pdb_data->symbol_records,
    };
    pthread_t cv_symbols_thread = NULL;
    pthread_create(&cv_symbols_thread, NULL, cv_symbols_thread_proc, &cv_symbols_thread_input);

    // Wait for the rest of the threads to finish
    pthread_join(tpi_thread, NULL);
    pthread_join(ipi_thread, NULL);
    pthread_join(dbi_modules_thread, NULL);
    pthread_join(dbi_section_contributions_thread, NULL);
    pthread_join(dbi_address_map_thread, NULL);
    pthread_join(cv_symbols_thread, NULL);
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
