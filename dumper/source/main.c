#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdb.h"
#include "cpp.h"
#include "utils.h"

void main_initialize(int argc, const char **argv);
void main_dispose(void);
void process_global_types(void);
void process_build_info_ids(void);
void process_user_defined_type_ids(void);
void process_symbol_records(void);
void process_modules(void);
void export_cpp_modules(void);

int main(int argc, const char *argv[])
{
    main_initialize(argc, argv);

    process_global_types();
    process_build_info_ids();
    process_user_defined_type_ids();
    process_symbol_records();
    process_modules();
    
    export_cpp_modules();

    main_dispose();
}

struct
{
    const char *pdb_path;
    const char *output_path;

    struct pdb_data pdb_data;

    uint32_t module_count;
    struct cpp_module *modules;
} main_globals;

void main_initialize(int argc, const char **argv)
{
    memset(&main_globals, 0, sizeof(main_globals));

    if (argc != 3)
    {
        fprintf(stderr, "Usage: dumper <pdb-file> <output-dir>\n");
        exit(EXIT_FAILURE);
    }

    main_globals.pdb_path = argv[1];
    main_globals.output_path = argv[2];

    FILE *pdb_file = fopen(main_globals.pdb_path, "r");

    if (!pdb_file)
    {
        fprintf(stderr, "ERROR: Failed to open \"%s\"\n", main_globals.pdb_path);
        exit(EXIT_FAILURE);
    }

    pdb_data_read(&main_globals.pdb_data, pdb_file);

    fclose(pdb_file);
}

void main_dispose(void)
{
    for (uint32_t i = 0; i < main_globals.module_count; i++)
        cpp_module_dispose(&main_globals.modules[i]);

    free(main_globals.modules);

    pdb_data_dispose(&main_globals.pdb_data);

    exit(EXIT_SUCCESS);
}

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

char *canonizalize_path(char *root_path, char *path, int is_dir)
{
    assert(path);

    char *result = NULL;

    //
    // Add the out path to the result path
    //

    size_t out_path_length = strlen(main_globals.output_path);

    if (out_path_length)
    {
        string_append(&result, (char *)main_globals.output_path);

        if (main_globals.output_path[out_path_length - 1] != '/' && main_globals.output_path[out_path_length - 1] != '\\')
            string_append(&result, "/");
    }

    //
    // Skip the disk root prefix if found, or append the root path
    //

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

    //
    // Split the path into components and canonicalize it
    //

    size_t result_length = strlen(result);

    size_t component_count = 0;
    char **components = NULL;

    char *current = result;

    while (current)
    {
        current = strchr(current, '/');
        if (!current)
            break;
        current++;

        char *next = strchr(current, '/');
        size_t length = next ? (next - current) : (result_length - (current - result));

        char *component = calloc(length + 1, sizeof(char));
        assert(component);

        strncpy(component, current, length);

        if (strcmp(component, ".") == 0)
        {
            free(component);
        }
        else if (strcmp(component, "..") == 0)
        {
            free(component);
            DYNARRAY_POP(components, component_count, sizeof(component));
        }
        else
        {
            DYNARRAY_PUSH(components, component_count, component);
        }

        current = next;
    }

    free(result);
    result = NULL;

    for (size_t i = 0; i < component_count; i++)
    {
        string_append(&result, "/");
        string_append(&result, components[i]);
        free(components[i]);
    }

    free(components);

    if (is_dir)
        string_append(&result, "/");

    return result;
}

struct cpp_module *cpp_module_find_or_create(char *module_path)
{
    assert(module_path);

    //
    // Attempt to find an existing module with the same path
    //

    struct cpp_module *module = NULL;

    for (uint32_t i = 0; i < main_globals.module_count; i++)
    {
        if (strcmp(module_path, main_globals.modules[i].path) == 0)
        {
            module = &main_globals.modules[i];
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

        DYNARRAY_PUSH(main_globals.modules, main_globals.module_count, new_module);

        module = &main_globals.modules[main_globals.module_count - 1];
    }

    return module;
}

void process_global_types(void)
{
    if (main_globals.pdb_data.ipi_symbols.count != 0 || main_globals.pdb_data.tpi_symbols.count == 0)
        return;
    
    struct cpp_module module;
    memset(&module, 0, sizeof(module));

    module.path = strdup("types.h");
    assert(module.path);

    for (uint32_t i = main_globals.pdb_data.tpi_header.minimum_index; i < main_globals.pdb_data.tpi_header.maximum_index; i++)
        cpp_module_add_type_definition(&module, &main_globals.pdb_data, i, 0);

    DYNARRAY_PUSH(main_globals.modules, main_globals.module_count, module);
}

void process_build_info_ids(void)
{
    for (uint32_t id_index = main_globals.pdb_data.ipi_header.minimum_index; id_index < main_globals.pdb_data.ipi_header.maximum_index; id_index++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, id_index);
        assert(symbol);

        if (symbol->type != LF_BUILDINFO)
            continue;

        char **arguments = malloc(symbol->build_info.count * sizeof(char *));

        for (uint32_t i = 0; i < symbol->build_info.count; i++)
            arguments[i] = ipi_string_id_to_string(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, symbol->build_info.argument_indices[i]);

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

        module_path = canonizalize_path(root_path, module_path, 0);
        assert(module_path);

        struct cpp_module *module = cpp_module_find_or_create(module_path);

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
}

void process_user_defined_type_ids(void)
{
    for (uint32_t id_index = main_globals.pdb_data.ipi_header.minimum_index; id_index < main_globals.pdb_data.ipi_header.maximum_index; id_index++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, id_index);
        assert(symbol);

        if (symbol->type != LF_UDT_SRC_LINE && symbol->type != LF_UDT_MOD_SRC_LINE)
            continue;

        char *module_path = NULL;
        uint32_t udt_type_index = UINT32_MAX;
        uint32_t line = 0;

        if (symbol->type == LF_UDT_SRC_LINE)
        {
            module_path = ipi_string_id_to_string(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, symbol->udt_src_line.file_id_index);
            udt_type_index = symbol->udt_src_line.udt_type_index;
            line = symbol->udt_src_line.line;
        }
        else if (symbol->type == LF_UDT_MOD_SRC_LINE)
        {
            assert(symbol->udt_mod_src_line.file_string_offset < main_globals.pdb_data.string_table.header.names_size);
            module_path = strdup(main_globals.pdb_data.string_table.names_data + symbol->udt_mod_src_line.file_string_offset);
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
        module_path = canonizalize_path(NULL, module_path, 0);
        assert(module_path);
        free(old_module_path);

        struct cpp_module *module = cpp_module_find_or_create(module_path);
        cpp_module_add_type_definition(module, &main_globals.pdb_data, udt_type_index, line);
    }
}

void process_symbol_records(void)
{
    char *prev_module_path = NULL;

    for (uint32_t symbol_record_index = 0; symbol_record_index < main_globals.pdb_data.symbol_records.count; symbol_record_index++)
    {
        struct cv_symbol *symbol = &main_globals.pdb_data.symbol_records.symbols[symbol_record_index];
        struct cv_pe_section_offset *code_offset = NULL;

        switch (symbol->type)
        {
        case S_UDT:
        case S_UDT_ST:
        case S_COBOLUDT:
        case S_COBOLUDT_ST:
            // HACK: insert user defined types into the last known module
            //       we do this because we don't know where else to put them :shrug:
            if (prev_module_path)
            {
                char *module_path = strdup(prev_module_path);
                assert(module_path);

                struct cpp_module *module = cpp_module_find_or_create(module_path);
                cpp_module_add_type_definition(module, &main_globals.pdb_data, symbol->user_defined_type_symbol.type_index, 0);
            }
            break;

        case S_PROCREF:
        case S_PROCREF_ST:
        case S_LPROCREF:
        case S_LPROCREF_ST:
            if (symbol->procedure_reference_symbol.module_index)
            {
                assert(symbol->procedure_reference_symbol.module_index < main_globals.pdb_data.modules.count);
                struct dbi_module *referenced_module = &main_globals.pdb_data.modules.modules[symbol->procedure_reference_symbol.module_index];

                if (prev_module_path)
                    free(prev_module_path);

                char *module_path = NULL;

                //
                // TODO: get module_path from referenced_module->module_name
                //

                // prev_module_path = canonizalize_path(NULL, module_path, 0);
                // assert(prev_module_path);
            }
            break;

        case S_PUB32:
        case S_PUB32_ST:
            code_offset = &symbol->public_symbol.code_offset;
            break;

        case S_LDATA32:
        case S_LDATA32_ST:
        case S_GDATA32:
        case S_GDATA32_ST:
        case S_LMANDATA:
        case S_LMANDATA_ST:
        case S_GMANDATA:
        case S_GMANDATA_ST:
            code_offset = &symbol->data_symbol.code_offset;
            break;

        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
            code_offset = &symbol->thread_storage_symbol.code_offset;
            break;

        case S_LPROC32:
        case S_LPROC32_ST:
        case S_GPROC32:
        case S_GPROC32_ST:
        case S_LPROC32_ID:
        case S_GPROC32_ID:
        case S_LPROC32_DPC:
        case S_LPROC32_DPC_ID:
            code_offset = &symbol->procedure_symbol.code_offset;
            break;

        default:
            break;
        }

        if (!code_offset)
            continue;

        for (uint32_t contribution_index = 0; contribution_index < main_globals.pdb_data.section_contributions.count; contribution_index++)
        {
            struct dbi_section_contribution *contribution = &main_globals.pdb_data.section_contributions.entries[contribution_index];

            if ((code_offset->section_index == contribution->section_index) &&
                (code_offset->memory_offset >= contribution->offset) &&
                (code_offset->memory_offset < contribution->offset + contribution->size))
            {
                assert(contribution->module_index < main_globals.pdb_data.modules.count);
                struct dbi_module *contributing_module = &main_globals.pdb_data.modules.modules[contribution->module_index];

                if (prev_module_path)
                    free(prev_module_path);

                char *module_path = NULL;

                //
                // TODO: get module_path from contributing_module->module_name
                //

                // prev_module_path = canonizalize_path(NULL, module_path, 0);
                // assert(prev_module_path);
                break;
            }
        }
    }
}

void process_modules(void)
{
    for (uint32_t dbi_module_index = 0; dbi_module_index < main_globals.pdb_data.modules.count; dbi_module_index++)
    {
        struct dbi_module *dbi_module = &main_globals.pdb_data.modules.modules[dbi_module_index];

        for (uint32_t symbol_index = 0; symbol_index < dbi_module->symbols.count; symbol_index++)
        {
            struct cv_symbol *symbol = &dbi_module->symbols.symbols[symbol_index];

            switch (symbol->type)
            {
                // TODO
            }
        }

        for (uint32_t subsection_index = 0; subsection_index < dbi_module->c11_lines_subsection_count; subsection_index++)
        {
            struct dbi_subsection *subsection = &dbi_module->c11_lines_subsections[subsection_index];
            
            // TODO: remove this VVV
            // dbi_subsection_print(subsection, 0, stdout);
            // printf("\n");
            
            // if (subsection->type == DEBUG_S_FILECHKSMS)
            //     printf("file name: %s\n", main_globals.pdb_data.string_table.names_data + subsection->file_checksum.header.name_offset);
        }

        for (uint32_t subsection_index = 0; subsection_index < dbi_module->c13_lines_subsection_count; subsection_index++)
        {
            struct dbi_subsection *subsection = &dbi_module->c13_lines_subsections[subsection_index];
            
            // TODO: remove this VVV
            // dbi_subsection_print(subsection, 0, stdout);
            // printf("\n");
            
            // if (subsection->type == DEBUG_S_FILECHKSMS)
            //     printf("file name: %s\n", main_globals.pdb_data.string_table.names_data + subsection->file_checksum.header.name_offset);
        }
    }
}

void export_cpp_modules(void)
{
    for (uint32_t i = 0; i < main_globals.module_count; i++)
    {
        struct cpp_module *module = &main_globals.modules[i];

        create_file_path_dirs(module->path);

        FILE *module_stream = fopen(module->path, "a");
        cpp_module_print(module, module_stream);
        fclose(module_stream);
    }
}
