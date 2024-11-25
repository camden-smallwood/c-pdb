#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdb.h"
#include "cpp.h"
#include "utils.h"

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

    //
    // Set up C++ containers
    //

    uint32_t module_count = 0;
    struct cpp_module *modules = NULL;

    //
    // Dump all types to a single header module if we don't have any ID symbols
    //

    if (!pdb_data.ipi_symbols.count && pdb_data.tpi_symbols.count)
    {
        struct cpp_module module;
        memset(&module, 0, sizeof(module));

        module.path = strdup("types.h");
        assert(module.path);

        for (uint32_t i = pdb_data.tpi_header.minimum_index; i < pdb_data.tpi_header.maximum_index; i++)
            cpp_module_add_type_definition(&module, &pdb_data, i, 0);

        DYNARRAY_PUSH(modules, module_count, module);
    }

    //
    // Load available build info and user-defined types
    //

    for (uint32_t i = pdb_data.ipi_header.minimum_index; i < pdb_data.ipi_header.maximum_index; i++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&pdb_data.ipi_header, &pdb_data.ipi_symbols, i);
        assert(symbol);

        switch (symbol->type)
        {
        case LF_BUILDINFO:
        {
            char *build_info = calloc(0, sizeof(char));
            assert(build_info);

            for (uint32_t i = 0; i < symbol->build_info.count; i++)
            {
                char *argument = ipi_string_id_to_string(&pdb_data.ipi_header, &pdb_data.ipi_symbols, symbol->build_info.argument_indices[i]);
                string_append(&build_info, argument);
            }

            fprintf(stderr, "build_info: %s\n", build_info);
            free(build_info);
            break;
        }
        
        case LF_UDT_SRC_LINE:
        case LF_UDT_MOD_SRC_LINE:
            // TODO
            break;
        
        default:
            break;
        }
    }

    //
    // Load modules
    //

    // --- TODO ---

    //
    // Dispose c++ modules
    //

    for (uint32_t i = 0; i < module_count; i++)
        cpp_module_dispose(&modules[i]);
    
    free(modules);

    //
    // Cleanup
    //

    pdb_data_dispose(&pdb_data);

    exit(EXIT_SUCCESS);
}
