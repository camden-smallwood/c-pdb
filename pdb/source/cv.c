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

void cv_register_variable_symbol_print(struct cv_register_variable_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_REGISTER_VARIABLE_SYMBOL_STRUCT
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
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint32_t symbol_type,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(symbol_type);
    assert(file_stream);

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->major, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->minor, file_stream);
    MSF_STREAM_READ(msf, msf_stream, out_offset, item->build, file_stream);

    if (symbol_type == S_COMPILE3)
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->qfe, file_stream);
}

void cv_compile_flags_print(enum cv_compile_flags item, FILE *stream)
{
    assert(stream);

    CV_COMPILE_FLAGS_ENUM
}

void cv_compile_flags_symbol_dispose(struct cv_compile_flags_symbol *item)
{
    assert(item);
}

void cv_compile_flags_symbol_print(struct cv_compile_flags_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_COMPILE_FLAGS_SYMBOL_STRUCT
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

void cv_build_info_symbol_print(struct cv_build_info_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_BUILD_INFO_SYMBOL_STRUCT
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

void cv_separated_code_symbol_print(struct cv_separated_code_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_SEPARATED_CODE_SYMBOL_STRUCT
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

void cv_call_site_info_symbol_print(struct cv_call_site_info_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_CALL_SITE_INFO_SYMBOL_STRUCT
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

void cv_symbol_dispose(struct cv_symbol *item)
{
    assert(item);

    switch (item->type)
    {
    case S_END:
        break;

    case S_OBJNAME:
    case S_OBJNAME_ST:
        cv_obj_name_symbol_dispose(&item->obj_name_symbol);
        break;

    case S_REGISTER:
    case S_REGISTER_ST:
        break;

    case S_CONSTANT:
    case S_CONSTANT_ST:
    case S_MANCONSTANT:
        cv_constant_symbol_dispose(&item->constant_symbol);
        break;

    case S_UDT:
    case S_UDT_ST:
    case S_COBOLUDT:
    case S_COBOLUDT_ST:
        cv_user_defined_type_symbol_dispose(&item->user_defined_type_symbol);
        break;

    case S_MANYREG:
    case S_MANYREG_ST:
    case S_MANYREG2:
    case S_MANYREG2_ST:
        cv_multi_register_variable_symbol_dispose(&item->multi_register_variable_symbol);
        break;

    case S_LDATA32:
    case S_LDATA32_ST:
    case S_GDATA32:
    case S_GDATA32_ST:
    case S_LMANDATA:
    case S_LMANDATA_ST:
    case S_GMANDATA:
    case S_GMANDATA_ST:
        cv_data_symbol_dispose(&item->data_symbol);
        break;

    case S_PUB32:
    case S_PUB32_ST:
        cv_public_symbol_dispose(&item->public_symbol);
        break;

    case S_LPROC32:
    case S_LPROC32_ST:
    case S_GPROC32:
    case S_GPROC32_ST:
    case S_LPROC32_ID:
    case S_GPROC32_ID:
    case S_LPROC32_DPC:
    case S_LPROC32_DPC_ID:
        cv_procedure_symbol_dispose(&item->procedure_symbol);
        break;

    case S_LTHREAD32:
    case S_LTHREAD32_ST:
    case S_GTHREAD32:
    case S_GTHREAD32_ST:
        cv_thread_storage_symbol_dispose(&item->thread_storage_symbol);
        break;

    case S_COMPILE2:
    case S_COMPILE2_ST:
    case S_COMPILE3:
        cv_compile_flags_symbol_dispose(&item->compile_flags_symbol);
        break;

    case S_UNAMESPACE:
    case S_UNAMESPACE_ST:
        cv_using_namespace_symbol_dispose(&item->using_namespace_symbol);
        break;

    case S_PROCREF:
    case S_PROCREF_ST:
    case S_LPROCREF:
    case S_LPROCREF_ST:
        cv_procedure_reference_symbol_dispose(&item->procedure_reference_symbol);
        break;

    case S_DATAREF:
    case S_DATAREF_ST:
        cv_data_reference_symbol_dispose(&item->data_reference_symbol);
        break;

    case S_ANNOTATIONREF:
        cv_annotation_reference_symbol_dispose(&item->annotation_reference_symbol);
        break;

    case S_TRAMPOLINE:
        break;

    case S_EXPORT:
        cv_export_symbol_dispose(&item->export_symbol);
        break;

    case S_LOCAL:
        cv_local_symbol_dispose(&item->local_symbol);
        break;

    case S_BUILDINFO:
        break;

    case S_INLINESITE:
    case S_INLINESITE2:
        cv_inline_site_symbol_dispose(&item->inline_site_symbol);
        break;

    case S_INLINESITE_END:
        break;

    case S_PROC_ID_END:
        break;

    case S_LABEL32:
    case S_LABEL32_ST:
        cv_label_symbol_dispose(&item->label_symbol);
        break;

    case S_BLOCK32:
    case S_BLOCK32_ST:
        cv_block_symbol_dispose(&item->block_symbol);
        break;

    case S_REGREL32:
        cv_register_relative_symbol_dispose(&item->register_relative_symbol);
        break;

    case S_THUNK32:
    case S_THUNK32_ST:
        cv_thunk_symbol_dispose(&item->thunk_symbol);
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
        cv_env_block_symbol_dispose(&item->env_block_symbol);
        break;

    case S_FILESTATIC:
        break;

    case S_CALLERS:
    case S_CALLEES:
        cv_function_list_symbol_dispose(&item->function_list_symbol);
        break;
    
    case S_SECTION:
        cv_section_symbol_dispose(&item->section_symbol);
        break;

    case S_COFFGROUP:
        cv_coff_group_symbol_dispose(&item->coff_group_symbol);
        break;
    
    case S_ANNOTATION:
        cv_annotation_symbol_dispose(&item->annotation_symbol);
        break;
    
    case S_DEFRANGE_REGISTER:
    case S_DEFRANGE_REGISTER_REL:
    case S_DEFRANGE_FRAMEPOINTER_REL:
    case S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE:
    case S_DEFRANGE_SUBFIELD_REGISTER:
        // TODO
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

    while (*out_offset < symbols_size)
    {
        struct cv_symbol symbol;
        memset(&symbol, 0, sizeof(symbol));

        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.size, file_stream);
        assert(symbol.size >= sizeof(uint16_t));

        uint32_t start_offset = *out_offset;

        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.type, file_stream);

        switch (symbol.type)
        {
        case S_END:
            break;
        
        case S_OBJNAME:
        case S_OBJNAME_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.obj_name_symbol.signature, file_stream);
            symbol.obj_name_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;
        
        case S_REGISTER:
        case S_REGISTER_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.register_variable_symbol.type_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.register_variable_symbol.register_index, file_stream);
            symbol.register_variable_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_CONSTANT:
        case S_CONSTANT_ST:
        case S_MANCONSTANT:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.constant_symbol.type_index, file_stream);
            tpi_enumerate_variant_read(&symbol.constant_symbol.value, msf, msf_stream, out_offset, file_stream);
            symbol.constant_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_UDT:
        case S_UDT_ST:
        case S_COBOLUDT:
        case S_COBOLUDT_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.user_defined_type_symbol.type_index, file_stream);
            symbol.user_defined_type_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_MANYREG:
        case S_MANYREG_ST:
        case S_MANYREG2:
        case S_MANYREG2_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.multi_register_variable_symbol.type_index, file_stream);
            if (symbol.type == S_MANYREG2 || symbol.type == S_MANYREG2_ST)
            {
                uint16_t count = 0;
                MSF_STREAM_READ(msf, msf_stream, out_offset, count, file_stream);
                symbol.multi_register_variable_symbol.register_count = count;
            }
            else
            {
                uint8_t count = 0;
                MSF_STREAM_READ(msf, msf_stream, out_offset, count, file_stream);
                symbol.multi_register_variable_symbol.register_count = count;
            }
            for (uint32_t i = 0; i < symbol.multi_register_variable_symbol.register_count; i++)
            {
                struct cv_multi_register_entry entry;
                MSF_STREAM_READ(msf, msf_stream, out_offset, entry.register_index, file_stream);
                entry.register_name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
                DYNARRAY_PUSH(symbol.multi_register_variable_symbol.registers, symbol.multi_register_variable_symbol.register_count, entry);
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
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.data_symbol.type_index, file_stream);
            cv_pe_section_offset_read(&symbol.data_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            symbol.data_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_PUB32:
        case S_PUB32_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.public_symbol.flags, file_stream);
            cv_pe_section_offset_read(&symbol.public_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            symbol.public_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_LPROC32:
        case S_LPROC32_ST:
        case S_GPROC32:
        case S_GPROC32_ST:
        case S_LPROC32_ID:
        case S_GPROC32_ID:
        case S_LPROC32_DPC:
        case S_LPROC32_DPC_ID:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.parent_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.end_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.next_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.code_block_length, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.debug_start_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.debug_end_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.type_index, file_stream);
            cv_pe_section_offset_read(&symbol.procedure_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_symbol.flags, file_stream);
            symbol.procedure_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.thread_storage_symbol.type_index, file_stream);
            cv_pe_section_offset_read(&symbol.thread_storage_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            symbol.thread_storage_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_COMPILE2:
        case S_COMPILE2_ST:
        case S_COMPILE3:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.compile_flags_symbol.language, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.compile_flags_symbol.flags, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.compile_flags_symbol.padding, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.compile_flags_symbol.cpu_type, file_stream);
            cv_compiler_version_read(&symbol.compile_flags_symbol.frontend_version, msf, msf_stream, out_offset, symbol.type, file_stream);
            cv_compiler_version_read(&symbol.compile_flags_symbol.backend_version, msf, msf_stream, out_offset, symbol.type, file_stream);
            symbol.compile_flags_symbol.version_string = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_UNAMESPACE:
        case S_UNAMESPACE_ST:
            symbol.using_namespace_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_PROCREF:
        case S_PROCREF_ST:
        case S_LPROCREF:
        case S_LPROCREF_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_reference_symbol.sum_name, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_reference_symbol.symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.procedure_reference_symbol.module_index, file_stream);
            symbol.procedure_reference_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_DATAREF:
        case S_DATAREF_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.data_reference_symbol.sum_name, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.data_reference_symbol.symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.data_reference_symbol.module_index, file_stream);
            symbol.data_reference_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_ANNOTATIONREF:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.annotation_reference_symbol.sum_name, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.annotation_reference_symbol.symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.annotation_reference_symbol.module_index, file_stream);
            symbol.annotation_reference_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_TRAMPOLINE:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.trampoline_symbol.type, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.trampoline_symbol.size, file_stream);
            cv_pe_section_offset_read(&symbol.trampoline_symbol.thunk_offset, msf, msf_stream, out_offset, file_stream);
            cv_pe_section_offset_read(&symbol.trampoline_symbol.target_offset, msf, msf_stream, out_offset, file_stream);
            break;

        case S_EXPORT:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.export_symbol.ordinal, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.export_symbol.flags, file_stream);
            symbol.export_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_LOCAL:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.local_symbol.type_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.local_symbol.flags, file_stream);
            symbol.local_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_BUILDINFO:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.build_info_symbol.id_index, file_stream);
            break;

        case S_INLINESITE:
        case S_INLINESITE2:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.inline_site_symbol.parent_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.inline_site_symbol.end_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.inline_site_symbol.inlinee_id_index, file_stream);
            if (symbol.type == S_INLINESITE2)
                MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.inline_site_symbol.invocation_count, file_stream);
            while (*out_offset < start_offset + symbol.size)
            {
                struct cv_annotation annotation;
                cv_annotation_read(&annotation, msf, msf_stream, out_offset, file_stream);
                DYNARRAY_PUSH(
                    symbol.inline_site_symbol.annotations.annotations,
                    symbol.inline_site_symbol.annotations.count,
                    annotation);
            }
            break;

        case S_INLINESITE_END:
            break;
        
        case S_PROC_ID_END:
            break;

        case S_LABEL32:
        case S_LABEL32_ST:
            cv_pe_section_offset_read(&symbol.label_symbol.section_offset, msf, msf_stream, out_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.label_symbol.flags, file_stream);
            symbol.label_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_BLOCK32:
        case S_BLOCK32_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.block_symbol.parent_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.block_symbol.end_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.block_symbol.length, file_stream);
            cv_pe_section_offset_read(&symbol.block_symbol.section_offset, msf, msf_stream, out_offset, file_stream);
            symbol.block_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_REGREL32:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.register_relative_symbol.offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.register_relative_symbol.type_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.register_relative_symbol.register_index, file_stream);
            symbol.register_relative_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_THUNK32:
        case S_THUNK32_ST:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.thunk_symbol.parent_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.thunk_symbol.end_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.thunk_symbol.next_symbol_index, file_stream);
            cv_pe_section_offset_read(&symbol.thunk_symbol.section_offset, msf, msf_stream, out_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.thunk_symbol.length, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.thunk_symbol.ordinal, file_stream);
            symbol.thunk_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;

        case S_SEPCODE:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.separated_code_symbol.parent_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.separated_code_symbol.end_symbol_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.separated_code_symbol.length, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.separated_code_symbol.flags, file_stream);
            cv_pe_section_offset_read(&symbol.separated_code_symbol.section_offset, msf, msf_stream, out_offset, file_stream);
            cv_pe_section_offset_read(&symbol.separated_code_symbol.parent_section_offset, msf, msf_stream, out_offset, file_stream);
            break;
        
        case S_FRAMECOOKIE:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_cookie_symbol.frame_relative_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_cookie_symbol.register_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_cookie_symbol.cookie_type, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_cookie_symbol.flags, file_stream);
            break;
        
        case S_FRAMEPROC:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_proc_symbol.frame_size, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_proc_symbol.padding_size, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_proc_symbol.padding_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_proc_symbol.callee_save_registers_size, file_stream);
            cv_pe_section_offset_read(&symbol.frame_proc_symbol.exception_handler_offset, msf, msf_stream, out_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.frame_proc_symbol.flags, file_stream);
            break;
        
        case S_CALLSITEINFO:
            cv_pe_section_offset_read(&symbol.call_site_info_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.call_site_info_symbol.padding, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.call_site_info_symbol.type_index, file_stream);
            break;
        
        case S_ENVBLOCK:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.env_block_symbol.flags, file_stream);

            while (*out_offset < start_offset + symbol.size)
            {
                char *string = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);

                if (strlen(string) == 0)
                {
                    free(string);
                    break;
                }

                DYNARRAY_PUSH(symbol.env_block_symbol.strings, symbol.env_block_symbol.string_count, string);
            }
            break;
        
        case S_FILESTATIC:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.file_static_symbol.type_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.file_static_symbol.module_filename_string_index, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.file_static_symbol.flags, file_stream);
            symbol.file_static_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;
        
        case S_CALLERS:
        case S_CALLEES:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.function_list_symbol.count, file_stream);
            symbol.function_list_symbol.type_indices = malloc(symbol.function_list_symbol.count * sizeof(uint32_t));
            assert(symbol.function_list_symbol.type_indices);
            for (uint32_t i = 0; i < symbol.function_list_symbol.count; i++)
                MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.function_list_symbol.type_indices[i], file_stream);
            break;

        case S_SECTION:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.section_symbol.isec, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.section_symbol.align, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.section_symbol.bReserved, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.section_symbol.rva, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.section_symbol.cb, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.section_symbol.characteristics, file_stream);
            symbol.section_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;
        
        case S_COFFGROUP:
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.coff_group_symbol.cb, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.coff_group_symbol.characteristics, file_stream);
            cv_pe_section_offset_read(&symbol.coff_group_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            symbol.coff_group_symbol.name = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;
        
        case S_ANNOTATION:
            cv_pe_section_offset_read(&symbol.annotation_symbol.code_offset, msf, msf_stream, out_offset, file_stream);
            MSF_STREAM_READ(msf, msf_stream, out_offset, symbol.annotation_symbol.string_count, file_stream);
            symbol.annotation_symbol.strings = malloc(symbol.annotation_symbol.string_count * sizeof(char *));
            assert(symbol.annotation_symbol.strings);
            for (uint16_t i = 0; i < symbol.annotation_symbol.string_count; i++)
                symbol.annotation_symbol.strings[i] = msf_read_cv_symbol_string(msf, msf_stream, out_offset, symbol.type, file_stream);
            break;
        
        //
        // TODO: investigate these:
        //

        case S_DEFRANGE_REGISTER:
        case S_DEFRANGE_REGISTER_REL:
        case S_DEFRANGE_FRAMEPOINTER_REL:
        case S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE:
        case S_DEFRANGE_SUBFIELD_REGISTER:
            // TODO: remove this VVV
            // cv_symbol_print(&symbol, 0, stdout);
            // printf("\n");
            break;

        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type value: ", __FILE__, __LINE__);
            cv_symbol_type_print(symbol.type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }
        
        DYNARRAY_PUSH(item->symbols, item->count, symbol);

        *out_offset = start_offset + symbol.size;
    }
}
