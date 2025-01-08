#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "pdb.h"
#include "cpp.h"
#include "utils.h"
#include "path.h"

static void main_initialize(int argc, const char **argv);
static void main_dispose(void);
static void process_global_types(void);
static void process_build_info_ids(void);
static void process_user_defined_type_ids(void);
static void process_symbol_records(void);
static void process_modules(void);
static void export_cpp_modules(void);

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

    exit(EXIT_SUCCESS);
}

struct
{
    const char *pdb_path;
    const char *output_path;

    struct pdb_data pdb_data;

    uint32_t module_count;
    struct cpp_module *modules;
} static main_globals;

static void main_initialize(int argc, const char **argv)
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

static void main_dispose(void)
{
    for (uint32_t i = 0; i < main_globals.module_count; i++)
        cpp_module_dispose(&main_globals.modules[i]);

    free(main_globals.modules);

    pdb_data_dispose(&main_globals.pdb_data);
}

static void create_file_path_dirs(char *file_path)
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
#ifdef _WIN32
        mkdir(dir_path);
#else
        mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH);
#endif
        next_sep = strchr(next_sep + 1, '/');
    }

    free(dir_path);
}

static char *sanitize_path(char *path)
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

static char *canonicalize_path(char *root_path, char *path, int is_dir)
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

    struct path final_path;
    path_from_string(&final_path, result);

    free(result);
    result = path_to_string(&final_path, is_dir);

    path_dispose(&final_path);

    return result;
}

static struct cpp_module *cpp_module_find(char *module_path)
{
    assert(module_path);

    struct cpp_module *module = NULL;

    for (uint32_t i = 0; i < main_globals.module_count; i++)
    {
        if (strcasecmp(module_path, main_globals.modules[i].path) == 0)
        {
            module = &main_globals.modules[i];
            break;
        }
    }

    return module;
}

static struct cpp_module *cpp_module_create(char *module_path)
{
    assert(module_path);

    struct cpp_module new_module;
    memset(&new_module, 0, sizeof(new_module));

    new_module.path = module_path;

    DYNARRAY_PUSH(main_globals.modules, main_globals.module_count, new_module);

    return &main_globals.modules[main_globals.module_count - 1];
}

static inline struct cpp_module *cpp_module_find_or_create(char *module_path)
{
    assert(module_path);

    struct cpp_module *result = cpp_module_find(module_path);

    if (result)
    {
        free(module_path);
        return result;
    }

    return cpp_module_create(module_path);
}

static const char *const CPP_SOURCE_FILE_EXTS[] = {
    "c",
    "cc",
    "cpp",
    "cxx",
    "pch",
    "asm",
    "fasm",
    "masm",
    "res",
    "exp",
};

static const size_t CPP_SOURCE_FILE_EXT_COUNT =
    sizeof(CPP_SOURCE_FILE_EXTS) / sizeof(CPP_SOURCE_FILE_EXTS[0]);

static void dbi_module_get_file_path_and_header_paths(
    struct dbi_module *dbi_module,
    char **out_file_path,
    size_t *out_header_path_count,
    char ***out_header_paths)
{
    assert(dbi_module);

    assert(out_file_path);
    *out_file_path = NULL;

    if (out_header_path_count || out_header_paths)
    {
        assert(out_header_path_count);
        *out_header_path_count = 0;

        assert(out_header_paths);
        *out_header_paths = NULL;
    }

    char *obj_path_string = sanitize_path(dbi_module->module_name);
    assert(obj_path_string);

    struct path obj_path;
    path_from_string(&obj_path, obj_path_string);

    char *obj_stem = path_get_file_stem(&obj_path);
    char *obj_ext = path_get_extension(&obj_path);

    uint32_t subsection_count = dbi_module->c11_lines_subsection_count;
    struct dbi_subsection *subsections = dbi_module->c11_lines_subsections;

    if (subsection_count == 0)
    {
        subsection_count = dbi_module->c13_lines_subsection_count;
        subsections = dbi_module->c13_lines_subsections;
    }

    uint32_t source_files_found = 0;

    for (uint32_t subsection_index = 0; subsection_index < subsection_count; subsection_index++)
    {
        struct dbi_subsection *subsection = &subsections[subsection_index];
        
        if (subsection->type != DEBUG_S_FILECHKSMS)
            continue;

        for (uint32_t i = 0; i < subsection->file_checksums.count; i++)
        {
            struct dbi_file_checksum *entry = &subsection->file_checksums.entries[i];

            assert(entry->header.name_offset < main_globals.pdb_data.string_table.header.names_size);
            char *file_path = sanitize_path(main_globals.pdb_data.string_table.names_data + entry->header.name_offset);
            assert(file_path);

            struct path path;
            path_from_string(&path, file_path);

            char *file_stem = path_get_file_stem(&path);
            assert(file_stem);

            char *file_ext = path_get_extension(&path);
            
            if (file_ext)
            {
                for (size_t j = 0; j < CPP_SOURCE_FILE_EXT_COUNT; j++)
                {
                    if (strcasecmp(file_ext, CPP_SOURCE_FILE_EXTS[j]) == 0)
                        source_files_found++;
                    else
                        continue;
                    
                    // If the file extension is a recognized source file extension, and the file stem
                    // matches the .obj file stem, add it to the headers list
                    if (strcasecmp(file_stem, obj_stem) == 0)
                    {
                        free(file_path);
                        path_dispose(&path);

                        file_path = canonicalize_path(NULL, main_globals.pdb_data.string_table.names_data + entry->header.name_offset, 0);
                        assert(file_path);

                        path_from_string(&path, file_path);
                        
                        *out_file_path = path_to_string(&path, 0);
                        assert(*out_file_path);
                        break;
                    }
                }
            }
            
            if (!*out_file_path && out_header_path_count && out_header_paths)
            {
                char *header_path = path_to_string(&path, 0);
                assert(header_path);
                DYNARRAY_PUSH(*out_header_paths, *out_header_path_count, header_path);
            }

            path_dispose(&path);

            free(file_stem);
            free(file_ext);
            free(file_path);
        }

        if (subsections == dbi_module->c11_lines_subsections && subsection_index == subsection_count - 1)
        {
            subsection_count = dbi_module->c13_lines_subsection_count;
            subsections = dbi_module->c13_lines_subsections;
            subsection_index = UINT32_MAX;
        }
    }

    // HACK: if we didn't find an exact match, but we found a single source file, use that
    if (!*out_file_path && source_files_found == 1)
    {
        subsection_count = dbi_module->c11_lines_subsection_count;
        subsections = dbi_module->c11_lines_subsections;

        if (subsection_count == 0)
        {
            subsection_count = dbi_module->c13_lines_subsection_count;
            subsections = dbi_module->c13_lines_subsections;
        }

        for (uint32_t subsection_index = 0; subsection_index < subsection_count; subsection_index++)
        {
            struct dbi_subsection *subsection = &subsections[subsection_index];
            
            if (subsection->type != DEBUG_S_FILECHKSMS)
                continue;

            for (uint32_t i = 0; i < subsection->file_checksums.count; i++)
            {
                struct dbi_file_checksum *entry = &subsection->file_checksums.entries[i];

                assert(entry->header.name_offset < main_globals.pdb_data.string_table.header.names_size);
                char *file_path = sanitize_path(main_globals.pdb_data.string_table.names_data + entry->header.name_offset);
                assert(file_path);

                struct path path;
                path_from_string(&path, file_path);

                char *file_ext = path_get_extension(&path);
                
                if (file_ext)
                {
                    for (size_t j = 0; j < CPP_SOURCE_FILE_EXT_COUNT; j++)
                    {
                        if (strcasecmp(file_ext, CPP_SOURCE_FILE_EXTS[j]) == 0)
                        {
                            free(file_path);
                            path_dispose(&path);

                            file_path = canonicalize_path(NULL, main_globals.pdb_data.string_table.names_data + entry->header.name_offset, 0);
                            assert(file_path);

                            path_from_string(&path, file_path);
                            
                            *out_file_path = path_to_string(&path, 0);
                            assert(*out_file_path);
                            break;
                        }
                    }
                }
                
                path_dispose(&path);

                free(file_ext);
                free(file_path);
            }

            if (*out_file_path)
                break;

            if (subsections == dbi_module->c11_lines_subsections && subsection_index == subsection_count - 1)
            {
                subsection_count = dbi_module->c13_lines_subsection_count;
                subsections = dbi_module->c13_lines_subsections;
                subsection_index = UINT32_MAX;
            }
        }
    }

    if (!*out_file_path &&
        strcasecmp(obj_path_string, "* CIL *") != 0 &&
        strcasecmp(obj_path_string, "* Linker *") != 0 &&
        strcasecmp(obj_path_string, "* Linker Generated Manifest RES *") != 0 &&
        (obj_ext && strcasecmp(obj_ext, "exp") != 0))
    {
        fprintf(stderr, "WARNING: Failed to find source file of \"%s\"", obj_path_string);

        uint32_t printed_count = 0;

        for (uint32_t subsection_index = 0; subsection_index < subsection_count; subsection_index++)
        {
            struct dbi_subsection *subsection = &subsections[subsection_index];
            
            if (subsection->type != DEBUG_S_FILECHKSMS)
                continue;

            for (uint32_t i = 0; i < subsection->file_checksums.count; i++)
            {
                struct dbi_file_checksum *entry = &subsection->file_checksums.entries[i];

                assert(entry->header.name_offset < main_globals.pdb_data.string_table.header.names_size);
                char *file_path = sanitize_path(main_globals.pdb_data.string_table.names_data + entry->header.name_offset);
                assert(file_path);

                if (printed_count == 0)
                    fprintf(stderr, " in list:\n");
                
                fprintf(stderr, "    [%u]: \"%s\"\n", printed_count++, file_path);
                free(file_path);
            }
        }

        fprintf(stderr, "\n");
    }

    path_dispose(&obj_path);

    free(obj_ext);
    free(obj_stem);
    free(obj_path_string);
}

static char *dbi_module_get_file_path(struct dbi_module *dbi_module)
{
    char *file_path = NULL;
    dbi_module_get_file_path_and_header_paths(dbi_module, &file_path, NULL, NULL);

    return file_path;
}

static void process_global_types(void)
{
    //
    // NOTE:
    // If there are no available symbols in the IPI stream, then we don't have any information
    // about what files each of the symbols in the TPI stream belong in. When this is encountered,
    // we just export every type into a toplevel "types.h" header file. There *may* be a better
    // way to do this, but I think this is a reasonable approach for now...
    //

    if (main_globals.pdb_data.ipi_symbols.count != 0 || main_globals.pdb_data.tpi_symbols.count == 0)
        return;

    struct cpp_module module;
    memset(&module, 0, sizeof(module));

    module.path = canonicalize_path(NULL, "types.h", 0);
    assert(module.path);

    for (uint32_t i = main_globals.pdb_data.tpi_header.minimum_index; i < main_globals.pdb_data.tpi_header.maximum_index; i++)
        cpp_module_add_type_definition(&module, &main_globals.pdb_data, i, 0);

    DYNARRAY_PUSH(main_globals.modules, main_globals.module_count, module);
}

static void process_build_info_ids(void)
{
    for (uint32_t id_index = main_globals.pdb_data.ipi_header.minimum_index; id_index < main_globals.pdb_data.ipi_header.maximum_index; id_index++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, id_index);
        assert(symbol);

        if (symbol->type != LF_BUILDINFO)
            continue;

        char **arguments = malloc(symbol->build_info.argument_count * sizeof(char *));

        for (uint32_t i = 0; i < symbol->build_info.argument_count; i++)
            arguments[i] = ipi_string_id_to_string(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, symbol->build_info.argument_id_indices[i]);

        char *root_path = NULL;
        char *compiler_path = NULL;
        char *module_path = NULL;
        char *module_pdb_path = NULL;
        char *args_string = NULL;

        switch (symbol->build_info.argument_count)
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

        module_path = canonicalize_path(root_path, module_path, 0);
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

        for (uint32_t i = 0; i < symbol->build_info.argument_count; i++)
            free(arguments[i]);

        free(arguments);
    }
}

static void process_user_defined_type_ids(void)
{
    for (uint32_t id_index = main_globals.pdb_data.ipi_header.minimum_index; id_index < main_globals.pdb_data.ipi_header.maximum_index; id_index++)
    {
        struct ipi_symbol *symbol = ipi_symbol_get(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, id_index);
        assert(symbol);

        if (symbol->type != LF_UDT_SRC_LINE && symbol->type != LF_UDT_MOD_SRC_LINE)
            continue;

        char *module_path;
        uint32_t udt_type_index;
        uint32_t line;

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
        module_path = canonicalize_path(NULL, module_path, 0);
        assert(module_path);
        free(old_module_path);

        struct cpp_module *module = cpp_module_find_or_create(module_path);
        cpp_module_add_type_definition(module, &main_globals.pdb_data, udt_type_index, line);
    }
}

static void process_symbol_records(void)
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
        {
            // HACK: insert user defined types into the last known module
            //       we do this because we don't know where else to put them :shrug:
            if (prev_module_path)
            {
                char *module_path = strdup(prev_module_path);
                assert(module_path);

                struct cpp_module *module = cpp_module_find_or_create(module_path);
                
                char *type_name = cpp_type_name(
                    &main_globals.pdb_data,
                    symbol->user_defined_type_symbol.type_index,
                    symbol->user_defined_type_symbol.name,
                    0,
                    NULL,
                    0);
                assert(type_name);

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_TYPEDEF,
                    .typedef_ = {
                        .type_name = type_name,
                    },
                };

                DYNARRAY_PUSH(module->members, module->member_count, member);
            }
            break;
        }

        case S_PROCREF:
        case S_PROCREF_ST:
        case S_LPROCREF:
        case S_LPROCREF_ST:
            if (symbol->procedure_reference_symbol.module_index)
            {
                assert(symbol->procedure_reference_symbol.module_index < main_globals.pdb_data.modules.count);
                struct dbi_module *referenced_module = &main_globals.pdb_data.modules.modules[symbol->procedure_reference_symbol.module_index];

                char *module_path = dbi_module_get_file_path(referenced_module);
                
                if (module_path)
                {
                    if (prev_module_path)
                        free(prev_module_path);
                    
                    prev_module_path = strdup(module_path);
                    assert(prev_module_path);

                    free(module_path);
                }
            }
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

        case S_PUB32:
        case S_PUB32_ST:
            code_offset = &symbol->public_symbol.code_offset;
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

        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
            code_offset = &symbol->thread_storage_symbol.code_offset;
            break;
        
        case S_TRAMPOLINE:
            code_offset = &symbol->trampoline_symbol.target_offset;
            break;
        
        case S_LABEL32:
        case S_LABEL32_ST:
            code_offset = &symbol->label_symbol.section_offset;
            break;
        
        case S_BLOCK32:
        case S_BLOCK32_ST:
            code_offset = &symbol->block_symbol.section_offset;
            break;
        
        case S_THUNK32:
        case S_THUNK32_ST:
            code_offset = &symbol->thunk_symbol.section_offset;
            break;
        
        case S_SEPCODE:
            code_offset = &symbol->separated_code_symbol.section_offset;
            break;
        
        case S_FRAMEPROC:
            code_offset = &symbol->frame_proc_symbol.exception_handler_offset;
            break;
        
        case S_CALLSITEINFO:
            code_offset = &symbol->call_site_info_symbol.code_offset;
            break;
        
        case S_COFFGROUP:
            code_offset = &symbol->coff_group_symbol.code_offset;
            break;
        
        case S_ANNOTATION:
            code_offset = &symbol->annotation_symbol.code_offset;
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

                char *module_path = dbi_module_get_file_path(contributing_module);
                
                if (module_path)
                {
                    if (prev_module_path)
                        free(prev_module_path);
                    
                    prev_module_path = strdup(module_path);
                    assert(prev_module_path);
                    
                    free(module_path);
                    break;
                }
            }
        }
    }

    if (prev_module_path)
        free(prev_module_path);
}

static uint32_t process_scoped_symbols(struct dbi_module *dbi_module, uint32_t start_index, uint16_t scope_end_symbol_type);

static uint32_t process_procedure_symbols(struct dbi_module *dbi_module, uint32_t start_index)
{
    return process_scoped_symbols(dbi_module, start_index, S_END);
}

static uint32_t process_inline_site_symbols(struct dbi_module *dbi_module, uint32_t start_index)
{
    return process_scoped_symbols(dbi_module, start_index, S_INLINESITE_END);
}

static uint32_t process_thunk_symbols(struct dbi_module *dbi_module, uint32_t start_index)
{
    assert(dbi_module);

    for (uint32_t symbol_index = start_index; symbol_index < dbi_module->symbols.count; symbol_index++)
    {
        struct cv_symbol *cv_symbol = &dbi_module->symbols.symbols[symbol_index];

        switch (cv_symbol->type)
        {
        case S_END:
            return (symbol_index - start_index) + 1;
        
        case S_FRAMEPROC:
            // TODO
            break;
        
        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type: ", __FILE__, __LINE__);
            cv_symbol_type_print(cv_symbol->type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }
    }

    return dbi_module->symbols.count - start_index;
}

static uint32_t process_separated_code_symbols(struct dbi_module *dbi_module, uint32_t start_index)
{
    assert(dbi_module);

    for (uint32_t symbol_index = start_index; symbol_index < dbi_module->symbols.count; symbol_index++)
    {
        struct cv_symbol *cv_symbol = &dbi_module->symbols.symbols[symbol_index];

        switch (cv_symbol->type)
        {
        case S_END:
            return (symbol_index - start_index) + 1;
        
        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type: ", __FILE__, __LINE__);
            cv_symbol_type_print(cv_symbol->type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }
    }

    return dbi_module->symbols.count - start_index;
}

static uint32_t process_block_symbols(struct dbi_module *dbi_module, uint32_t start_index)
{
    return process_scoped_symbols(dbi_module, start_index, S_END);
}

static uint32_t process_scoped_symbols(struct dbi_module *dbi_module, uint32_t start_index, uint16_t scope_end_symbol_type)
{
    assert(dbi_module);

    for (uint32_t symbol_index = start_index; symbol_index < dbi_module->symbols.count; symbol_index++)
    {
        struct cv_symbol *cv_symbol = &dbi_module->symbols.symbols[symbol_index];

        if (cv_symbol->type == scope_end_symbol_type)
            return (symbol_index - start_index) + 1;

        switch (cv_symbol->type)
        {
        case S_BLOCK32:
        case S_BLOCK32_ST:
            symbol_index += process_block_symbols(dbi_module, symbol_index + 1);
            break;
        
        case S_INLINESITE:
        case S_INLINESITE2:
            symbol_index += process_inline_site_symbols(dbi_module, symbol_index + 1);
            break;

        case S_SEPCODE:
            symbol_index += process_separated_code_symbols(dbi_module, symbol_index + 1);
            break;
        
        case S_REGISTER:
        case S_REGISTER_ST:
            // TODO
            break;
        
        case S_REGREL32:
            // TODO
            break;
        
        case S_DEFRANGE:
        case S_DEFRANGE_SUBFIELD:
        case S_DEFRANGE_REGISTER:
        case S_DEFRANGE_FRAMEPOINTER_REL:
        case S_DEFRANGE_SUBFIELD_REGISTER:
        case S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE:
        case S_DEFRANGE_REGISTER_REL:
            // TODO
            break;
        
        case S_FRAMECOOKIE:
        case S_FRAMEPROC:
            // TODO
            break;
        
        case S_CALLSITEINFO:
            // TODO
            break;
        
        case S_FILESTATIC:
            // TODO
            break;
        
        case S_CALLERS:
        case S_CALLEES:
            // TODO
            break;
        
        case S_ANNOTATION:
            // TODO
            break;
        
        case S_CONSTANT:
        case S_CONSTANT_ST:
        case S_MANCONSTANT:
            // TODO
            break;
        
        case S_LDATA32:
        case S_LDATA32_ST:
        case S_GDATA32:
        case S_GDATA32_ST:
        case S_LMANDATA:
        case S_LMANDATA_ST:
        case S_GMANDATA:
        case S_GMANDATA_ST:
            // TODO
            break;
        
        case S_LOCAL:
            // TODO
            break;
        
        case S_LABEL32:
        case S_LABEL32_ST:
            // TODO
            break;
        
        case S_UDT:
        case S_UDT_ST:
        case S_COBOLUDT:
        case S_COBOLUDT_ST:
            // TODO
            break;
        
        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
            // TODO
            break;

        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type: ", __FILE__, __LINE__);
            cv_symbol_type_print(cv_symbol->type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }
    }

    return dbi_module->symbols.count - start_index;
}

static void process_modules(void)
{
    for (uint32_t dbi_module_index = 0; dbi_module_index < main_globals.pdb_data.modules.count; dbi_module_index++)
    {
        struct dbi_module *dbi_module = &main_globals.pdb_data.modules.modules[dbi_module_index];

        char *file_path = NULL;
        
        size_t header_path_count = 0;
        char **header_paths = NULL;

        dbi_module_get_file_path_and_header_paths(dbi_module, &file_path, &header_path_count, &header_paths);

        if (!file_path)
        {
            for (size_t i = 0; i < header_path_count; i++)
                free(header_paths[i]);
            free(header_paths);
            continue;
        }

        struct cpp_module *cpp_module = cpp_module_find_or_create(file_path);
        assert(cpp_module);

        for (size_t i = 0; i < header_path_count; i++)
        {
            int header_found = 0;

            for (size_t j = 0; j < cpp_module->header_count; j++)
            {
                if (strcasecmp(header_paths[i], cpp_module->headers[j].path) == 0)
                {
                    header_found = 1;
                    break;
                }
            }

            if (header_found)
                continue;
            
            char *header_path = strdup(header_paths[i]);
            assert(header_path);

            header_paths[i] = NULL;

            //
            // TODO: determine global include paths and fix header_path if possible
            //

            struct cpp_header header = {
                .flags = CPP_HEADER_IS_LOCAL,
                .path = header_path,
            };

            DYNARRAY_PUSH(cpp_module->headers, cpp_module->header_count, header);
        }
    
        for (size_t i = 0; i < header_path_count; i++)
            free(header_paths[i]);
        
        free(header_paths);

        for (uint32_t symbol_index = 0; symbol_index < dbi_module->symbols.count; symbol_index++)
        {
            struct cv_symbol *cv_symbol = &dbi_module->symbols.symbols[symbol_index];

            switch (cv_symbol->type)
            {
            case S_LPROC32:
            case S_LPROC32_ST:
            case S_GPROC32:
            case S_GPROC32_ST:
            case S_LPROC32_ID:
            case S_GPROC32_ID:
            case S_LPROC32_DPC:
            case S_LPROC32_DPC_ID:
            {
                symbol_index += process_procedure_symbols(dbi_module, symbol_index + 1);

                char *signature = cpp_type_name(
                    &main_globals.pdb_data,
                    cv_symbol->procedure_symbol.type_index,
                    cv_symbol->procedure_symbol.name,
                    0, // TODO
                    NULL, // TODO
                    0);
                assert(signature);

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_PROCEDURE,
                    .procedure = {
                        .address = 0, // TODO
                        .line = 0, // TODO
                        .type_index = cv_symbol->procedure_symbol.type_index,
                        .signature = signature,
                        .body = NULL, // TODO
                    }
                };

                DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
                break;
            }
            
            case S_THUNK32:
            case S_THUNK32_ST:
                symbol_index += process_thunk_symbols(dbi_module, symbol_index + 1);
                break;
            
            case S_SEPCODE:
                symbol_index += process_separated_code_symbols(dbi_module, symbol_index + 1);
                break;
            
            case S_UNAMESPACE:
            case S_UNAMESPACE_ST:
            {
                char *using_namespace = strdup(cv_symbol->using_namespace_symbol.name);
                assert(using_namespace);

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_USING_NAMESPACE,
                    .using_namespace = using_namespace,
                };

                DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
                break;
            }

            case S_UDT:
            case S_UDT_ST:
            case S_COBOLUDT:
            case S_COBOLUDT_ST:
            {
                char *type_name = cpp_type_name(
                    &main_globals.pdb_data,
                    cv_symbol->user_defined_type_symbol.type_index,
                    cv_symbol->user_defined_type_symbol.name,
                    0,
                    NULL,
                    0);
                assert(type_name);
                
                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_TYPEDEF,
                    .typedef_ = {
                        .type_name = type_name,
                    },
                };

                DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
                break;
            }
            
            case S_CONSTANT:
            case S_CONSTANT_ST:
            case S_MANCONSTANT:
            {
                char *type_name = cpp_type_name(
                    &main_globals.pdb_data,
                    cv_symbol->constant_symbol.type_index,
                    cv_symbol->constant_symbol.name,
                    0,
                    NULL,
                    0);
                assert(type_name);
                size_t type_name_length = strlen(type_name);

                char *value_string = tpi_enumerate_variant_to_string(&cv_symbol->constant_symbol.value);
                assert(value_string);
                size_t value_string_length = strlen(value_string);

                size_t result_length = type_name_length + /* = */3 + value_string_length + /*;\0*/2;
                int prepend = 0;

                if (!string_starts_with(type_name, "const "))
                {
                    result_length += /*const */6;
                    prepend = 1;
                }

                char *result = malloc(result_length);
                assert(result);
                sprintf(result, "%s%s = %s;", prepend ? "const " : "", type_name, value_string);

                free(value_string);
                free(type_name);

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_CONSTANT,
                    .constant = result,
                };

                DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
                break;
            }
            
            case S_LDATA32:
            case S_LDATA32_ST:
            case S_GDATA32:
            case S_GDATA32_ST:
            case S_LMANDATA:
            case S_LMANDATA_ST:
            case S_GMANDATA:
            case S_GMANDATA_ST:
            {
                char *type_name = cpp_type_name(
                    &main_globals.pdb_data,
                    cv_symbol->data_symbol.type_index,
                    cv_symbol->data_symbol.name,
                    0,
                    NULL,
                    0);
                assert(type_name);
                size_t type_name_length = strlen(type_name);

                size_t result_length = type_name_length + /*;\0*/2;
                char *result = malloc(result_length);
                assert(result);
                sprintf(result, "%s;", type_name);

                free(type_name);
                
                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_DATA,
                    .data = result,
                };

                DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
                break;
            }
            
            case S_LTHREAD32:
            case S_LTHREAD32_ST:
            case S_GTHREAD32:
            case S_GTHREAD32_ST:
            {
                char *type_name = cpp_type_name(
                    &main_globals.pdb_data,
                    cv_symbol->thread_storage_symbol.type_index,
                    cv_symbol->thread_storage_symbol.name,
                    0,
                    NULL,
                    0);
                assert(type_name);
                size_t type_name_length = strlen(type_name);

                size_t result_length = /*thread_local */13 + type_name_length + /*;\0*/2;
                char *result = malloc(result_length);
                assert(result);
                sprintf(result, "thread_local %s;", type_name);

                free(type_name);

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_THREAD_STORAGE,
                    .constant = result,
                };

                DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
                break;
            }
            
            case S_EXPORT:
                // TODO
                break;
            
            case S_PUB32:
            case S_PUB32_ST:
                // TODO
                break;
            
            case S_PROCREF:
            case S_PROCREF_ST:
            case S_LPROCREF:
            case S_LPROCREF_ST:
                // TODO
                break;
            
            case S_OBJNAME:
            case S_OBJNAME_ST:
                // TODO
                break;
            
            case S_BUILDINFO:
                // TODO
                break;
            
            case S_COMPILE2:
            case S_COMPILE2_ST:
            case S_COMPILE3:
                // TODO
                break;
            
            case S_ENVBLOCK:
                // TODO
                break;
            
            case S_LABEL32:
            case S_LABEL32_ST:
                // TODO
                break;
            
            default:
                fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type: ", __FILE__, __LINE__);
                cv_symbol_type_print(cv_symbol->type, stderr);
                fprintf(stderr, "\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

static void export_cpp_modules(void)
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
