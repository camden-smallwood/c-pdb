#include <stdlib.h>
#include <string.h>
#include "cv.h"
#include "dbi.h"
#include "utils.h"
#include "macros_print.h"

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

void cv_compile_flags_print(struct cv_compile_flags *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    CV_COMPILE_FLAGS_STRUCT
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

    uint32_t signature = 0;
    MSF_STREAM_READ(msf, msf_stream, out_offset, signature, file_stream);

    // TODO: support other versions
    assert(signature == CV_SIGNATURE_C13);

    uint32_t symbol_count = 0;
    struct cv_symbol *symbols = NULL;

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
            // TODO: struct cv_constant_symbol, constant_symbol, type, cv_constant_symbol_print
            break;

        case S_UDT:
        case S_UDT_ST:
        case S_COBOLUDT:
        case S_COBOLUDT_ST:
            // TODO: struct cv_user_defined_type_symbol, user_defined_type_symbol, type, cv_user_defined_type_symbol_print
            break;

        case S_MANYREG:
        case S_MANYREG_ST:
        case S_MANYREG2:
        case S_MANYREG2_ST:
            // TODO: struct cv_multi_register_variable_symbol, multi_register_variable_symbol, type, cv_multi_register_variable_symbol_print
            break;

        case S_LDATA32:
        case S_LDATA32_ST:
        case S_GDATA32:
        case S_GDATA32_ST:
        case S_LMANDATA:
        case S_LMANDATA_ST:
        case S_GMANDATA:
        case S_GMANDATA_ST:
            // TODO: struct cv_data_symbol, data_symbol, type, cv_data_symbol_print
            break;

        case S_PUB32:
        case S_PUB32_ST:
            // TODO: struct cv_public_symbol, public_symbol, type, cv_public_symbol_print
            break;

        case S_LPROC32:
        case S_LPROC32_ST:
        case S_GPROC32:
        case S_GPROC32_ST:
        case S_LPROC32_ID:
        case S_GPROC32_ID:
        case S_LPROC32_DPC:
        case S_LPROC32_DPC_ID:
            // TODO: struct cv_procedure_symbol, procedure_symbol, type, cv_procedure_symbol_print
            break;

        case S_LTHREAD32:
        case S_LTHREAD32_ST:
        case S_GTHREAD32:
        case S_GTHREAD32_ST:
            // TODO: struct cv_thread_storage_symbol, thread_storage_symbol, type, cv_thread_storage_symbol_print
            break;

        case S_COMPILE2:
        case S_COMPILE2_ST:
        case S_COMPILE3:
            // TODO: struct cv_compile_flags_symbol, compile_flags_symbol, type, cv_compile_flags_symbol_print
            break;

        case S_UNAMESPACE:
        case S_UNAMESPACE_ST:
            // TODO: struct cv_using_namespace_symbol, using_namespace_symbol, type, cv_using_namespace_symbol_print
            break;

        case S_PROCREF:
        case S_PROCREF_ST:
        case S_LPROCREF:
        case S_LPROCREF_ST:
            // TODO: struct cv_procedure_reference_symbol, procedure_reference_symbol, type, cv_procedure_reference_symbol_print
            break;

        case S_DATAREF:
        case S_DATAREF_ST:
            // TODO: struct cv_data_reference_symbol, data_reference_symbol, type, cv_data_reference_symbol_print
            break;

        case S_ANNOTATIONREF:
            // TODO: struct cv_annotation_reference_symbol, annotation_reference_symbol, type, cv_annotation_reference_symbol_print
            break;

        case S_TRAMPOLINE:
            // TODO: struct cv_trampoline_symbol, trampoline_symbol, type, cv_trampoline_symbol_print
            break;

        case S_EXPORT:
            // TODO: struct cv_export_symbol, export_symbol, type, cv_export_symbol_print
            break;

        case S_LOCAL:
            // TODO: struct cv_local_symbol, local_symbol, type, cv_local_symbol_print
            break;

        case S_BUILDINFO:
            // TODO: struct cv_build_info_symbol, build_info_symbol, type, cv_build_info_symbol_print
            break;

        case S_INLINESITE:
        case S_INLINESITE2:
            // TODO: struct cv_inline_site_symbol, inline_site_symbol, type, cv_inline_site_symbol_print
            break;

        case S_INLINESITE_END:
            break;
        
        case S_PROC_ID_END:
            break;

        case S_LABEL32:
        case S_LABEL32_ST:
            // TODO: struct cv_label_symbol, label_symbol, type, cv_label_symbol_print
            break;

        case S_BLOCK32:
        case S_BLOCK32_ST:
            // TODO: struct cv_block_symbol, block_symbol, type, cv_block_symbol_print
            break;

        case S_REGREL32:
            // TODO: struct cv_register_relative_symbol, register_relative_symbol, type, cv_register_relative_symbol_print
            break;

        case S_THUNK32:
        case S_THUNK32_ST:
            // TODO: struct cv_thunk_symbol, thunk_symbol, type, cv_thunk_symbol_print
            break;

        case S_SEPCODE:
            // TODO: struct cv_separated_code_symbol, separated_code_symbol, type, cv_separated_code_symbol_print
            break;
        
        //
        // TODO: investigate these:
        //

        case S_DEFRANGE_REGISTER:
        case S_DEFRANGE_REGISTER_REL:
        case S_DEFRANGE_FRAMEPOINTER_REL:
        case S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE:
        case S_DEFRANGE_SUBFIELD_REGISTER:
        case S_FRAMECOOKIE:
        case S_FRAMEPROC:
        case S_CALLSITEINFO:
        case S_ENVBLOCK:
        case S_FILESTATIC:
        case S_CALLEES:
        case S_SECTION:
        case S_COFFGROUP:
            break;

        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cv_symbol_type value: ", __FILE__, __LINE__);
            cv_symbol_type_print(symbol.type, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        //
        // TODO
        //

        // TODO: remove this VVV
        // cv_symbol_print(&symbol, 0, stdout);
        // printf("\n");

        DYNARRAY_PUSH(symbols, symbol_count, symbol);

        *out_offset = start_offset + symbol.size;
    }
}
