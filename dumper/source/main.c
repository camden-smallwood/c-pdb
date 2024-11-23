#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdb.h"
#include "cpp.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: dumper <pdb-file>\n");
        exit(EXIT_FAILURE);
    }

    const char *pdb_path = argv[1];
    FILE *pdb_file = fopen(pdb_path, "r");

    if (!pdb_file)
    {
        fprintf(stderr, "ERROR: Failed to open \"%s\"\n", pdb_path);
        exit(EXIT_FAILURE);
    }

    struct pdb_data pdb_data;
    pdb_data_read(&pdb_data, pdb_file);

    fclose(pdb_file);

    //--------------------------------------------------------------------------------
    // WIP: add all types to a single header module

    struct cpp_module module;
    memset(&module, 0, sizeof(module));

    if (pdb_data.tpi_symbols.count)
    {
        for (uint32_t i = pdb_data.tpi_header.minimum_index; i < pdb_data.tpi_header.maximum_index; i++)
        {
            cpp_module_add_type_definition(&module, &pdb_data, i, 0);
        }
    }

    cpp_module_dispose(&module);

    //--------------------------------------------------------------------------------

    pdb_data_dispose(&pdb_data);

    exit(EXIT_SUCCESS);
}
