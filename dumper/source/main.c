#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#   include <direct.h>
#   define strcasecmp stricmp
#else
#   include <unistd.h>
#   include <fcntl.h>
#   include <sys/stat.h>
#   include <sys/mman.h>
#endif

#include "pdb.h"
#include "cpp.h"
#include "utils.h"
#include "path.h"
#include "memory_stream.h"

/* ---------- private prototypes */

static void main_initialize(int argc, const char **argv);
static void main_dispose(void);
static void process_global_types(void);
static void process_build_info_ids(void);
static void process_user_defined_type_ids(void);
static void process_symbol_records(void);
static void process_modules(void);
static void export_cpp_modules(void);

/* ---------- public code */

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

/* ---------- private variables */

struct sanitized_path
{
    uint32_t string_table_offset;
    char *path;
};

struct canonicalized_path
{
    char *root_path;
    char *path;
    int is_dir;
    char *canonicalized;
};

struct string_id_string
{
    uint32_t id_index;
    char *string;
};

struct dbi_module_paths
{
    uint32_t module_index;
    uint32_t path_count;
    uint32_t source_file_index;
    char **paths;
};

struct
{
    const char *pdb_path;
    const char *output_path;

    uint64_t base_address;

    int unroll_functions;

    struct pdb_data pdb_data;

    uint32_t cpp_module_count;
    struct cpp_module *cpp_modules;

    uint32_t sanitized_path_count;
    struct sanitized_path *sanitized_paths;

    uint32_t canonicalized_path_count;
    struct canonicalized_path *canonicalized_paths;

    uint32_t string_id_string_count;
    struct string_id_string *string_id_strings;

    uint32_t dbi_module_path_count;
    struct dbi_module_paths *dbi_module_paths;
} static main_globals;

/* ---------- private code */

static void main_initialize(int argc, const char **argv)
{
    memset(&main_globals, 0, sizeof(main_globals));

    //
    // Parse the input arguments
    //

    if (argc < 3 || argc > 5)
    {
        fprintf(stderr, "Usage: dumper <pdb-file> <output-dir> [base-address] [--unroll-functions|-u]\n");
        exit(EXIT_FAILURE);
    }

    main_globals.pdb_path = argv[1];
    main_globals.output_path = argv[2];

    if (argc >= 4)
        sscanf(argv[3], "%" SCNx64, &main_globals.base_address);

    if (argc >= 5 && (strcmp(argv[4], "--unroll-functions") == 0 || strcmp(argv[4], "-u") == 0))
        main_globals.unroll_functions = 1;

    //
    // Map the PDB file to memory
    //

    int fd = open(main_globals.pdb_path, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "ERROR: Failed to open \"%s\"\n", main_globals.pdb_path);
        exit(EXIT_FAILURE);
    }

    struct stat file_stats;
    if (fstat(fd, &file_stats) == -1)
    {
        close(fd);
        fprintf(stderr, "ERROR: Failed to get size of \"%s\"\n", main_globals.pdb_path);
        exit(EXIT_FAILURE);
    }
    
    void *pdb_file_memory = mmap(NULL, file_stats.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (pdb_file_memory == MAP_FAILED)
    {
        close(fd);
        fprintf(stderr, "ERROR: Failed to map file to memory: \"%s\"\n", main_globals.pdb_path);
        exit(EXIT_FAILURE);
    }

    close(fd);

    //
    // Read the PDB data from the mapped memory
    //

    struct memory_stream file_stream = {
        .address = pdb_file_memory,
        .size = file_stats.st_size,
        .position = 0,
    };

    pdb_data_read(&main_globals.pdb_data, &file_stream);

    //
    // Cleanup
    //
    
    munmap(pdb_file_memory, file_stats.st_size);
}

static void main_dispose(void)
{
    for (uint32_t i = 0; i < main_globals.dbi_module_path_count; i++)
    {
        struct dbi_module_paths *entry = &main_globals.dbi_module_paths[i];

        for (uint32_t j = 0; j < entry->path_count; j++)
            free(entry->paths[j]);
        
        free(entry->paths);
    }

    free(main_globals.dbi_module_paths);

    for (uint32_t i = 0; i < main_globals.string_id_string_count; i++)
        free(main_globals.string_id_strings[i].string);
    free(main_globals.string_id_strings);

    for (uint32_t i = 0; i < main_globals.canonicalized_path_count; i++)
        free(main_globals.canonicalized_paths[i].canonicalized);
    free(main_globals.canonicalized_paths);

    for (uint32_t i = 0; i < main_globals.sanitized_path_count; i++)
        free(main_globals.sanitized_paths[i].path);
    free(main_globals.sanitized_paths);

    for (uint32_t i = 0; i < main_globals.cpp_module_count; i++)
        cpp_module_dispose(&main_globals.cpp_modules[i]);
    free(main_globals.cpp_modules);

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

static char *sanitize_path_from_string_table(uint32_t string_table_offset)
{
    assert(string_table_offset < main_globals.pdb_data.string_table.header.names_size);

    for (uint32_t i = 0; i < main_globals.sanitized_path_count; i++)
    {
        struct sanitized_path *entry = &main_globals.sanitized_paths[i];

        if (entry->string_table_offset == string_table_offset)
            return strdup(entry->path);
    }

    char *path = main_globals.pdb_data.string_table.names_data + string_table_offset;
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

    struct sanitized_path new_entry = {
        .string_table_offset = string_table_offset,
        .path = strdup(result),
    };
    assert(new_entry.path);
    DYNARRAY_PUSH(main_globals.sanitized_paths, main_globals.sanitized_path_count, new_entry);

    return result;
}

static char *canonicalize_path(char *root_path, char *path, int is_dir)
{
    assert(path);

    for (uint32_t i = 0; i < main_globals.canonicalized_path_count; i++)
    {
        struct canonicalized_path *entry = &main_globals.canonicalized_paths[i];

        if (root_path == entry->root_path && path == entry->path && is_dir == entry->is_dir)
        {
            char *result = strdup(entry->canonicalized);
            assert(result);
            return result;
        }
    }

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

        char *current_root_path = sanitized_root_path;
        size_t root_path_length = strlen(current_root_path);

        if (root_path_length)
        {
            while (root_path_length && current_root_path[0] == '/')
            {
                current_root_path++;
                root_path_length--;
            }

            string_append(&result, current_root_path);

            if (current_root_path[root_path_length - 1] != '/' && current_root_path[root_path_length - 1] != '\\')
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

    struct canonicalized_path new_entry = {
        .root_path = root_path,
        .path = path,
        .is_dir = is_dir,
        .canonicalized = strdup(result),
    };
    assert(new_entry.canonicalized);
    DYNARRAY_PUSH(main_globals.canonicalized_paths, main_globals.canonicalized_path_count, new_entry);

    return result;
}

static struct cpp_module *cpp_module_find(char *module_path)
{
    assert(module_path);

    struct cpp_module *module = NULL;

    for (uint32_t i = 0; i < main_globals.cpp_module_count; i++)
    {
        if (strcasecmp(module_path, main_globals.cpp_modules[i].path) == 0)
        {
            module = &main_globals.cpp_modules[i];
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

    DYNARRAY_PUSH(main_globals.cpp_modules, main_globals.cpp_module_count, new_module);

    return &main_globals.cpp_modules[main_globals.cpp_module_count - 1];
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

static struct dbi_module_paths *dbi_module_get_paths(uint32_t module_index)
{
    assert(module_index < main_globals.pdb_data.modules.count);

    for (uint32_t i = 0; i < main_globals.dbi_module_path_count; i++)
    {
        struct dbi_module_paths *entry = &main_globals.dbi_module_paths[i];

        if (entry->module_index == module_index)
            return entry;
    }

    struct dbi_module *dbi_module = &main_globals.pdb_data.modules.modules[module_index];

    struct dbi_module_paths new_entry = {
        .module_index = module_index,
        .path_count = 0,
        .paths = NULL,
        .source_file_index = UINT32_MAX,
    };

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

    // struct dbi_subsection *found_subsections = NULL;
    // uint32_t found_subsection_count = 0;
    // uint32_t found_subsection_index = UINT32_MAX;
    // uint32_t found_file_checksum_index = UINT32_MAX;
    // uint32_t source_files_found = 0;

    for (uint32_t subsection_index = 0; subsection_index < subsection_count; subsection_index++)
    {
        struct dbi_subsection *subsection = &subsections[subsection_index];
        
        if (subsection->type != DEBUG_S_FILECHKSMS)
            continue;

        for (uint32_t file_index = 0; file_index < subsection->file_checksums.count; file_index++)
        {
            struct dbi_file_checksum *entry = &subsection->file_checksums.entries[file_index];

            char *file_path = sanitize_path_from_string_table(entry->header.name_offset);
            assert(file_path);

            struct path path;
            path_from_string(&path, file_path);

            char *file_stem = path_get_file_stem(&path);
            assert(file_stem);

            char *file_ext = path_get_extension(&path);

            int is_source_file = 0;
            
            if (file_ext)
            {
                for (size_t ext_index = 0; ext_index < CPP_SOURCE_FILE_EXT_COUNT; ext_index++)
                {
                    if (strcasecmp(file_ext, CPP_SOURCE_FILE_EXTS[ext_index]) != 0)
                        continue;
                    
                    // found_subsections = subsections;
                    // found_subsection_count = subsection_count;
                    // found_file_checksum_index = file_index;
                    // found_subsection_index = subsection_index;
                    // source_files_found++;

                    // If the file extension is a recognized source file extension, and the file stem
                    // matches the .obj file stem, add it to the headers list
                    if (strcasecmp(file_stem, obj_stem) == 0)
                    {
                        free(file_path);
                        path_dispose(&path);

                        file_path = canonicalize_path(NULL, main_globals.pdb_data.string_table.names_data + entry->header.name_offset, 0);
                        assert(file_path);

                        path_from_string(&path, file_path);
                        
                        char *source_file_path = path_to_string(&path, 0);
                        assert(*source_file_path);

                        is_source_file = 1;
                        new_entry.source_file_index = new_entry.path_count;
                        DYNARRAY_PUSH(new_entry.paths, new_entry.path_count, source_file_path);
                    }

                    // We already found the file extension, so no point in checking other extensions
                    break;
                }
            }
            
            if (!is_source_file)
            {
                char *header_path = path_to_string(&path, 0);
                assert(header_path);
                DYNARRAY_PUSH(new_entry.paths, new_entry.path_count, header_path);
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

    /*// HACK: if we didn't find an exact match, but we found a single source file, use that
    if (new_entry.source_file_index == UINT32_MAX && source_files_found == 1)
    {
        assert(found_subsections);
        assert(found_subsection_index < found_subsection_count);

        struct dbi_subsection *subsection = &found_subsections[found_subsection_index];
        assert(found_file_checksum_index < subsection->file_checksums.count);

        struct dbi_file_checksum *entry = &subsection->file_checksums.entries[found_file_checksum_index];

        char *file_path = sanitize_path_from_string_table(entry->header.name_offset);
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
    }*/

    /*if (!*out_file_path &&
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

                char *file_path = sanitize_path_from_string_table(entry->header.name_offset);
                assert(file_path);

                if (printed_count == 0)
                    fprintf(stderr, " in list:\n");
                
                fprintf(stderr, "    [%u]: \"%s\"\n", printed_count++, file_path);
                free(file_path);
            }
        }

        if (printed_count == 0)
            fprintf(stderr, ": file list is empty");

        fprintf(stderr, "\n");
    }*/

    path_dispose(&obj_path);

    free(obj_ext);
    free(obj_stem);
    free(obj_path_string);

    DYNARRAY_PUSH(main_globals.dbi_module_paths, main_globals.dbi_module_path_count, new_entry);
    return &main_globals.dbi_module_paths[main_globals.dbi_module_path_count - 1];
}

static char *cv_pe_section_offset_to_module_path(struct cv_pe_section_offset *code_offset)
{
    assert(code_offset);

    for (uint32_t contribution_index = 0; contribution_index < main_globals.pdb_data.section_contributions.count; contribution_index++)
    {
        struct dbi_section_contribution *contribution = &main_globals.pdb_data.section_contributions.entries[contribution_index];

        if ((code_offset->section_index == contribution->section_index) &&
            (code_offset->memory_offset >= contribution->offset) &&
            (code_offset->memory_offset < contribution->offset + contribution->size))
        {
            struct dbi_module_paths *paths = dbi_module_get_paths(contribution->module_index);
            assert(paths);

            if (paths->source_file_index == UINT32_MAX)
                return NULL;
            
            char *result = strdup(paths->paths[paths->source_file_index]);
            assert(result);

            return result;
        }
    }

    return NULL;
}

static inline uint64_t cv_pe_section_offset_to_pe_address(struct cv_pe_section_offset *offset)
{
    return main_globals.base_address + dbi_address_map_pe_offset_to_pe_address(&main_globals.pdb_data.address_map, offset);
}

static char *ipi_string_id_to_string(uint32_t index)
{
    for (uint32_t i = 0; i < main_globals.string_id_string_count; i++)
    {
        struct string_id_string *entry = &main_globals.string_id_strings[i];

        if (entry->id_index == index)
            return entry->string;
    }
    
    struct ipi_symbol *argument_symbol = ipi_symbol_get(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, index);

    assert(argument_symbol);
    assert(argument_symbol->type == LF_STRING_ID);

    char *string = calloc(1, sizeof(char));
    assert(string);

    struct ipi_symbol *substrings_symbol = ipi_symbol_get(&main_globals.pdb_data.ipi_header, &main_globals.pdb_data.ipi_symbols, argument_symbol->string_id.substrings_index);
    
    if (substrings_symbol)
    {
        assert(substrings_symbol->type = LF_SUBSTR_LIST);

        for (uint32_t i = 0; i < substrings_symbol->substr_list.count; i++)
        {
            char *substring = ipi_string_id_to_string(substrings_symbol->substr_list.substring_indices[i]);
            string_append(&string, substring);
        }
    }

    string_append(&string, argument_symbol->string_id.string);

    struct string_id_string new_entry = {
        .id_index = index,
        .string = string,
    };
    DYNARRAY_PUSH(main_globals.string_id_strings, main_globals.string_id_string_count, new_entry);

    return string;
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

    DYNARRAY_PUSH(main_globals.cpp_modules, main_globals.cpp_module_count, module);
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
            arguments[i] = ipi_string_id_to_string(symbol->build_info.argument_id_indices[i]);

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

        free(arguments);
    }
}

static void process_user_defined_type_ids(void)
{
    char **id_to_module_paths = calloc(main_globals.pdb_data.ipi_symbols.count, sizeof(char *));
    assert(id_to_module_paths);

    struct string_offset_to_module_path
    {
        uint32_t string_offset;
        char *module_path;
    };

    uint32_t string_offset_to_module_path_count = 0;
    struct string_offset_to_module_path *string_offset_to_module_paths = NULL;

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
            uint32_t absolute_id_index = ipi_index_to_absolute_index(
                &main_globals.pdb_data.ipi_header,
                &main_globals.pdb_data.ipi_symbols,
                symbol->udt_src_line.file_id_index);

            assert(absolute_id_index != UINT32_MAX);
            
            if (!id_to_module_paths[absolute_id_index])
            {
                char *old_module_path = ipi_string_id_to_string(symbol->udt_src_line.file_id_index);
                
                id_to_module_paths[absolute_id_index] = canonicalize_path(NULL, old_module_path, 0);
                assert(id_to_module_paths[absolute_id_index]);
            }
            
            module_path = strdup(id_to_module_paths[absolute_id_index]);
            assert(module_path);

            udt_type_index = symbol->udt_src_line.udt_type_index;
            line = symbol->udt_src_line.line;
        }
        else if (symbol->type == LF_UDT_MOD_SRC_LINE)
        {
            struct string_offset_to_module_path *entry = NULL;

            for (uint32_t i = 0; i < string_offset_to_module_path_count; i++)
            {
                if (string_offset_to_module_paths[i].string_offset == symbol->udt_mod_src_line.file_string_offset)
                {
                    entry = &string_offset_to_module_paths[i];
                    break;
                }
            }

            if (!entry)
            {
                assert(symbol->udt_mod_src_line.file_string_offset < main_globals.pdb_data.string_table.header.names_size);
                module_path = canonicalize_path(NULL, main_globals.pdb_data.string_table.names_data + symbol->udt_mod_src_line.file_string_offset, 0);
                assert(module_path);

                struct string_offset_to_module_path new_entry = {
                    .string_offset = symbol->udt_mod_src_line.file_string_offset,
                    .module_path = module_path,
                };

                DYNARRAY_PUSH(string_offset_to_module_paths, string_offset_to_module_path_count, new_entry);
                entry = &string_offset_to_module_paths[string_offset_to_module_path_count - 1];
            }

            module_path = strdup(entry->module_path);
            assert(module_path);
            
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

        struct cpp_module *module = cpp_module_find_or_create(module_path);
        cpp_module_add_type_definition(module, &main_globals.pdb_data, udt_type_index, line);
    }

    for (uint32_t i = 0; i < main_globals.pdb_data.ipi_symbols.count; i++)
        free(id_to_module_paths[i]);
    
    free(id_to_module_paths);
    
    for (uint32_t i = 0; i < string_offset_to_module_path_count; i++)
        free(string_offset_to_module_paths[i].module_path);
    
    free(string_offset_to_module_paths);
}

static void process_symbol_records(void)
{
    struct cpp_module *cpp_module = NULL;

    //
    // NOTE:
    // We insert public constants and public user-defined types into the last known module.
    // We do this because there's no other known way to determine where they should go.
    //

    for (uint32_t symbol_record_index = 0; symbol_record_index < main_globals.pdb_data.symbol_records.count; symbol_record_index++)
    {
        struct cv_symbol *symbol = &main_globals.pdb_data.symbol_records.symbols[symbol_record_index];

        switch (symbol->type)
        {
        case S_PUB32:
        case S_PUB32_ST:
        {
            //
            // TODO:
            // These appear to be public symbol references...
            // We should verify if they should be included here or not.
            //

            // char *module_path = cv_pe_section_offset_to_module_path(&symbol->public_.code_offset);

            // if (!module_path)
            //     break;
            
            // cpp_module = cpp_module_find_or_create(module_path);
            // assert(cpp_module);
            
            //
            // TODO: do we need to do anything else?
            //
            break;
        }

        case S_CONSTANT:
        case S_CONSTANT_ST:
        case S_MANCONSTANT:
        {
            //
            // TODO:
            // These appear to be constant references...
            // We should verify if they should be included here or not.
            //

            // if (!cpp_module)
            //     break;
            
            // char *type_name = cpp_type_name(
            //     &main_globals.pdb_data,
            //     symbol->constant.type_index,
            //     symbol->constant.name,
            //     0,
            //     NULL,
            //     0);
            // assert(type_name);
            
            // char *value_string = tpi_enumerate_variant_to_string(&symbol->constant.value);
            // assert(value_string);

            // size_t type_name_length = strlen(type_name);
            // size_t value_string_length = strlen(value_string);
            // size_t result_length = type_name_length + /* = */3 + value_string_length + /*;\0*/2;

            // // HACK: only prepend with "const" if it doesn't already start with "const"
            // int prepend = 0;
            // if (!string_starts_with(type_name, "const "))
            // {
            //     result_length += /*const */6;
            //     prepend = 1;
            // }

            // char *result = malloc(result_length);
            // assert(result);

            // sprintf(result, "%s%s = %s;", prepend ? "const " : "", type_name, value_string);

            // free(value_string);
            // free(type_name);

            // //
            // // HACK: Don't add the variable if it was already added...
            // //

            // int exists = 0;

            // for (uint32_t i = 0; i < cpp_module->member_count; i++)
            // {
            //     struct cpp_module_member *member = &cpp_module->members[i];

            //     if (member->type != CPP_MODULE_MEMBER_TYPE_CONSTANT)
            //         continue;
                
            //     if (strcmp(result, member->constant) == 0)
            //     {
            //         exists = 1;
            //         break;
            //     }
            // }

            // if (exists)
            // {
            //     free(result);
            //     break;
            // }
            
            // struct cpp_module_member member = {
            //     .type = CPP_MODULE_MEMBER_TYPE_CONSTANT,
            //     .constant = result,
            // };

            // DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
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
            char *module_path = cv_pe_section_offset_to_module_path(&symbol->data.code_offset);
            
            if (!module_path)
                break;
            
            cpp_module = cpp_module_find_or_create(module_path);
            assert(cpp_module);

            char *type_name = cpp_type_name(
                &main_globals.pdb_data,
                symbol->data.type_index,
                symbol->data.name,
                0,
                NULL,
                0);
            assert(type_name);

            size_t type_name_length = strlen(type_name);
            size_t result_length = type_name_length + /*;\0*/ 2;

            char *result = malloc(result_length);
            assert(result);

            sprintf(result, "%s;", type_name);
        
            struct dbi_module *dbi_module = dbi_modules_get_module_from_pe_offset(
                &main_globals.pdb_data.modules,
                &main_globals.pdb_data.section_contributions,
                &symbol->data.code_offset);
            
            uint64_t address = cv_pe_section_offset_to_pe_address(&symbol->data.code_offset);
            uint32_t line = dbi_module_get_line_from_pe_offset(dbi_module, &symbol->data.code_offset);

            //
            // HACK: Don't add the variable if it was already added...
            //

            int exists = 0;

            for (uint32_t i = 0; i < cpp_module->member_count; i++)
            {
                struct cpp_module_member *member = &cpp_module->members[i];

                if (member->type != CPP_MODULE_MEMBER_TYPE_DATA)
                    continue;
                
                if (strcmp(result, member->data.string) == 0 &&
                    address == member->data.address &&
                    line == member->data.line)
                {
                    exists = 1;
                    break;
                }
            }

            if (exists)
            {
                free(result);
                break;
            }
            
            struct cpp_module_member member = {
                .type = CPP_MODULE_MEMBER_TYPE_DATA,
                .data = {
                    .string = result,
                    .address = address,
                    .line = line,
                }
            };

            DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
            break;
        }

        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
        {
            char *module_path = cv_pe_section_offset_to_module_path(&symbol->thread_storage.code_offset);
            
            if (!module_path)
                break;

            cpp_module = cpp_module_find_or_create(module_path);
            assert(cpp_module);

            char *type_name = cpp_type_name(
                &main_globals.pdb_data,
                symbol->thread_storage.type_index,
                symbol->thread_storage.name,
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

            struct dbi_module *dbi_module = dbi_modules_get_module_from_pe_offset(
                &main_globals.pdb_data.modules,
                &main_globals.pdb_data.section_contributions,
                &symbol->thread_storage.code_offset);
            
            uint64_t address = cv_pe_section_offset_to_pe_address(&symbol->thread_storage.code_offset);
            uint32_t line = dbi_module_get_line_from_pe_offset(dbi_module, &symbol->thread_storage.code_offset);

            //
            // HACK: Don't add the variable if it was already added...
            //

            int exists = 0;

            for (uint32_t i = 0; i < cpp_module->member_count; i++)
            {
                struct cpp_module_member *member = &cpp_module->members[i];

                if (member->type != CPP_MODULE_MEMBER_TYPE_THREAD_LOCAL)
                    continue;
                
                if (strcmp(result, member->thread_local_.string) == 0 &&
                    address == member->thread_local_.address &&
                    line == member->thread_local_.line)
                {
                    exists = 1;
                    break;
                }
            }

            if (exists)
            {
                free(result);
                break;
            }
            
            struct cpp_module_member member = {
                .type = CPP_MODULE_MEMBER_TYPE_THREAD_LOCAL,
                .thread_local_ = {
                    .string = result,
                    .address = address,
                    .line = line,
                }
            };

            DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
            break;
        }
        
        case S_UDT:
        case S_UDT_ST:
        case S_COBOLUDT:
        case S_COBOLUDT_ST:
        {
            //
            // HACK: insert public user defined types into the last known module
            //       we do this because we don't know where else to put them :shrug:
            //

            if (!cpp_module)
                break;
            
            char *type_name = cpp_type_name(
                &main_globals.pdb_data,
                symbol->user_defined_type.type_index,
                symbol->user_defined_type.name,
                0,
                NULL,
                0);
            assert(type_name);

            //
            // HACK: Don't add the typedef if it was already added...
            //

            int exists = 0;

            for (uint32_t i = 0; i < cpp_module->member_count; i++)
            {
                struct cpp_module_member *member = &cpp_module->members[i];

                if (member->type != CPP_MODULE_MEMBER_TYPE_TYPEDEF)
                    continue;
                
                if (strcmp(type_name, member->typedef_.type_name) == 0)
                {
                    exists = 1;
                    break;
                }
            }

            if (exists)
            {
                free(type_name);
                break;
            }
            
            struct cpp_module_member member = {
                .type = CPP_MODULE_MEMBER_TYPE_TYPEDEF,
                .typedef_ = {
                    .type_name = type_name,
                },
            };

            DYNARRAY_PUSH(cpp_module->members, cpp_module->member_count, member);
            break;
        }

        case S_PROCREF:
        case S_PROCREF_ST:
        case S_LPROCREF:
        case S_LPROCREF_ST:
            // TODO: Should we use the referenced module?
            break;
        
        case S_ANNOTATIONREF:
            // TODO: Should we use the referenced module?
            break;

        default:
            fprintf(stderr, "%s:%i: ERROR: Unhandled cv_symbol_type value: ", __FILE__, __LINE__);
            cv_symbol_type_print(symbol->type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }
    }
}

static uint32_t process_scoped_symbols(
    struct dbi_module *dbi_module,
    uint32_t start_index,
    uint16_t scope_end_symbol_type,
    struct cpp_block *out_block);

static uint32_t process_procedure_symbols(struct dbi_module *dbi_module, uint32_t start_index, struct cpp_block *out_block)
{
    return process_scoped_symbols(dbi_module, start_index, S_END, out_block);
}

static uint32_t process_inline_site_symbols(struct dbi_module *dbi_module, uint32_t start_index, struct cpp_block *out_block)
{
    return process_scoped_symbols(dbi_module, start_index, S_INLINESITE_END, out_block);
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

static uint32_t process_block_symbols(struct dbi_module *dbi_module, uint32_t start_index, struct cpp_block *out_block)
{
    return process_scoped_symbols(dbi_module, start_index, S_END, out_block);
}

static uint32_t process_scoped_symbols(
    struct dbi_module *dbi_module,
    uint32_t start_index,
    uint16_t scope_end_symbol_type,
    struct cpp_block *out_block)
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
        {
            struct cpp_statement statement = {
                .type = CPP_STATEMENT_TYPE_BLOCK,
                .block = {
                    .address = 0,
                    .statement_count = 0,
                    .statements = NULL,
                }
            };
            
            symbol_index += process_block_symbols(dbi_module, symbol_index + 1, &statement.block);

            if (out_block)
                DYNARRAY_PUSH(out_block->statements, out_block->statement_count, statement);
            else
                cpp_statement_dispose(&statement);
            break;
        }

        case S_INLINESITE:
        case S_INLINESITE2:
            symbol_index += process_inline_site_symbols(dbi_module, symbol_index + 1, out_block);
            break;

        case S_SEPCODE:
            symbol_index += process_separated_code_symbols(dbi_module, symbol_index + 1);
            break;
        
        case S_REGISTER:
        case S_REGISTER_ST:
            if (out_block)
            {
                char *name = strdup(cv_symbol->register_variable.name);
                assert(name);

                struct cpp_statement statement = {
                    .type = CPP_STATEMENT_TYPE_VARIABLE,
                    .variable = {
                        .type = CPP_VARIABLE_TYPE_REGISTER,
                        .name = name,
                        .signature = cpp_type_name(&main_globals.pdb_data, cv_symbol->register_variable.type_index, cv_symbol->register_variable.name, 0, NULL, 0),
                        .value = NULL,
                        .comment = NULL, // TODO
                    },
                };
                DYNARRAY_PUSH(out_block->statements, out_block->statement_count, statement);
            }
            break;

        case S_REGREL32:
            if (out_block)
            {
                char *name = strdup(cv_symbol->register_relative.name);
                assert(name);

                struct cpp_statement statement = {
                    .type = CPP_STATEMENT_TYPE_VARIABLE,
                    .variable = {
                        .type = CPP_VARIABLE_TYPE_REGISTER_RELATIVE,
                        .name = name,
                        .signature = cpp_type_name(&main_globals.pdb_data, cv_symbol->register_relative.type_index, cv_symbol->register_relative.name, 0, NULL, 0),
                        .value = NULL,
                        .comment = NULL, // TODO
                    },
                };
                DYNARRAY_PUSH(out_block->statements, out_block->statement_count, statement);
            }
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

        //
        // Get the module's file path and header paths
        //

        struct dbi_module_paths *paths = dbi_module_get_paths(dbi_module_index);

        if (paths->source_file_index == UINT32_MAX)
            continue;

        char *file_path = NULL;
        
        size_t header_path_count = 0;
        char **header_paths = NULL;

        for (uint32_t i = 0; i < paths->path_count; i++)
        {
            if (i == paths->source_file_index)
            {
                file_path = strdup(paths->paths[i]);
                assert(file_path);
            }
            else
            {
                char *header_path = strdup(paths->paths[i]);
                assert(header_path);
                DYNARRAY_PUSH(header_paths, header_path_count, header_path);
            }
        }

        //
        // Add the header paths to the C++ module
        //

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

        //
        // Add each of the module's symbols to the C++ module
        //

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
                struct cpp_block *body = calloc(1, sizeof(*body));
                assert(body);

                symbol_index += process_procedure_symbols(dbi_module, symbol_index + 1, body);

                //
                // Collect parameter names from register-based variables
                //

                uint32_t register_variable_name_count = 0;
                char **register_variable_names = NULL;

                uint32_t register_relative_name_count = 0;
                char **register_relative_names = NULL;

                for (uint32_t i = 0; i < body->statement_count; i++)
                {
                    struct cpp_statement *statement = &body->statements[i];

                    if (statement->type != CPP_STATEMENT_TYPE_VARIABLE)
                        continue;
                    
                    struct cpp_variable *variable = &statement->variable;

                    switch (variable->type)
                    {
                    case CPP_VARIABLE_TYPE_REGISTER:
                    {
                        char *name = strdup(variable->name);
                        assert(name);
                        DYNARRAY_PUSH(register_variable_names, register_variable_name_count, name);
                        break;
                    }
                    
                    case CPP_VARIABLE_TYPE_REGISTER_RELATIVE:
                    {
                        char *name = strdup(variable->name);
                        assert(name);
                        DYNARRAY_PUSH(register_relative_names, register_relative_name_count, name);
                        break;
                    }
                    
                    default:
                        break;
                    }
                }

                uint32_t parameter_name_count = register_variable_name_count ? register_variable_name_count : register_relative_name_count;
                char **parameter_names = register_variable_names ? register_variable_names : register_relative_names;

                // Skip `this` parameter
                if (parameter_name_count && strcmp(parameter_names[0], "this") == 0)
                {
                    parameter_name_count--;
                    parameter_names++;
                }

                char *signature = cpp_type_name(
                    &main_globals.pdb_data,
                    cv_symbol->procedure.type_index,
                    cv_symbol->procedure.name,
                    parameter_name_count,
                    parameter_names,
                    0);
                assert(signature);

                //
                // If the procedure is a member function, find its declaration and update the signature
                //

                struct tpi_symbol *procedure_type_symbol = &main_globals.pdb_data.tpi_symbols.symbols[cv_symbol->procedure.type_index];

                if (procedure_type_symbol->leaf == LF_MFUNCTION && strstr(cv_symbol->procedure.name, "::"))
                {
                    size_t part_count = 0;
                    char **parts = NULL;
                    string_split_by_string(cv_symbol->procedure.name, "::", &part_count, &parts);
                    assert(part_count >= 2);

                    char *procedure_name = parts[part_count - 1];

                    char *procedure_class_name = strdup("");
                    assert(procedure_class_name);

                    for (size_t i = 0; i < part_count - 1; i++)
                    {
                        if (i > 0)
                            string_append(&procedure_class_name, "::");
                        string_append(&procedure_class_name, parts[i]);
                        free(parts[i]);
                    }

                    free(parts);

                    struct tpi_symbol *class_type_symbol = NULL;

                    for (uint32_t i = 0; i < main_globals.pdb_data.tpi_symbols.count; i++)
                    {
                        struct tpi_symbol *tpi_symbol = &main_globals.pdb_data.tpi_symbols.symbols[i];

                        switch (tpi_symbol->leaf)
                        {
                        case LF_CLASS:
                        case LF_CLASS_ST:
                        case LF_STRUCTURE:
                        case LF_STRUCTURE_ST:
                        case LF_STRUCTURE19:
                        case LF_INTERFACE:
                            if (strcmp(tpi_symbol->class_.name, procedure_class_name) == 0 && tpi_symbol->class_.size != 0)
                            {
                                class_type_symbol = tpi_symbol;

                                for (uint32_t j = 0; j < main_globals.cpp_module_count; j++)
                                {
                                    struct cpp_module *current_module = &main_globals.cpp_modules[j];

                                    for (uint32_t k = 0; k < current_module->member_count; k++)
                                    {
                                        struct cpp_module_member *current_member = &current_module->members[k];

                                        if (current_member->type != CPP_MODULE_MEMBER_TYPE_CLASS)
                                            continue;
                                        
                                        if (strcmp(current_member->class_.name, procedure_class_name) != 0)
                                            continue;
                                        
                                        for (uint32_t l = 0; l < current_member->class_.member_count; l++)
                                        {
                                            struct cpp_class_member *class_member = &current_member->class_.members[l];

                                            if (class_member->type != CPP_CLASS_MEMBER_TYPE_METHOD)
                                                continue;
                                            
                                            int valid = 0;

                                            if (cv_symbol->procedure.type_index == class_member->method.type_index)
                                            {
                                                valid = 1;
                                            }
                                            else if (strcmp(cv_symbol->procedure.name, class_member->method.name) == 0)
                                            {
                                                struct tpi_symbol *method_symbol = tpi_symbol_get(&main_globals.pdb_data.tpi_header, &main_globals.pdb_data.tpi_symbols, class_member->method.type_index);

                                                if (method_symbol)
                                                {
                                                    assert(method_symbol->leaf == LF_MFUNCTION);

                                                    struct tpi_symbol *argument_list_symbol = tpi_symbol_get(&main_globals.pdb_data.tpi_header, &main_globals.pdb_data.tpi_symbols, method_symbol->member_function.argument_list_type_index);
                                                    
                                                    if (argument_list_symbol)
                                                    {
                                                        assert(argument_list_symbol->leaf == LF_ARGLIST);
                                                        
                                                        if (class_member->method.argument_count == argument_list_symbol->argument_list.count)
                                                        {
                                                            valid = 1;

                                                            for (uint32_t x = 0; x < argument_list_symbol->argument_list.count; x++)
                                                            {
                                                                char *type_name = cpp_type_name(&main_globals.pdb_data, argument_list_symbol->argument_list.type_indices[x], NULL, 0, NULL, 1);
                                                                assert(type_name);
                                                                if (strcmp(type_name, class_member->method.arguments[x]) != 0)
                                                                {
                                                                    valid = 0;
                                                                    free(type_name);
                                                                    break;
                                                                }
                                                                free(type_name);
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                            if (valid)
                                            {
                                                struct tpi_symbol *method_symbol = tpi_symbol_get(&main_globals.pdb_data.tpi_header, &main_globals.pdb_data.tpi_symbols, class_member->method.type_index);

                                                if (method_symbol)
                                                {
                                                    assert(method_symbol->leaf == LF_MFUNCTION);
                                                    
                                                    struct tpi_symbol *argument_list_symbol = tpi_symbol_get(&main_globals.pdb_data.tpi_header, &main_globals.pdb_data.tpi_symbols, method_symbol->member_function.argument_list_type_index);
                                                    
                                                    if (argument_list_symbol)
                                                    {
                                                        assert(argument_list_symbol->leaf == LF_ARGLIST);
                                                        assert(class_member->method.argument_count == argument_list_symbol->argument_list.count);

                                                        for (uint32_t x = 0; x < argument_list_symbol->argument_list.count; x++)
                                                        {
                                                            free(class_member->method.arguments[x]);
                                                            class_member->method.arguments[x] = cpp_type_name(&main_globals.pdb_data, argument_list_symbol->argument_list.type_indices[x], parameter_name_count > x ? parameter_names[x] : NULL, 0, NULL, 1);
                                                            assert(class_member->method.arguments[x]);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        
                        default:
                            break;
                        }

                        if (class_type_symbol)
                            break;
                    }

                    //
                    // TODO:
                    // * find class in CPP modules
                    // * find method in class
                    // * give method parameter names
                    //

                    free(procedure_class_name);
                    free(procedure_name);
                }

                for (uint32_t i = 0; i < register_variable_name_count; i++)
                    free(register_variable_names[i]);
                free(register_variable_names);

                for (uint32_t i = 0; i < register_relative_name_count; i++)
                    free(register_relative_names[i]);
                free(register_relative_names);

                if (!main_globals.unroll_functions)
                {
                    cpp_block_dispose(body);
                    free(body);
                    body = NULL;
                }

                //
                // TODO: don't add the procedure if it was already added...
                //

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_PROCEDURE,
                    .procedure = {
                        .address = cv_pe_section_offset_to_pe_address(&cv_symbol->procedure.code_offset),
                        .line = dbi_module_get_line_from_pe_offset(dbi_module, &cv_symbol->procedure.code_offset),
                        .type_index = cv_symbol->procedure.type_index,
                        .signature = signature,
                        .body = body,
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
                char *using_namespace = strdup(cv_symbol->using_namespace.name);
                assert(using_namespace);

                //
                // HACK: Don't add the using namespace if it was already added...
                //

                int exists = 0;

                for (uint32_t i = 0; i < cpp_module->member_count; i++)
                {
                    struct cpp_module_member *member = &cpp_module->members[i];

                    if (member->type != CPP_MODULE_MEMBER_TYPE_USING_NAMESPACE)
                        continue;
                    
                    if (strcmp(using_namespace, member->using_namespace) == 0)
                    {
                        exists = 1;
                        break;
                    }
                }

                if (exists)
                {
                    free(using_namespace);
                    break;
                }
                
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
                    cv_symbol->user_defined_type.type_index,
                    cv_symbol->user_defined_type.name,
                    0,
                    NULL,
                    0);
                assert(type_name);
                
                //
                // HACK: Don't add the typedef if it was already added...
                //

                int exists = 0;

                for (uint32_t i = 0; i < cpp_module->member_count; i++)
                {
                    struct cpp_module_member *member = &cpp_module->members[i];

                    if (member->type != CPP_MODULE_MEMBER_TYPE_TYPEDEF)
                        continue;
                    
                    if (strcmp(type_name, member->typedef_.type_name) == 0)
                    {
                        exists = 1;
                        break;
                    }
                }

                if (exists)
                {
                    free(type_name);
                    break;
                }
                
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
                    cv_symbol->constant.type_index,
                    cv_symbol->constant.name,
                    0,
                    NULL,
                    0);
                assert(type_name);
                size_t type_name_length = strlen(type_name);

                char *value_string = tpi_enumerate_variant_to_string(&cv_symbol->constant.value);
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

                //
                // HACK: Don't add the variable if it was already added...
                //

                int exists = 0;

                for (uint32_t i = 0; i < cpp_module->member_count; i++)
                {
                    struct cpp_module_member *member = &cpp_module->members[i];

                    if (member->type != CPP_MODULE_MEMBER_TYPE_CONSTANT)
                        continue;
                    
                    if (strcmp(result, member->constant) == 0)
                    {
                        exists = 1;
                        break;
                    }
                }

                if (exists)
                {
                    free(result);
                    break;
                }
                
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
                    cv_symbol->data.type_index,
                    cv_symbol->data.name,
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

                uint64_t address = cv_pe_section_offset_to_pe_address(&cv_symbol->data.code_offset);
                uint32_t line = dbi_module_get_line_from_pe_offset(dbi_module, &cv_symbol->data.code_offset);

                //
                // HACK: Don't add the variable if it was already added...
                //

                int exists = 0;

                for (uint32_t i = 0; i < cpp_module->member_count; i++)
                {
                    struct cpp_module_member *member = &cpp_module->members[i];

                    if (member->type != CPP_MODULE_MEMBER_TYPE_DATA)
                        continue;
                    
                    if (strcmp(result, member->data.string) == 0 &&
                        address == member->data.address &&
                        line == member->data.line)
                    {
                        exists = 1;
                        break;
                    }
                }

                if (exists)
                {
                    free(result);
                    break;
                }
                
                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_DATA,
                    .data = {
                        .string = result,
                        .address = address,
                        .line = line,
                    },
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
                    cv_symbol->thread_storage.type_index,
                    cv_symbol->thread_storage.name,
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

                uint64_t address = cv_pe_section_offset_to_pe_address(&cv_symbol->thread_storage.code_offset);
                uint32_t line = dbi_module_get_line_from_pe_offset(dbi_module, &cv_symbol->thread_storage.code_offset);

                //
                // HACK: Don't add the thread_local variable if it was already added...
                //

                int exists = 0;

                for (uint32_t i = 0; i < cpp_module->member_count; i++)
                {
                    struct cpp_module_member *member = &cpp_module->members[i];

                    if (member->type != CPP_MODULE_MEMBER_TYPE_THREAD_LOCAL)
                        continue;
                    
                    if (strcmp(result, member->thread_local_.string) == 0 &&
                        address == member->thread_local_.address &&
                        line == member->thread_local_.line)
                    {
                        exists = 1;
                        break;
                    }
                }

                if (exists)
                {
                    free(result);
                    break;
                }

                struct cpp_module_member member = {
                    .type = CPP_MODULE_MEMBER_TYPE_THREAD_LOCAL,
                    .thread_local_ = {
                        .string = result,
                        .address = address,
                        .line = line,
                    }
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
    for (uint32_t i = 0; i < main_globals.cpp_module_count; i++)
    {
        struct cpp_module *module = &main_globals.cpp_modules[i];

        create_file_path_dirs(module->path);

        FILE *module_stream = fopen(module->path, "a");
        cpp_module_print(module, module_stream);
        fclose(module_stream);
    }
}
