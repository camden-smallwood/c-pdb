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
    // Load available build info
    //

    for (uint32_t id_index = pdb_data.ipi_header.minimum_index; id_index < pdb_data.ipi_header.maximum_index; id_index++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&pdb_data.ipi_header, &pdb_data.ipi_symbols, id_index);
        assert(symbol);

        if (symbol->type != LF_BUILDINFO)
            continue;

        char **arguments = malloc(symbol->build_info.count * sizeof(char *));

        for (uint32_t i = 0; i < symbol->build_info.count; i++)
            arguments[i] = ipi_string_id_to_string(&pdb_data.ipi_header, &pdb_data.ipi_symbols, symbol->build_info.argument_indices[i]);
        
        // char *root_path = NULL;
        char *compiler_path = NULL;
        char *module_path = NULL;
        char *module_pdb_path = NULL;
        char *args_string = NULL;
        
        switch (symbol->build_info.count)
        {
        case 2:
            // root_path = arguments[0];
            module_path = arguments[1];
            break;
        
        case 5:
            // root_path = arguments[0];
            compiler_path = arguments[1];
            module_path = arguments[2];
            module_pdb_path = arguments[3];
            args_string = arguments[4];
            break;
        
        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled build info symbol: ", __FILE__, __LINE__);
            ipi_symbol_print(symbol, 0 , stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        assert(module_path);

        //
        // Attempt to find an existing module with the same path
        //

        struct cpp_module *module = NULL;

        for (uint32_t i = 0; i < module_count; i++)
        {
            if (strcmp(module_path, modules[i].path) == 0)
            {
                module = &modules[i];
                break;
            }
        }

        //
        // Create a new module if an existing one was not found
        //

        if (!module)
        {
            struct cpp_module new_module;
            memset(&new_module, 0, sizeof(new_module));

            new_module.path = strdup(module_path);
            assert(new_module.path);

            DYNARRAY_PUSH(modules, module_count, new_module);
            
            module = &modules[module_count - 1];
        }

        if (compiler_path)
        {
            module->compiler_path = strdup(compiler_path);
            assert(module->compiler_path);
        }

        if (module_pdb_path)
        {
            module->pdb_path = strdup(module_pdb_path);
            assert(module->pdb_path);
        }

        if (args_string)
        {
            module->args_string = strdup(args_string);
            assert(module->args_string);
        }

        for (uint32_t i = 0; i < symbol->build_info.count; i++)
            free(arguments[i]);
        
        free(arguments);
    }

    //
    // Load available user-defined types
    //

    for (uint32_t id_index = pdb_data.ipi_header.minimum_index; id_index < pdb_data.ipi_header.maximum_index; id_index++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&pdb_data.ipi_header, &pdb_data.ipi_symbols, id_index);
        assert(symbol);

        if (symbol->type != LF_UDT_SRC_LINE && symbol->type != LF_UDT_MOD_SRC_LINE)
            continue;

        char *module_path = NULL;
        uint32_t udt_type_index = UINT32_MAX;
        uint32_t line = 0;

        if (symbol->type == LF_UDT_SRC_LINE)
        {
            module_path = ipi_string_id_to_string(&pdb_data.ipi_header, &pdb_data.ipi_symbols, symbol->udt_src_line.file_id_index);
            udt_type_index = symbol->udt_src_line.udt_type_index;
            line = symbol->udt_src_line.line;
        }
        else if (symbol->type == LF_UDT_MOD_SRC_LINE)
        {
            assert(symbol->udt_mod_src_line.file_string_offset < pdb_data.string_table.header.names_size);
            module_path = strdup(pdb_data.string_table.names_data + symbol->udt_mod_src_line.file_string_offset);
            udt_type_index = symbol->udt_mod_src_line.udt_type_index;
            line = symbol->udt_mod_src_line.line;
        }
        else
        {
            fprintf(stderr, "%s:%i: ERROR: unhandled UDT source line symbol: ", __FILE__, __LINE__);
            ipi_symbol_print(symbol, 0 , stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        assert(module_path);

        //
        // Attempt to find an existing module with the same path
        //

        struct cpp_module *module = NULL;

        for (uint32_t i = 0; i < module_count; i++)
        {
            if (strcmp(module_path, modules[i].path) == 0)
            {
                module = &modules[i];
                break;
            }
        }

        //
        // Create a new module if an existing one was not found
        //

        if (!module)
        {
            struct cpp_module new_module;
            memset(&new_module, 0, sizeof(new_module));

            new_module.path = module_path;
            assert(new_module.path);

            DYNARRAY_PUSH(modules, module_count, new_module);
            
            module = &modules[module_count - 1];
        }

        //
        // Add the user-defined type to the module
        //
        
        cpp_module_add_type_definition(module, &pdb_data, udt_type_index, line);
    }

    //
    // Process DBI modules
    //

    for (uint32_t dbi_module_index = 0; dbi_module_index < pdb_data.modules.count; dbi_module_index++)
    {
        struct dbi_module *dbi_module = &pdb_data.modules.modules[dbi_module_index];
        
        //
        // TODO
        //
    }

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
