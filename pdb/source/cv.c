#include <stdlib.h>
#include <string.h>
#include "cv.h"
#include "utils.h"
#include "macros_print.h"

void cv_signature_type_print(enum cv_signature_type item, FILE *stream)
{
    assert(stream);

    DBI_CV_SIGNATURE_ENUM
}

void cv_cpu_type_print(enum cv_cpu_type item, FILE *stream)
{
    assert(stream);

    CV_CPU_TYPE_ENUM
}

void cv_symbol_type_print(enum cv_symbol_type item, FILE *stream)
{
    assert(stream);

    CV_SYMBOL_TYPE_ENUM
}

void cv_pe_section_offset_print(struct cv_pe_section_offset *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_PE_SECTION_OFFSET_STRUCT
}

void cv_pe_section_offset_read(struct cv_pe_section_offset *item, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->memory_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->section_index, file_stream);
}

void cv_obj_name_symbol_dispose(struct cv_obj_name_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_obj_name_symbol_print(struct cv_obj_name_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_OBJ_NAME_SYMBOL_STRUCT
}

void cv_obj_name_symbol_read(
    struct cv_obj_name_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->signature, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_register_variable_symbol_dispose(struct cv_register_variable_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_register_variable_symbol_print(struct cv_register_variable_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_REGISTER_VARIABLE_SYMBOL_STRUCT
}

void cv_register_variable_symbol_read(
    struct cv_register_variable_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_index, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_constant_symbol_dispose(struct cv_constant_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_constant_symbol_print(struct cv_constant_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_CONSTANT_SYMBOL_STRUCT
}

void cv_constant_symbol_read(
    struct cv_constant_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    tpi_enumerate_variant_read(&item->value, msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_user_defined_type_symbol_dispose(struct cv_user_defined_type_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_user_defined_type_symbol_print(struct cv_user_defined_type_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_USER_DEFINED_TYPE_SYMBOL_STRUCT
}

void cv_user_defined_type_symbol_read(
    struct cv_user_defined_type_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_multi_register_entry_dispose(struct cv_multi_register_entry *item)
{
    assert(item);
    
    free(item->register_name);
}

void cv_multi_register_entry_print(struct cv_multi_register_entry *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_MULTI_REGISTER_ENTRY_STRUCT
}

void cv_multi_register_entry_read(
    struct cv_multi_register_entry *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_index, file_stream);
    item->register_name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_multi_register_variable_symbol_dispose(struct cv_multi_register_variable_symbol *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->register_count; i++)
        cv_multi_register_entry_dispose(&item->registers[i]);
    
    free(item->registers);
}

void cv_multi_register_variable_symbol_print(struct cv_multi_register_variable_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_MULTI_REGISTER_VARIABLE_SYMBOL_STRUCT
}

void cv_multi_register_variable_symbol_read(
    struct cv_multi_register_variable_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);

    if (symbol_type == S_MANYREG2 || symbol_type == S_MANYREG2_ST)
    {
        uint16_t count = 0;
        MSF_STREAM_READ(msf, msf_stream, out_offset, count, file_stream);
        item->register_count = count;
    }
    else
    {
        uint8_t count = 0;
        MSF_STREAM_READ(msf, msf_stream, out_offset, count, file_stream);
        item->register_count = count;
    }

    for (uint32_t i = 0; i < item->register_count; i++)
    {
        struct cv_multi_register_entry entry;
        cv_multi_register_entry_read(&entry, symbol_type, msf, msf_stream, out_offset, file_stream);
        DYNARRAY_PUSH(item->registers, item->register_count, entry);
    }
}

void cv_data_symbol_dispose(struct cv_data_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_data_symbol_print(struct cv_data_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DATA_SYMBOL_STRUCT
}

void cv_data_symbol_read(
    struct cv_data_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_public_symbol_dispose(struct cv_public_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_public_symbol_print(struct cv_public_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_PUBLIC_SYMBOL_STRUCT
}

void cv_public_symbol_read(
    struct cv_public_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_procedure_symbol_dispose(struct cv_procedure_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_procedure_symbol_print(struct cv_procedure_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_PROCEDURE_SYMBOL_STRUCT
}

void cv_procedure_symbol_read(
    struct cv_procedure_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->end_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->next_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->code_block_length, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->debug_start_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->debug_end_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_thread_storage_symbol_dispose(struct cv_thread_storage_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_thread_storage_symbol_print(struct cv_thread_storage_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_THREAD_STORAGE_SYMBOL_STRUCT
}

void cv_thread_storage_symbol_read(
    struct cv_thread_storage_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_source_language_print(enum cv_source_language item, FILE *stream)
{
    assert(stream);

    CV_SOURCE_LANGUAGE_ENUM
}

void cv_compiler_version_print(struct cv_compiler_version *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_COMPILER_VERSION_STRUCT
}

void cv_compiler_version_read(
    struct cv_compiler_version *item,
    uint32_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->major, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->minor, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->build, file_stream);

    if (symbol_type == S_COMPILE3)
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->qfe, file_stream);
}

void cv_compile_flags_print(struct cv_compile_flags *item, uint32_t depth, FILE *stream)
{
    assert(stream);

    CV_COMPILE_FLAGS_STRUCT
}

void cv_compile_flags_symbol_dispose(struct cv_compile_flags_symbol *item)
{
    assert(item);

    free(item->version_string);

    for (uint32_t i = 0; i < item->string_count; i++)
        free(item->strings[i]);
    
    free(item->strings);
}

void cv_compile_flags_symbol_print(struct cv_compile_flags_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_COMPILE_FLAGS_SYMBOL_STRUCT
}

void cv_compile_flags_symbol_read(
    struct cv_compile_flags_symbol *item,
    uint16_t symbol_type,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->cpu_type, file_stream);
    cv_compiler_version_read(&item->frontend_version, symbol_type, msf, msf_stream, out_offset, file_stream);
    cv_compiler_version_read(&item->backend_version, symbol_type, msf, msf_stream, out_offset, file_stream);
    item->version_string = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
    
    item->string_count = 0;
    item->strings = NULL;

    if (symbol_type == S_COMPILE2)
    {
        //
        // NOTE:
        // S_COMPILE2 symbols have an optional block of zero terminated strings.
        // This block is terminated with a double zero.
        //

        while (*out_offset < start_offset + symbol_size)
        {
            uint8_t byte = 0;
            MSF_STREAM_READ(msf, msf_stream, out_offset, byte, file_stream);

            if (byte == 0)
            {
                MSF_STREAM_READ(msf, msf_stream, out_offset, byte, file_stream);

                if (byte == 0)
                    break;
                else
                    *out_offset -= 2;
            }
            else
            {
                *out_offset -= 1;
            }

            char *string = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
            assert(string);

            DYNARRAY_PUSH(item->strings, item->string_count, string);
        }
    }
    
    // HACK: idk why there can be extra zero bytes after, but we're gonna skip 'em...
    while (*out_offset < start_offset + symbol_size)
    {
        uint8_t byte = 0;
        MSF_STREAM_READ(msf, msf_stream, out_offset, byte, file_stream);
        if (byte)
            printf("file position: %li\n", ftell(file_stream) - 1);
        assert(byte == 0);
    }
}

void cv_using_namespace_symbol_dispose(struct cv_using_namespace_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_using_namespace_symbol_print(struct cv_using_namespace_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_USING_NAMESPACE_SYMBOL_STRUCT
}

void cv_using_namespace_symbol_read(
    struct cv_using_namespace_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_procedure_reference_symbol_dispose(struct cv_procedure_reference_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_procedure_reference_symbol_print(struct cv_procedure_reference_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_PROCEDURE_REFERENCE_SYMBOL_STRUCT
}

void cv_procedure_reference_symbol_read(
    struct cv_procedure_reference_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->sum_name, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->module_index, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_data_reference_symbol_dispose(struct cv_data_reference_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_data_reference_symbol_print(struct cv_data_reference_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DATA_REFERENCE_SYMBOL_STRUCT
}

void cv_data_reference_symbol_read(
    struct cv_data_reference_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->sum_name, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->module_index, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_annotation_reference_symbol_dispose(struct cv_annotation_reference_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_annotation_reference_symbol_print(struct cv_annotation_reference_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ANNOTATION_REFERENCE_SYMBOL_STRUCT
}

void cv_annotation_reference_symbol_read(
    struct cv_annotation_reference_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->sum_name, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->module_index, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_trampoline_type_print(enum cv_trampoline_type item, FILE *stream)
{
    assert(stream);

    CV_TRAMPOLINE_TYPE_ENUM
}

void cv_trampoline_symbol_print(struct cv_trampoline_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_TRAMPOLINE_SYMBOL_STRUCT
}

void cv_trampoline_symbol_read(
    struct cv_trampoline_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->size, file_stream);
    cv_pe_section_offset_read(&item->thunk_offset, msf, msf_stream, out_offset, file_stream);
    cv_pe_section_offset_read(&item->target_offset, msf, msf_stream, out_offset, file_stream);
}

void cv_export_symbol_dispose(struct cv_export_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_export_symbol_print(struct cv_export_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_EXPORT_SYMBOL_STRUCT
}

void cv_export_symbol_read(
    struct cv_export_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->ordinal, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_local_symbol_dispose(struct cv_local_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_local_symbol_print(struct cv_local_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_LOCAL_SYMBOL_STRUCT
}

void cv_local_symbol_read(
    struct cv_local_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_build_info_symbol_print(struct cv_build_info_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_BUILD_INFO_SYMBOL_STRUCT
}

void cv_build_info_symbol_read(
    struct cv_build_info_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->id_index, file_stream);
}

void cv_annotation_type_print(enum cv_annotation_type item, FILE *stream)
{
    assert(stream);

    CV_ANNOTATION_TYPE_ENUM
}

void cv_annotation_code_and_line_offsets_print(struct cv_annotation_code_and_line_offsets *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ANNOTATION_CODE_AND_LINE_OFFSETS_STRUCT
}

void cv_annotation_code_length_and_offset_print(struct cv_annotation_code_length_and_offset *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ANNOTATION_CODE_LENGTH_AND_OFFSET_STRUCT
}

void cv_annotation_print(struct cv_annotation *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ANNOTATION_STRUCT
}

void cv_annotation_read(
    struct cv_annotation *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    item->type = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);

    if (item->type == CV_ANNOTATION_TYPE_EOF)
        return;

    switch (item->type)
    {
    case CV_ANNOTATION_TYPE_CODE_OFFSET:
        item->code_offset = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_CODE_OFFSET_BASE:
        item->change_code_offset_base = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_CODE_OFFSET:
        item->change_code_offset = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_CODE_LENGTH:
        item->change_code_length = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_FILE:
        item->change_file = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_LINE_OFFSET:
    {
        uint32_t value = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        item->change_line_offset = ((value & 1) != 0) ? -(int32_t)(value >> 1) : (int32_t)(value >> 1);
        break;
    }

    case CV_ANNOTATION_TYPE_CHANGE_LINE_END_DELTA:
        item->change_line_end_delta = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_RANGE_KIND:
        item->change_range_kind = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_COLUMN_START:
        item->change_column_start = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_COLUMN_END_DELTA:
    {
        uint32_t value = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        item->change_column_end_delta = ((value & 1) != 0) ? -(int32_t)(value >> 1) : (int32_t)(value >> 1);
        break;
    }

    case CV_ANNOTATION_TYPE_CHANGE_CODE_AND_LINE_OFFSETS:
    {
        uint32_t value = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        item->change_code_and_line_offsets.code_offset = value & 0xf;
        value >>= 4;
        item->change_code_and_line_offsets.line_offset = ((value & 1) != 0) ? -(int32_t)(value >> 1) : (int32_t)(value >> 1);
        break;
    }

    case CV_ANNOTATION_TYPE_CHANGE_CODE_LENGTH_AND_CODE_OFFSET:
        item->change_code_length_and_code_offset.code_length = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        item->change_code_length_and_code_offset.code_offset = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;

    case CV_ANNOTATION_TYPE_CHANGE_COLUMN_END:
        item->change_column_end = msf_stream_read_compressed_unsigned(msf, msf_stream, out_offset, file_stream);
        break;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled cv_annotation_type value: ", __FILE__, __LINE__);
        cv_annotation_type_print(item->type, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
}

void cv_annotations_dispose(struct cv_annotations *item)
{
    assert(item);

    free(item->annotations);
}

void cv_annotations_print(struct cv_annotations *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ANNOTATIONS_STRUCT
}

void cv_annotations_read(
    struct cv_annotations *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint32_t size,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    item->count = 0;
    item->annotations = NULL;

    uint32_t start_offset = *out_offset;

    while (*out_offset < start_offset + size)
    {
        struct cv_annotation annotation;
        cv_annotation_read(&annotation, msf, msf_stream, out_offset, file_stream);

        DYNARRAY_PUSH(
            item->annotations,
            item->count,
            annotation);
    }
}

void cv_inline_site_symbol_dispose(struct cv_inline_site_symbol *item)
{
    assert(item);

    cv_annotations_dispose(&item->annotations);
}

void cv_inline_site_symbol_print(struct cv_inline_site_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_INLINE_SITE_SYMBOL_STRUCT
}

void cv_inline_site_symbol_read(
    struct cv_inline_site_symbol *item,
    uint16_t symbol_type,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->end_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->inlinee_id_index, file_stream);

    if (symbol_type == S_INLINESITE2)
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->invocation_count, file_stream);
    
    cv_annotations_read(&item->annotations, msf, msf_stream, out_offset, symbol_size - (*out_offset - start_offset), file_stream);
}

void cv_label_symbol_dispose(struct cv_label_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_label_symbol_print(struct cv_label_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_LABEL_SYMBOL_STRUCT
}

void cv_label_symbol_read(
    struct cv_label_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    cv_pe_section_offset_read(&item->section_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_block_symbol_dispose(struct cv_block_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_block_symbol_print(struct cv_block_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_BLOCK_SYMBOL_STRUCT
}

void cv_block_symbol_read(
    struct cv_block_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->end_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->length, file_stream);
    cv_pe_section_offset_read(&item->section_offset, msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_register_relative_symbol_dispose(struct cv_register_relative_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_register_relative_symbol_print(struct cv_register_relative_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_REGISTER_RELATIVE_SYMBOL_STRUCT
}

void cv_register_relative_symbol_read(
    struct cv_register_relative_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_index, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_thunk_symbol_dispose(struct cv_thunk_symbol *item)
{
    assert(item);
    
    free(item->name);
}

void cv_thunk_symbol_print(struct cv_thunk_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_THUNK_SYMBOL_STRUCT
}

void cv_thunk_symbol_read(
    struct cv_thunk_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->end_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->next_symbol_index, file_stream);
    cv_pe_section_offset_read(&item->section_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->length, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->ordinal, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_separated_code_symbol_print(struct cv_separated_code_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_SEPARATED_CODE_SYMBOL_STRUCT
}

void cv_separated_code_symbol_read(
    struct cv_separated_code_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->end_symbol_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->length, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    cv_pe_section_offset_read(&item->section_offset, msf, msf_stream, out_offset, file_stream);
    cv_pe_section_offset_read(&item->parent_section_offset, msf, msf_stream, out_offset, file_stream);
}

void cv_cookie_type_print(enum cv_cookie_type item, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_COOKIE_TYPE_ENUM
}

void cv_frame_cookie_symbol_print(struct cv_frame_cookie_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_FRAME_COOKIE_SYMBOL_STRUCT
}

void cv_frame_cookie_symbol_read(
    struct cv_frame_cookie_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->frame_relative_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->cookie_type, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
}

void cv_frame_proc_flags_print(struct cv_frame_proc_flags *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_FRAME_PROC_FLAGS_STRUCT
}

void cv_frame_proc_symbol_print(struct cv_frame_proc_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_FRAME_PROC_SYMBOL_STRUCT
}

void cv_frame_proc_symbol_read(
    struct cv_frame_proc_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->frame_size, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->padding_size, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->padding_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->callee_save_registers_size, file_stream);
    cv_pe_section_offset_read(&item->exception_handler_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
}

void cv_call_site_info_symbol_print(struct cv_call_site_info_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_CALL_SITE_INFO_SYMBOL_STRUCT
}

void cv_call_site_info_symbol_read(
    struct cv_call_site_info_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->padding, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
}

void cv_env_block_flags_print(struct cv_env_block_flags *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ENV_BLOCK_FLAGS_STRUCT
}

void cv_env_block_symbol_dispose(struct cv_env_block_symbol *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->string_count; i++)
        free(item->strings[i]);
    
    free(item->strings);
}

void cv_env_block_symbol_print(struct cv_env_block_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);

    CV_ENV_BLOCK_SYMBOL_STRUCT
}

void cv_env_block_symbol_read(
    struct cv_env_block_symbol *item,
    uint16_t symbol_type,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);

    while (*out_offset < start_offset + symbol_size)
    {
        char *string = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);

        if (strlen(string) == 0)
        {
            free(string);
            break;
        }

        DYNARRAY_PUSH(item->strings, item->string_count, string);
    }

}

void cv_local_variable_flags_print(struct cv_local_variable_flags *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_LOCAL_VARIABLE_FLAGS_STRUCT
}

void cv_file_static_symbol_print(struct cv_file_static_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_FILE_STATIC_SYMBOL_STRUCT
}

void cv_file_static_symbol_read(
    struct cv_file_static_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->module_filename_string_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_section_symbol_dispose(struct cv_section_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_section_symbol_print(struct cv_section_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_SECTION_SYMBOL_STRUCT
}

void cv_section_symbol_read(
    struct cv_section_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->isec, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->align, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->bReserved, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->rva, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->cb, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->characteristics, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_coff_group_symbol_dispose(struct cv_coff_group_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_coff_group_symbol_print(struct cv_coff_group_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_COFF_GROUP_SYMBOL_STRUCT
}

void cv_coff_group_symbol_read(
    struct cv_coff_group_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->cb, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->characteristics, file_stream);
    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
}

void cv_annotation_symbol_dispose(struct cv_annotation_symbol *item)
{
    assert(item);

    for (uint16_t i = 0; i < item->string_count; i++)
        free(item->strings[i]);
    
    free(item->strings);
}

void cv_annotation_symbol_print(struct cv_annotation_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ANNOTATION_SYMBOL_STRUCT
}

void cv_annotation_symbol_read(
    struct cv_annotation_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    cv_pe_section_offset_read(&item->code_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->string_count, file_stream);
    
    item->strings = malloc(item->string_count * sizeof(char *));
    assert(item->strings);

    for (uint16_t i = 0; i < item->string_count; i++)
    {
        uint32_t length = 0;
        item->strings[i] = msf_stream_read_cstring(msf, msf_stream, *out_offset, &length, file_stream);
        assert(item->strings[i]);
        *out_offset += length;
    }
}

void cv_address_range_print(struct cv_address_range *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ADDRESS_RANGE_STRUCT
}

void cv_address_range_read(
    struct cv_address_range *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    cv_pe_section_offset_read(&item->start_offset, msf, msf_stream, out_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->length, file_stream);
}

void cv_address_gap_print(struct cv_address_gap *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_ADDRESS_GAP_STRUCT
}

void cv_def_range_symbol_dispose(struct cv_def_range_symbol *item)
{
    assert(item);

    free(item->gaps);
}

void cv_def_range_symbol_print(struct cv_def_range_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_SYMBOL_STRUCT
}

void cv_def_range_symbol_read(
    struct cv_def_range_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->program_index, file_stream);
    cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);

    while (*out_offset < start_offset + symbol_size)
    {
        struct cv_address_gap gap;
        MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
        DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
    }
}

void cv_def_range_subfield_symbol_dispose(struct cv_def_range_subfield_symbol *item)
{
    assert(item);

    free(item->gaps);
}

void cv_def_range_subfield_symbol_print(struct cv_def_range_subfield_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_SUBFIELD_SYMBOL_STRUCT
}

void cv_def_range_subfield_symbol_read(
    struct cv_def_range_subfield_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->program_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_offset, file_stream);
    cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);

    while (*out_offset < start_offset + symbol_size)
    {
        struct cv_address_gap gap;
        MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
        DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
    }
}

void cv_range_attributes_print(struct cv_range_attributes *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_RANGE_ATTRIBUTES_STRUCT
}

void cv_def_range_register_symbol_dispose(struct cv_def_range_register_symbol *item)
{
    assert(item);

    free(item->gaps);
}

void cv_def_range_register_symbol_print(struct cv_def_range_register_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_REGISTER_SYMBOL_STRUCT
}

void cv_def_range_register_symbol_read(
    struct cv_def_range_register_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->attributes, file_stream);
    cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);

    while (*out_offset < start_offset + symbol_size)
    {
        struct cv_address_gap gap;
        MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
        DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
    }
}

void cv_def_range_frame_pointer_rel_symbol_dispose(struct cv_def_range_frame_pointer_rel_symbol *item)
{
    assert(item);

    free(item->gaps);
}

void cv_def_range_frame_pointer_rel_symbol_print(struct cv_def_range_frame_pointer_rel_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_FRAME_POINTER_REL_SYMBOL_STRUCT
}

void cv_def_range_frame_pointer_rel_symbol_read(
    struct cv_def_range_frame_pointer_rel_symbol *item,
    uint16_t symbol_type,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->offset, file_stream);

    if (symbol_type == S_DEFRANGE_FRAMEPOINTER_REL)
    {
        cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);

        while (*out_offset < start_offset + symbol_size)
        {
            struct cv_address_gap gap;
            MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
            DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
        }
    }
}

void cv_def_range_subfield_register_packed_data_print(struct cv_def_range_subfield_register_packed_data *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_SUBFIELD_REGISTER_PACKED_DATA_STRUCT
}

void cv_def_range_subfield_register_symbol_dispose(struct cv_def_range_subfield_register_symbol *item)
{
    assert(item);

    free(item->gaps);
}

void cv_def_range_subfield_register_symbol_print(struct cv_def_range_subfield_register_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_SUBFIELD_REGISTER_SYMBOL_STRUCT
}

void cv_def_range_subfield_register_symbol_read(
    struct cv_def_range_subfield_register_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->attributes, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->packed_data, file_stream);
    cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);

    while (*out_offset < start_offset + symbol_size)
    {
        struct cv_address_gap gap;
        MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
        DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
    }
}

void cv_def_range_register_rel_packed_data_print(struct cv_def_range_register_rel_packed_data *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_REGISTER_REL_PACKED_DATA_STRUCT
}

void cv_def_range_register_rel_symbol_dispose(struct cv_def_range_register_rel_symbol *item)
{
    assert(item);

    free(item->gaps);
}

void cv_def_range_register_rel_symbol_print(struct cv_def_range_register_rel_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_REGISTER_REL_SYMBOL_STRUCT
}

void cv_def_range_register_rel_symbol_read(
    struct cv_def_range_register_rel_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->base_register_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->packed_data, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->base_offset, file_stream);
    cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);
    
    while (*out_offset < start_offset + symbol_size)
    {
        struct cv_address_gap gap;
        MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
        DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
    }
}

void cv_hlsl_register_type_print(enum cv_hlsl_register_type item, FILE *stream)
{
    assert(stream);

    CV_HLSL_REGISTER_TYPE_ENUM
}

void cv_def_range_hlsl_packed_data_print(struct cv_def_range_hlsl_packed_data *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_HLSL_PACKED_DATA_STRUCT
}

void cv_def_range_hlsl_symbol_dispose(struct cv_def_range_hlsl_symbol *item)
{
    assert(item);

    free(item->gaps);
    free(item->register_indices);
}

void cv_def_range_hlsl_symbol_print(struct cv_def_range_hlsl_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DEF_RANGE_HLSL_SYMBOL_STRUCT
}

void cv_def_range_hlsl_symbol_read(
    struct cv_def_range_hlsl_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->register_type, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->packed_data, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_offset, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->parent_size, file_stream);
    cv_address_range_read(&item->range, msf, msf_stream, out_offset, file_stream);

    uint32_t size_remaining = symbol_size - (*out_offset - start_offset);

    uint32_t register_indices_size = item->packed_data.register_index_count * sizeof(uint32_t);
    assert(register_indices_size <= size_remaining);

    uint32_t gaps_offset = *out_offset;
    uint32_t gaps_size = size_remaining - register_indices_size;

    while (*out_offset < gaps_offset + gaps_size)
    {
        struct cv_address_gap gap;
        MSF_STREAM_READ(msf, msf_stream, out_offset, gap, file_stream);
        DYNARRAY_PUSH(item->gaps, item->gap_count, gap);
    }

    item->register_indices = malloc(register_indices_size);
    assert(item->register_indices);
    
    msf_stream_read_data(msf, msf_stream, *out_offset, register_indices_size, item->register_indices, file_stream);
    *out_offset += register_indices_size;

    assert(*out_offset = start_offset + symbol_size);
}

void cv_local_dpc_group_shared_symbol_dispose(struct cv_local_dpc_group_shared_symbol *item)
{
    assert(item);

    free(item->name);
}

void cv_local_dpc_group_shared_symbol_print(struct cv_local_dpc_group_shared_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_LOCAL_DPC_GROUP_SHARED_SYMBOL_STRUCT
}

void cv_local_dpc_group_shared_symbol_read(
    struct cv_local_dpc_group_shared_symbol *item,
    uint16_t symbol_type,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_index, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->flags, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->data_slot, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->data_offset, file_stream);
    item->name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol_type, file_stream);
    assert(item->name);
}

void cv_dpc_symbol_tag_map_entry_print(struct cv_dpc_symbol_tag_map_entry *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DPC_SYMBOL_TAG_MAP_ENTRY_STRUCT
}

void cv_dpc_symbol_tag_map_symbol_dispose(struct cv_dpc_symbol_tag_map_symbol *item)
{
    assert(item);

    free(item->entries);
}

void cv_dpc_symbol_tag_map_symbol_print(struct cv_dpc_symbol_tag_map_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_DPC_SYMBOL_TAG_MAP_SYMBOL_STRUCT
}

void cv_dpc_symbol_tag_map_symbol_read(
    struct cv_dpc_symbol_tag_map_symbol *item,
    uint32_t symbol_size,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    item->entry_count = symbol_size / sizeof(*item->entries);

    if (item->entry_count == 0)
        return;
    
    uint32_t entries_size = item->entry_count * sizeof(*item->entries);
    
    item->entries = malloc(entries_size * sizeof(*item->entries));
    assert(item->entries);

    msf_stream_read_data(msf, msf_stream, *out_offset, entries_size, item->entries, file_stream);
    *out_offset += entries_size;
}

void cv_function_list_symbol_dispose(struct cv_function_list_symbol *item)
{
    assert(item);

    free(item->type_indices);
}

void cv_function_list_symbol_print(struct cv_function_list_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_FUNCTION_LIST_SYMBOL_STRUCT
}

void cv_function_list_symbol_read(
    struct cv_function_list_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->count, file_stream);
    
    item->type_indices = malloc(item->count * sizeof(uint32_t));
    assert(item->type_indices);

    for (uint32_t i = 0; i < item->count; i++)
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_indices[i], file_stream);
}

void cv_pogo_data_symbol_print(struct cv_pogo_data_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_POGO_DATA_SYMBOL_STRUCT
}

void cv_pogo_data_symbol_read(
    struct cv_pogo_data_symbol *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->invocations, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->dynamic_instruction_count, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->static_instruction_count, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->final_static_instruction_count, file_stream);
}

void cv_symbol_dispose(struct cv_symbol *item)
{
    assert(item);

    switch (item->type)
    {
    case S_END:
        break;

    case S_OBJNAME:
    case S_OBJNAME_ST:
        cv_obj_name_symbol_dispose(&item->obj_name);
        break;

    case S_REGISTER:
    case S_REGISTER_ST:
        cv_register_variable_symbol_dispose(&item->register_variable);
        break;

    case S_CONSTANT:
    case S_CONSTANT_ST:
    case S_MANCONSTANT:
        cv_constant_symbol_dispose(&item->constant);
        break;

    case S_UDT:
    case S_UDT_ST:
    case S_COBOLUDT:
    case S_COBOLUDT_ST:
        cv_user_defined_type_symbol_dispose(&item->user_defined_type);
        break;

    case S_MANYREG:
    case S_MANYREG_ST:
    case S_MANYREG2:
    case S_MANYREG2_ST:
        cv_multi_register_variable_symbol_dispose(&item->multi_register_variable);
        break;

    case S_LDATA32:
    case S_LDATA32_ST:
    case S_GDATA32:
    case S_GDATA32_ST:
    case S_LMANDATA:
    case S_LMANDATA_ST:
    case S_GMANDATA:
    case S_GMANDATA_ST:
        cv_data_symbol_dispose(&item->data);
        break;

    case S_PUB32:
    case S_PUB32_ST:
        cv_public_symbol_dispose(&item->public_);
        break;

    case S_LPROC32:
    case S_LPROC32_ST:
    case S_GPROC32:
    case S_GPROC32_ST:
    case S_LPROC32_ID:
    case S_GPROC32_ID:
    case S_LPROC32_DPC:
    case S_LPROC32_DPC_ID:
        cv_procedure_symbol_dispose(&item->procedure);
        break;

    case S_LTHREAD32:
    case S_LTHREAD32_ST:
    case S_GTHREAD32:
    case S_GTHREAD32_ST:
        cv_thread_storage_symbol_dispose(&item->thread_storage);
        break;

    case S_COMPILE2:
    case S_COMPILE2_ST:
    case S_COMPILE3:
        cv_compile_flags_symbol_dispose(&item->compile_flags);
        break;

    case S_UNAMESPACE:
    case S_UNAMESPACE_ST:
        cv_using_namespace_symbol_dispose(&item->using_namespace);
        break;

    case S_PROCREF:
    case S_PROCREF_ST:
    case S_LPROCREF:
    case S_LPROCREF_ST:
        cv_procedure_reference_symbol_dispose(&item->procedure_reference);
        break;

    case S_DATAREF:
    case S_DATAREF_ST:
        cv_data_reference_symbol_dispose(&item->data_reference);
        break;

    case S_ANNOTATIONREF:
        cv_annotation_reference_symbol_dispose(&item->annotation_reference);
        break;

    case S_TRAMPOLINE:
        break;

    case S_EXPORT:
        cv_export_symbol_dispose(&item->export);
        break;

    case S_LOCAL:
        cv_local_symbol_dispose(&item->local);
        break;

    case S_BUILDINFO:
        break;

    case S_INLINESITE:
    case S_INLINESITE2:
        cv_inline_site_symbol_dispose(&item->inline_site);
        break;

    case S_INLINESITE_END:
        break;

    case S_PROC_ID_END:
        break;

    case S_LABEL32:
    case S_LABEL32_ST:
        cv_label_symbol_dispose(&item->label);
        break;

    case S_BLOCK32:
    case S_BLOCK32_ST:
        cv_block_symbol_dispose(&item->block);
        break;

    case S_REGREL32:
        cv_register_relative_symbol_dispose(&item->register_relative);
        break;

    case S_THUNK32:
    case S_THUNK32_ST:
        cv_thunk_symbol_dispose(&item->thunk);
        break;

    case S_SEPCODE:
        break;

    case S_FRAMECOOKIE:
        break;

    case S_FRAMEPROC:
        break;

    case S_CALLSITEINFO:
        break;

    case S_ENVBLOCK:
        cv_env_block_symbol_dispose(&item->env_block);
        break;

    case S_FILESTATIC:
        break;

    case S_SECTION:
        cv_section_symbol_dispose(&item->section);
        break;

    case S_COFFGROUP:
        cv_coff_group_symbol_dispose(&item->coff_group);
        break;
    
    case S_ANNOTATION:
        cv_annotation_symbol_dispose(&item->annotation);
        break;
    
    case S_DEFRANGE:
        cv_def_range_symbol_dispose(&item->def_range);
        break;
    
    case S_DEFRANGE_SUBFIELD:
        cv_def_range_subfield_symbol_dispose(&item->def_range_subfield);
        break;
    
    case S_DEFRANGE_REGISTER:
        cv_def_range_register_symbol_dispose(&item->def_range_register);
        break;
    
    case S_DEFRANGE_FRAMEPOINTER_REL:
    case S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE:
        cv_def_range_frame_pointer_rel_symbol_dispose(&item->def_range_frame_pointer_rel);
        break;

    case S_DEFRANGE_SUBFIELD_REGISTER:
        cv_def_range_subfield_register_symbol_dispose(&item->def_range_subfield_register);
        break;
    
    case S_DEFRANGE_REGISTER_REL:
        cv_def_range_register_rel_symbol_dispose(&item->def_range_register_rel);
        break;
    
    case S_DEFRANGE_HLSL:
    case S_DEFRANGE_DPC_PTR_TAG:
        cv_def_range_hlsl_symbol_dispose(&item->def_range_hlsl);
        break;
    
    case S_LOCAL_DPC_GROUPSHARED:
        cv_local_dpc_group_shared_symbol_dispose(&item->local_dpc_group_shared);
        break;
    
    case S_DPC_SYM_TAG_MAP:
        cv_dpc_symbol_tag_map_symbol_dispose(&item->dpc_symbol_tag_map);
        break;
    
    case S_CALLERS:
    case S_CALLEES:
        cv_function_list_symbol_dispose(&item->function_list);
        break;
    
    case S_POGODATA:
        break;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type value: ", __FILE__, __LINE__);
        cv_symbol_type_print(item->type, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
}

void cv_symbol_print(struct cv_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_SYMBOL_STRUCT
}

void cv_symbols_dispose(struct cv_symbols *item)
{
    assert(item);
    
    for (uint32_t i = 0; i < item->count; i++)
        cv_symbol_dispose(&item->symbols[i]);
       
    free(item->symbols);
}

void cv_symbols_print(struct cv_symbols *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_SYMBOLS_STRUCT
}

void cv_symbols_read(
    struct cv_symbols *item,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t symbols_size,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(symbols_size);
    assert(out_offset);
    assert(file_stream);

    memset(item, 0, sizeof(*item));

    //
    // Collect info about where each CV symbol is located
    //

    struct cv_symbol_info
    {
        uint32_t offset;
        uint16_t size;
    };

    uint32_t symbol_info_count = 0;
    struct cv_symbol_info *symbol_info = NULL;

    while (*out_offset < symbols_size)
    {
        struct cv_symbol_info info;
        
        MSF_STREAM_READ(msf, msf_stream, out_offset, info.size, file_stream);
        assert(info.size >= sizeof(uint16_t));

        info.offset = *out_offset;

        DYNARRAY_PUSH(symbol_info, symbol_info_count, info);

        *out_offset += info.size;
    }

    //
    // Pre-allocate all of the CV symbols
    //

    item->count = symbol_info_count;
    item->symbols = calloc(item->count, sizeof(*item->symbols));
    assert(item->symbols);

    //
    // Read each CV symbol
    //

    for (uint32_t symbol_index = 0; symbol_index < symbol_info_count; symbol_index++)
    {
        *out_offset = symbol_info[symbol_index].offset;
        uint32_t start_offset = *out_offset;

        struct cv_symbol *symbol = &item->symbols[symbol_index];
        symbol->size = symbol_info[symbol_index].size;

        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol->type, file_stream);

        uint32_t size_remaining = symbol->size - (*out_offset - start_offset);

        switch (symbol->type)
        {
        case S_END:
            break;
        
        case S_OBJNAME:
        case S_OBJNAME_ST:
            cv_obj_name_symbol_read(&symbol->obj_name, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_REGISTER:
        case S_REGISTER_ST:
            cv_register_variable_symbol_read(&symbol->register_variable, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_CONSTANT:
        case S_CONSTANT_ST:
        case S_MANCONSTANT:
            cv_constant_symbol_read(&symbol->constant, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_UDT:
        case S_UDT_ST:
        case S_COBOLUDT:
        case S_COBOLUDT_ST:
            cv_user_defined_type_symbol_read(&symbol->user_defined_type, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_MANYREG:
        case S_MANYREG_ST:
        case S_MANYREG2:
        case S_MANYREG2_ST:
            cv_multi_register_variable_symbol_read(&symbol->multi_register_variable, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_LDATA32:
        case S_LDATA32_ST:
        case S_GDATA32:
        case S_GDATA32_ST:
        case S_LMANDATA:
        case S_LMANDATA_ST:
        case S_GMANDATA:
        case S_GMANDATA_ST:
            cv_data_symbol_read(&symbol->data, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_PUB32:
        case S_PUB32_ST:
            cv_public_symbol_read(&symbol->public_, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_LPROC32:
        case S_LPROC32_ST:
        case S_GPROC32:
        case S_GPROC32_ST:
        case S_LPROC32_ID:
        case S_GPROC32_ID:
        case S_LPROC32_DPC:
        case S_LPROC32_DPC_ID:
            cv_procedure_symbol_read(&symbol->procedure, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
            cv_thread_storage_symbol_read(&symbol->thread_storage, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_COMPILE2:
        case S_COMPILE2_ST:
        case S_COMPILE3:
            cv_compile_flags_symbol_read(&symbol->compile_flags, symbol->type, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;

        case S_UNAMESPACE:
        case S_UNAMESPACE_ST:
            cv_using_namespace_symbol_read(&symbol->using_namespace, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_PROCREF:
        case S_PROCREF_ST:
        case S_LPROCREF:
        case S_LPROCREF_ST:
            cv_procedure_reference_symbol_read(&symbol->procedure_reference, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_DATAREF:
        case S_DATAREF_ST:
            cv_data_reference_symbol_read(&symbol->data_reference, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_ANNOTATIONREF:
            cv_annotation_reference_symbol_read(&symbol->annotation_reference, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_TRAMPOLINE:
            cv_trampoline_symbol_read(&symbol->trampoline, msf, msf_stream, out_offset, file_stream);
            break;

        case S_EXPORT:
            cv_export_symbol_read(&symbol->export, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_LOCAL:
            cv_local_symbol_read(&symbol->local, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_BUILDINFO:
            cv_build_info_symbol_read(&symbol->build_info, msf, msf_stream, out_offset, file_stream);
            break;

        case S_INLINESITE:
        case S_INLINESITE2:
            cv_inline_site_symbol_read(&symbol->inline_site, symbol->type, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;

        case S_INLINESITE_END:
            break;
        
        case S_PROC_ID_END:
            break;

        case S_LABEL32:
        case S_LABEL32_ST:
            cv_label_symbol_read(&symbol->label, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_BLOCK32:
        case S_BLOCK32_ST:
            cv_block_symbol_read(&symbol->block, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_REGREL32:
            cv_register_relative_symbol_read(&symbol->register_relative, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_THUNK32:
        case S_THUNK32_ST:
            cv_thunk_symbol_read(&symbol->thunk, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;

        case S_SEPCODE:
            cv_separated_code_symbol_read(&symbol->separated_code, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_FRAMECOOKIE:
            cv_frame_cookie_symbol_read(&symbol->frame_cookie, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_FRAMEPROC:
            cv_frame_proc_symbol_read(&symbol->frame_proc, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_CALLSITEINFO:
            cv_call_site_info_symbol_read(&symbol->call_site_info, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_ENVBLOCK:
            cv_env_block_symbol_read(&symbol->env_block, symbol->type, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_FILESTATIC:
            cv_file_static_symbol_read(&symbol->file_static, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_SECTION:
            cv_section_symbol_read(&symbol->section, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_COFFGROUP:
            cv_coff_group_symbol_read(&symbol->coff_group, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_ANNOTATION:
            cv_annotation_symbol_read(&symbol->annotation, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DEFRANGE:
            cv_def_range_symbol_read(&symbol->def_range, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DEFRANGE_SUBFIELD:
            cv_def_range_subfield_symbol_read(&symbol->def_range_subfield, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DEFRANGE_REGISTER:
            cv_def_range_register_symbol_read(&symbol->def_range_register, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DEFRANGE_FRAMEPOINTER_REL:
        case S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE:
            cv_def_range_frame_pointer_rel_symbol_read(&symbol->def_range_frame_pointer_rel, symbol->type, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;

        case S_DEFRANGE_SUBFIELD_REGISTER:
            cv_def_range_subfield_register_symbol_read(&symbol->def_range_subfield_register, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DEFRANGE_REGISTER_REL:
            cv_def_range_register_rel_symbol_read(&symbol->def_range_register_rel, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DEFRANGE_HLSL:
        case S_DEFRANGE_DPC_PTR_TAG:
            cv_def_range_hlsl_symbol_read(&symbol->def_range_hlsl, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;

        case S_LOCAL_DPC_GROUPSHARED:
            cv_local_dpc_group_shared_symbol_read(&symbol->local_dpc_group_shared, symbol->type, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_DPC_SYM_TAG_MAP:
            cv_dpc_symbol_tag_map_symbol_read(&symbol->dpc_symbol_tag_map, size_remaining, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_CALLERS:
        case S_CALLEES:
            cv_function_list_symbol_read(&symbol->function_list, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_POGODATA:
            cv_pogo_data_symbol_read(&symbol->pogo_data, msf, msf_stream, out_offset, file_stream);
            break;
        
        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type value: ", __FILE__, __LINE__);
            cv_symbol_type_print(symbol->type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        uint32_t padding = 4 - (*out_offset % 4);

        if (padding < 4)
            *out_offset += padding;

        if (*out_offset != start_offset + symbol->size)
        {
            fprintf(stderr, "%s:%i: ERROR: Invalid cv_symbol read function for type ", __FILE__, __LINE__);
            cv_symbol_type_print(symbol->type, stderr);
            fprintf(stderr, ": read %u bytes, expected %u bytes\n", *out_offset - start_offset, symbol->size);
            exit(EXIT_FAILURE);
        }
    }

    //
    // Cleanup
    //

    free(symbol_info);
}
