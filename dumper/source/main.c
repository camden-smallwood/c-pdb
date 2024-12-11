#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdb.h"
#include "cpp.h"
#include "utils.h"

//---------------------------------------------------------------------------------
// TODO: this is posix only... do it better!
#include <sys/stat.h>
void create_file_path_dirs(char *file_path)
{
    assert(file_path);

    char *dir_path = malloc(strlen(file_path) + 1);
    assert(dir_path);

    char *next_sep = strchr(file_path, '/');

    while (next_sep)
    {
        size_t dir_path_len = next_sep - file_path;
        memcpy(dir_path, file_path, dir_path_len);
        dir_path[dir_path_len] = '\0';
        mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH);
        next_sep = strchr(next_sep + 1, '/');
    }
    
    free(dir_path);
}
//---------------------------------------------------------------------------------

char *sanitize_path(char *path)
{
    assert(path);

    char *substring = strchr(path, ':');

    if (substring)
        path = substring + 1;

    size_t path_length = strlen(path);

    size_t result_length = 0;
    char *result = NULL;

    int was_slash = 0;

    for (size_t i = 0; i < path_length; i++)
    {
        char c = path[i];

        if (c == '\\')
            c = '/';

        if (c == '/' && was_slash)
            continue;

        was_slash = 0;

        DYNARRAY_PUSH(result, result_length, c);

        if (c == '/')
            was_slash = 1;
    }

    char c = '\0';
    DYNARRAY_PUSH(result, result_length, c);

    return result;
}

char *canonizalize_path(const char *out_path, char *root_path, char *path, int is_dir)
{
    assert(out_path);
    assert(path);

    char *result = NULL;

    size_t out_path_length = strlen(out_path);

    if (out_path_length)
    {
        string_append(&result, (char *)out_path);

        if (out_path[out_path_length - 1] != '/' && out_path[out_path_length - 1] != '\\')
            string_append(&result, "/");
    }

    char *prefix = strchr(path, ':');

    if (prefix)
    {
        path = prefix + 1;
    }
    else if (root_path)
    {
        char *sanitized_root_path = sanitize_path(root_path);

        root_path = sanitized_root_path;
        size_t root_path_length = strlen(root_path);

        if (root_path_length)
        {
            while (root_path_length && root_path[0] == '/')
            {
                root_path++;
                root_path_length--;
            }

            string_append(&result, root_path);

            if (root_path[root_path_length - 1] != '/' && root_path[root_path_length - 1] != '\\')
                string_append(&result, "/");
        }

        free(sanitized_root_path);
    }

    char *sanitized_path = sanitize_path(path);
    
    path = sanitized_path;
    size_t path_length = strlen(path);

    while (path_length && path[0] == '/')
    {
        path++;
        path_length--;
    }

    string_append(&result, path);

    free(sanitized_path);

    if (is_dir)
        string_append(&result, "/");

    return result;
}

int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: dumper <pdb-file> <output-dir>\n");
        exit(EXIT_FAILURE);
    }

    const char *pdb_path = argv[1];
    const char *output_path = argv[2];

    //
    // Load the PDB file
    //

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

        char *root_path = NULL;
        char *compiler_path = NULL;
        char *module_path = NULL;
        char *module_pdb_path = NULL;
        char *args_string = NULL;

        switch (symbol->build_info.count)
        {
        case 2:
            root_path = arguments[0];
            module_path = arguments[1];
            break;

        case 5:
            root_path = arguments[0];
            compiler_path = arguments[1];
            module_path = arguments[2];
            module_pdb_path = arguments[3];
            args_string = arguments[4];
            break;

        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled build info symbol: ", __FILE__, __LINE__);
            ipi_symbol_print(symbol, 0, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        assert(root_path);
        assert(module_path);

        module_path = canonizalize_path(output_path, root_path, module_path, 0);
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
            ipi_symbol_print(symbol, 0, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        assert(module_path);

        char *old_module_path = module_path;
        module_path = canonizalize_path(output_path, NULL, module_path, 0);
        assert(module_path);
        free(old_module_path);

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

    // for (uint32_t dbi_module_index = 0; dbi_module_index < pdb_data.modules.count; dbi_module_index++)
    // {
    //     struct dbi_module *dbi_module = &pdb_data.modules.modules[dbi_module_index];
    //     dbi_module_print(dbi_module, 0, stdout);
    //     printf("\n");
    //     //
    //     // TODO
    //     //
    // }

    //
    // Export c++ modules
    //

    for (uint32_t i = 0; i < module_count; i++)
    {
        struct cpp_module *module = &modules[i];

        create_file_path_dirs(module->path);

        FILE *module_stream = fopen(module->path, "a");
        cpp_module_print(module, module_stream);
        fclose(module_stream);
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
