#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "tpi.h"

#include "macros_print.h"

void tpi_slice_print(struct tpi_slice *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_SLICE_STRUCT
}

void tpi_header_read(struct msf *msf, struct tpi_header *out_header, FILE *stream)
{
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_TPI], 0, sizeof(*out_header), out_header, stream);

    assert(out_header->minimum_index >= 4096);
    assert(out_header->maximum_index >= out_header->minimum_index);
}

void tpi_header_print(struct tpi_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_HEADER_STRUCT
}

void tpi_leaf_print(enum tpi_leaf item, FILE *stream)
{
    assert(stream);

    TPI_LEAF_ENUM
}

void tpi_primitive_type_print(enum tpi_primitive_type item, FILE *stream)
{
    assert(stream);

    TPI_PRIMITIVE_TYPE_ENUM
}

void tpi_primitive_indirection_print(enum tpi_primitive_indirection item, FILE *stream)
{
    assert(stream);

    TPI_PRIMITIVE_INDIRECTION_ENUM
}

void tpi_primitive_print(struct tpi_primitive *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_PRIMITIVE_STRUCT
}

void tpi_hfa_type_print(enum tpi_hfa_type item, FILE *stream)
{
    assert(stream);

    TPI_HFA_TYPE_ENUM
}

void tpi_mocom_udt_type_print(enum tpi_mocom_udt_type item, FILE *stream)
{
    assert(stream);

    TPI_MOCOM_UDT_TYPE_ENUM
}

void tpi_properties_print(struct tpi_properties *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_PROPERTIES_STRUCT
}

void tpi_member_header_print(struct tpi_member_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_MEMBER_HEADER_STRUCT
}

void tpi_member_read(
    struct tpi_member *item,
    uint32_t leaf,
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

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

    item->offset = msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);
}

void tpi_member_dispose(struct tpi_member *item)
{
    assert(item);

    free(item->name);
}

void tpi_member_print(struct tpi_member *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_MEMBER_STRUCT
}

void tpi_static_member_header_print(struct tpi_static_member_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_STATIC_MEMBER_HEADER_STRUCT
}

void tpi_static_member_dispose(struct tpi_static_member *item)
{
    assert(item);

    free(item->name);
}

void tpi_static_member_print(struct tpi_static_member *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_STATIC_MEMBER_STRUCT
}

void tpi_pointer_type_print(enum tpi_pointer_type item, FILE *stream)
{
    assert(stream);

    TPI_POINTER_TYPE_ENUM
}

void tpi_pointer_mode_print(enum tpi_pointer_mode item, FILE *stream)
{
    assert(stream);

    TPI_POINTER_MODE_ENUM
}

void tpi_pointer_attributes_print(struct tpi_pointer_attributes *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_POINTER_ATTRIBUTES_STRUCT
}

void tpi_pointer_header_print(struct tpi_pointer_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_POINTER_HEADER_STRUCT
}

void tpi_pointer_print(struct tpi_pointer *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_POINTER_STRUCT
}

void tpi_argument_list_dispose(struct tpi_argument_list *item)
{
    assert(item);

    free(item->type_indices);
}

void tpi_argument_list_print(struct tpi_argument_list *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ARGUMENT_LIST_STRUCT
}

void tpi_procedure_attributes_print(struct tpi_procedure_attributes *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_PROCEDURE_ATTRIBUTES_STRUCT
}

void tpi_procedure_print(struct tpi_procedure *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_PROCEDURE_STRUCT
}

void tpi_modifier_print(struct tpi_modifier *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_MODIFIER_STRUCT
}

void tpi_enumerate_variant_type_print(enum tpi_enumerate_variant_type item, FILE *stream)
{
    assert(stream);

    TPI_ENUMERATE_VARIANT_TYPE_ENUM
}

void tpi_enumerate_variant_read(
    struct tpi_enumerate_variant *variant,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(variant);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    memset(variant, 0, sizeof(*variant));

    uint16_t variant_leaf;
    MSF_STREAM_READ(msf, msf_stream, out_offset, variant_leaf, file_stream);

    switch (variant_leaf)
    {
    case LF_CHAR:
        variant->type = TPI_ENUMERATE_VARIANT_UINT8;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->uint8, file_stream);
        break;

    case LF_SHORT:
        variant->type = TPI_ENUMERATE_VARIANT_INT16;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->int16, file_stream);
        break;

    case LF_LONG:
        variant->type = TPI_ENUMERATE_VARIANT_INT32;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->int32, file_stream);
        break;

    case LF_QUADWORD:
        variant->type = TPI_ENUMERATE_VARIANT_INT64;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->int64, file_stream);
        break;

    case LF_USHORT:
        variant->type = TPI_ENUMERATE_VARIANT_UINT16;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->uint16, file_stream);
        break;

    case LF_ULONG:
        variant->type = TPI_ENUMERATE_VARIANT_UINT32;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->uint32, file_stream);
        break;

    case LF_UQUADWORD:
        variant->type = TPI_ENUMERATE_VARIANT_UINT64;
        MSF_STREAM_READ(msf, msf_stream, out_offset, variant->uint64, file_stream);
        break;

    default:
        if (variant_leaf < LF_NUMERIC)
        {
            variant->type = TPI_ENUMERATE_VARIANT_UINT16;
            variant->uint16 = variant_leaf;
            break;
        }

        fprintf(stderr, "%s:%i: ERROR: Unhandled variant leaf type: ", __FILE__, __LINE__);
        tpi_leaf_print(variant_leaf, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }

    assert(variant->type <= TPI_ENUMERATE_VARIANT_INT64);
}

void tpi_enumerate_variant_print(struct tpi_enumerate_variant *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ENUMERATE_VARIANT_STRUCT
}

char *tpi_enumerate_variant_to_string(struct tpi_enumerate_variant *variant)
{
    assert(variant);

    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    switch (variant->type)
    {
    case TPI_ENUMERATE_VARIANT_UINT8:
        sprintf(buffer, "%u", variant->uint8);
        break;

    case TPI_ENUMERATE_VARIANT_UINT16:
        sprintf(buffer, "%u", variant->uint16);
        break;

    case TPI_ENUMERATE_VARIANT_UINT32:
        sprintf(buffer, "%u", variant->uint32);
        break;

    case TPI_ENUMERATE_VARIANT_UINT64:
        sprintf(buffer, "%llu", variant->uint64);
        break;

    case TPI_ENUMERATE_VARIANT_INT8:
        sprintf(buffer, "%i", variant->int8);
        break;

    case TPI_ENUMERATE_VARIANT_INT16:
        sprintf(buffer, "%i", variant->int16);
        break;

    case TPI_ENUMERATE_VARIANT_INT32:
        sprintf(buffer, "%i", variant->int32);
        break;

    case TPI_ENUMERATE_VARIANT_INT64:
        sprintf(buffer, "%lli", variant->int64);
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled tpi_enumerate_variant_type value: ", __FILE__, __LINE__);
        tpi_enumerate_variant_type_print(variant->type, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }

    return strdup(buffer);
}

void tpi_enumerate_dispose(struct tpi_enumerate *item)
{
    assert(item);

    free(item->name);
}

void tpi_enumerate_print(struct tpi_enumerate *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ENUMERATE_STRUCT
}

void tpi_enumeration_header_print(struct tpi_enumeration_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ENUMERATION_HEADER_STRUCT
}

void tpi_enumeration_dispose(struct tpi_enumeration *item)
{
    assert(item);

    free(item->name);
    free(item->unique_name);
}

void tpi_enumeration_print(struct tpi_enumeration *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ENUMERATION_STRUCT
}

void tpi_array_header_print(struct tpi_array_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ARRAY_HEADER_STRUCT
}

void tpi_array_dispose(struct tpi_array *item)
{
    assert(item);

    free(item->dimensions);
}

void tpi_array_print(struct tpi_array *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_ARRAY_STRUCT
}

void tpi_field_attributes_print(struct tpi_field_attributes *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_FIELD_ATTRIBUTES_STRUCT
}

void tpi_field_list_dispose(struct tpi_field_list *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->count; i++)
        tpi_symbol_dispose(&item->fields[i]);

    free(item->fields);
}

void tpi_field_list_print(struct tpi_field_list *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_FIELD_LIST_STRUCT
}

void tpi_union_header_print(struct tpi_union_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_UNION_HEADER_STRUCT
}

void tpi_union_dispose(struct tpi_union *item)
{
    assert(item);

    free(item->name);
    free(item->unique_name);
}

void tpi_union_print(struct tpi_union *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_UNION_STRUCT
}

void tpi_bitfield_print(struct tpi_bitfield *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_BITFIELD_STRUCT
}

void tpi_member_function_read(
    struct tpi_member_function *item,
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

    MSF_STREAM_READ(msf, msf_stream, out_offset, *item, file_stream);
}

void tpi_member_function_print(struct tpi_member_function *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_MEMBER_FUNCTION_STRUCT
}

void tpi_overloaded_method_header_print(struct tpi_overloaded_method_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_OVERLOADED_METHOD_HEADER_STRUCT
}

void tpi_overloaded_method_dispose(struct tpi_overloaded_method *item)
{
    assert(item);

    free(item->name);
}

void tpi_overloaded_method_print(struct tpi_overloaded_method *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_OVERLOADED_METHOD_STRUCT
}

void tpi_method_header_print(struct tpi_method_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_METHOD_HEADER_STRUCT
}

void tpi_method_dispose(struct tpi_method *item)
{
    assert(item);

    free(item->name);
}

void tpi_method_print(struct tpi_method *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_METHOD_STRUCT
}

void tpi_method_list_entry_header_print(struct tpi_method_list_entry_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_METHOD_LIST_ENTRY_HEADER_STRUCT
}

void tpi_method_list_entry_print(struct tpi_method_list_entry *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_METHOD_LIST_ENTRY_STRUCT
}

void tpi_method_list_dispose(struct tpi_method_list *item)
{
    assert(item);

    free(item->entries);
}

void tpi_method_list_print(struct tpi_method_list *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_METHOD_LIST_STRUCT
}

void tpi_vtable_print(struct tpi_vtable *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_VTABLE_STRUCT
}

void tpi_nested_type_header_print(struct tpi_nested_type_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_NESTED_TYPE_HEADER_STRUCT
}

void tpi_nested_type_read(
    struct tpi_nested_type *item,
    uint32_t leaf,
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

    MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

    item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);
}

void tpi_nested_type_dispose(struct tpi_nested_type *item)
{
    assert(item);

    free(item->name);
}

void tpi_nested_type_print(struct tpi_nested_type *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_NESTED_TYPE_STRUCT
}

void tpi_base_class_header_print(struct tpi_base_class_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_BASE_CLASS_HEADER_STRUCT
}

void tpi_base_class_print(struct tpi_base_class *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_BASE_CLASS_STRUCT
}

void tpi_virtual_base_class_header_print(struct tpi_virtual_base_class_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_VIRTUAL_BASE_CLASS_HEADER_STRUCT
}

void tpi_virtual_base_class_print(struct tpi_virtual_base_class *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_VIRTUAL_BASE_CLASS_STRUCT
}

void tpi_class_header_print(struct tpi_class_header *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_CLASS_HEADER_STRUCT
}

void tpi_class_read(
    struct tpi_class *item,
    uint32_t leaf,
    struct msf *msf,
    struct msf_stream *msf_stream,
    uint32_t *out_offset,
    uint32_t end_offset,
    FILE *file_stream)
{
    assert(item);
    assert(msf);
    assert(msf_stream);
    assert(out_offset);
    assert(file_stream);

    if (leaf == LF_STRUCTURE19)
    {
        struct tpi_properties properties;
        MSF_STREAM_READ(msf, msf_stream, out_offset, properties, file_stream);
        *out_offset += sizeof(uint16_t); // padding

        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

        item->header.properties = properties;
    }
    else
    {
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);
    }

    if (leaf != LF_UNION)
    {
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->derived_from_type_index, file_stream);
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->vtable_shape_type_index, file_stream);
    }

    item->size = msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);

    if (item->header.properties.hasuniquename)
        item->unique_name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);

    msf_stream_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
}

void tpi_class_dispose(struct tpi_class *item)
{
    assert(item);

    free(item->name);
    free(item->unique_name);
}

void tpi_class_print(struct tpi_class *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_CLASS_STRUCT
}

void tpi_symbol_read(
    struct tpi_symbol *symbol,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct tpi_header *tpi_header,
    uint32_t *out_offset,
    FILE *file_stream)
{
    assert(symbol);
    assert(msf);
    assert(msf_stream);
    assert(tpi_header);
    assert(out_offset);
    assert(file_stream);

    uint32_t start_offset = *out_offset;
    uint32_t end_offset = start_offset + symbol->size;

    MSF_STREAM_READ(msf, msf_stream, out_offset, symbol->leaf, file_stream);

    switch (symbol->leaf)
    {
    case LF_CLASS:
    case LF_CLASS_ST:
    case LF_STRUCTURE:
    case LF_STRUCTURE_ST:
    case LF_STRUCTURE19:
    case LF_INTERFACE:
        tpi_class_read(&symbol->class_, symbol->leaf, msf, msf_stream, out_offset, end_offset, file_stream);
        break;

    case LF_MEMBER:
    case LF_MEMBER_ST:
        tpi_member_read(&symbol->member, symbol->leaf, msf, msf_stream, out_offset, file_stream);
        break;

    case LF_NESTTYPE:
    case LF_NESTTYPE_ST:
    case LF_NESTTYPEEX:
    case LF_NESTTYPEEX_ST:
        tpi_nested_type_read(&symbol->nested_type, symbol->leaf, msf, msf_stream, out_offset, file_stream);
        break;

    case LF_MFUNCTION:
        tpi_member_function_read(&symbol->member_function, msf, msf_stream, out_offset, file_stream);
        break;

    case LF_METHOD:
    case LF_METHOD_ST:
    {
        struct tpi_overloaded_method *item = &symbol->overloaded_method;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);
        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);
        break;
    }

    case LF_ONEMETHOD:
    case LF_ONEMETHOD_ST:
    {
        struct tpi_method *item = &symbol->method;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

        if (item->header.attributes.mprop == TPI_METHOD_INTRO || item->header.attributes.mprop == TPI_METHOD_PUREINTRO)
            MSF_STREAM_READ(msf, msf_stream, out_offset, item->vtable_offset, file_stream);

        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);
        break;
    }

    case LF_BCLASS:
    case LF_BINTERFACE:
    {
        struct tpi_base_class *item = &symbol->base_class;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);
        item->offset = (uint32_t)msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
        break;
    }

    case LF_VFUNCTAB:
        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol->vtable, file_stream);
        break;

    case LF_STMEMBER:
    case LF_STMEMBER_ST:
    {
        struct tpi_static_member *item = &symbol->static_member;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);
        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);
        break;
    }

    case LF_POINTER:
    {
        struct tpi_pointer *item = &symbol->pointer;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

        if (item->header.attributes.ptrmode == CV_PTR_MODE_PMEM || item->header.attributes.ptrmode == CV_PTR_MODE_PMFUNC)
            MSF_STREAM_READ(msf, msf_stream, out_offset, item->containing_class_type_index, file_stream);

        // TODO: seems to be something else here...
        break;
    }

    case LF_PROCEDURE:
        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol->procedure, file_stream);
        break;

    case LF_MODIFIER:
        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol->modifier, file_stream);
        msf_stream_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
        break;

    case LF_ENUM:
    case LF_ENUM_ST:
    {
        struct tpi_enumeration *item = &symbol->enumeration;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);
        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);

        if (item->header.properties.hasuniquename)
            item->unique_name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);

        // TODO: is this correct?
        msf_stream_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
        break;
    }

    case LF_ENUMERATE:
    case LF_ENUMERATE_ST:
    {
        struct tpi_enumerate *item = &symbol->enumerate;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->attributes, file_stream);

        tpi_enumerate_variant_read(&item->variant, msf, msf_stream, out_offset, file_stream);

        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);
        break;
    }

    case LF_ARRAY:
    case LF_ARRAY_ST:
    case LF_STRIDED_ARRAY:
    {
        struct tpi_array *item = &symbol->array;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);
   
        if (symbol->leaf == LF_STRIDED_ARRAY)
            MSF_STREAM_READ(msf, msf_stream, out_offset, item->stride, file_stream);
        
        while (*out_offset < end_offset)
        {
            uint64_t dimension = msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
            assert(dimension <= UINT32_MAX);
            assert(*out_offset < msf_stream->size);

            DYNARRAY_PUSH(item->dimensions, item->dimension_count, dimension);

            uint8_t next = 0;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(next), &next, file_stream);

            if (next == 0)
            {
                *out_offset += 2;
                break;
            }
        }

        assert(item->dimension_count);

        msf_stream_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
        assert(*out_offset < msf_stream->size);
        break;
    }

    case LF_UNION:
    case LF_UNION_ST:
    {
        struct tpi_union *item = &symbol->union_;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

        item->size = msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);

        if (item->header.properties.hasuniquename)
            item->unique_name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);

        msf_stream_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
        break;
    }

    case LF_BITFIELD:
        MSF_STREAM_READ(msf, msf_stream, out_offset, symbol->bitfield, file_stream);
        break;

    case LF_VTSHAPE:
        // TODO
        *out_offset += symbol->size - sizeof(uint16_t);
        break;

    case LF_VFTABLE:
        // TODO
        *out_offset += symbol->size;
        break;

    case LF_VBCLASS:
    case LF_IVBCLASS:
    {
        struct tpi_virtual_base_class *item = &symbol->virtual_base_class;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

        item->base_pointer_offset = (uint32_t)msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
        item->virtual_base_offset = (uint32_t)msf_read_tpi_unsigned(msf, msf_stream, out_offset, file_stream);
        break;
    }

    case LF_FIELDLIST:
    {
        struct tpi_field_list *item = &symbol->field_list;
        item->continuation_type_index = 0;

        while (*out_offset < end_offset)
        {
            uint16_t field_leaf;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(field_leaf), &field_leaf, file_stream);

            if (field_leaf == LF_INDEX)
            {
                // continuation record, eat the leaf value
                *out_offset += sizeof(uint16_t);
                *out_offset += sizeof(uint16_t);

                MSF_STREAM_READ(msf, msf_stream, out_offset, item->continuation_type_index, file_stream);
            }
            else
            {
                item->count++;
                item->fields = realloc(item->fields, item->count * sizeof(*item->fields));
                assert(item->fields);

                uint32_t size = end_offset - *out_offset;
                assert(size <= UINT16_MAX);
                item->fields[item->count - 1].size = size;

                tpi_symbol_read(&item->fields[item->count - 1], msf, msf_stream, tpi_header, out_offset, file_stream);
            }

            msf_stream_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
        }
        break;
    }

    case LF_ARGLIST:
    {
        struct tpi_argument_list *item = &symbol->argument_list;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->count, file_stream);

        item->type_indices = malloc(item->count * sizeof(*item->type_indices));
        assert(item->type_indices);

        for (uint32_t i = 0; i < item->count; i++)
            MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_indices[i], file_stream);
        
        break;
    }

    case LF_METHODLIST:
    {
        struct tpi_method_list *item = &symbol->method_list;

        while (*out_offset < end_offset)
        {
            item->count++;
            item->entries = realloc(item->entries, item->count * sizeof(*item->entries));
            assert(item->entries);

            struct tpi_method_list_entry *entry = &item->entries[item->count - 1];
            memset(entry, 0, sizeof(*entry));

            MSF_STREAM_READ(msf, msf_stream, out_offset, entry->header, file_stream);

            switch (entry->header.attributes.mprop)
            {
            case 0x04:
            case 0x06:
                MSF_STREAM_READ(msf, msf_stream, out_offset, entry->vtable_offset, file_stream);
            }
        }
        break;
    }

    default:
        fprintf(stderr, "%s:%i: ERROR: Unsupported TPI symbol leaf: ", __FILE__, __LINE__);
        tpi_leaf_print(symbol->leaf, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }

    // if (*out_offset > end_offset)
    // {
    //     fprintf(stderr, "%s:%i: ERROR: Incorrect TPI symbol data parsing for leaf: ", __FILE__, __LINE__);
    //     tpi_leaf_print(symbol->leaf, stderr);
    //     uint32_t size_read = *out_offset - start_offset;
    //     fprintf(stderr, "; read %u, expected %u\n", size_read, symbol->size);
    //     exit(EXIT_FAILURE);
    // }
}

void tpi_symbol_dispose(struct tpi_symbol *symbol)
{
    assert(symbol);

    switch (symbol->leaf)
    {
    case LF_CLASS:
    case LF_CLASS_ST:
    case LF_STRUCTURE:
    case LF_STRUCTURE_ST:
    case LF_STRUCTURE19:
    case LF_INTERFACE:
        tpi_class_dispose(&symbol->class_);
        break;

    case LF_MEMBER:
    case LF_MEMBER_ST:
        tpi_member_dispose(&symbol->member);
        break;

    case LF_NESTTYPE:
    case LF_NESTTYPE_ST:
    case LF_NESTTYPEEX:
    case LF_NESTTYPEEX_ST:
        tpi_nested_type_dispose(&symbol->nested_type);
        break;

    case LF_MFUNCTION:
        // TODO
        break;

    case LF_METHOD:
    case LF_METHOD_ST:
        tpi_overloaded_method_dispose(&symbol->overloaded_method);
        break;

    case LF_ONEMETHOD:
    case LF_ONEMETHOD_ST:
        tpi_method_dispose(&symbol->method);
        break;

    case LF_BCLASS:
    case LF_BINTERFACE:
        // TODO
        break;

    case LF_VFUNCTAB:
        // TODO
        break;

    case LF_STMEMBER:
    case LF_STMEMBER_ST:
        tpi_static_member_dispose(&symbol->static_member);
        break;

    case LF_POINTER:
        break;

    case LF_PROCEDURE:
        // TODO
        break;

    case LF_MODIFIER:
        // TODO
        break;

    case LF_ENUM:
    case LF_ENUM_ST:
        tpi_enumeration_dispose(&symbol->enumeration);
        break;

    case LF_ENUMERATE:
    case LF_ENUMERATE_ST:
        tpi_enumerate_dispose(&symbol->enumerate);
        break;

    case LF_ARRAY:
    case LF_ARRAY_ST:
    case LF_STRIDED_ARRAY:
        tpi_array_dispose(&symbol->array);
        break;

    case LF_UNION:
    case LF_UNION_ST:
        tpi_union_dispose(&symbol->union_);
        break;

    case LF_BITFIELD:
        break;

    case LF_VTSHAPE:
        // TODO
        break;

    case LF_VFTABLE:
        // TODO
        break;

    case LF_VBCLASS:
    case LF_IVBCLASS:
        break;

    case LF_FIELDLIST:
        tpi_field_list_dispose(&symbol->field_list);
        break;

    case LF_ARGLIST:
        tpi_argument_list_dispose(&symbol->argument_list);
        break;

    case LF_METHODLIST:
        tpi_method_list_dispose(&symbol->method_list);
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled TPI symbol leaf: ", __FILE__, __LINE__);
        tpi_leaf_print(symbol->leaf, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
}

void tpi_symbol_print(struct tpi_symbol *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_SYMBOL_STRUCT
}

void tpi_symbols_read(
    struct tpi_symbols *symbols,
    struct msf *msf,
    struct msf_stream *msf_stream,
    struct tpi_header *tpi_header,
    FILE *file_stream)
{
    assert(symbols);
    assert(msf);
    assert(msf_stream);
    assert(tpi_header);
    assert(file_stream);

    //
    // Collect info about where each TPI symbol is located
    //

    struct tpi_symbol_info
    {
        uint32_t offset;
        uint16_t size;
    };

    uint32_t symbol_info_count = 0;
    struct tpi_symbol_info *symbol_info = NULL;

    uint32_t offset = tpi_header->header_size;

    while (offset < msf_stream->size)
    {
        struct tpi_symbol_info info;
        
        MSF_STREAM_READ(msf, msf_stream, &offset, info.size, file_stream);
        assert(info.size >= sizeof(uint16_t));

        info.offset = offset;

        DYNARRAY_PUSH(symbol_info, symbol_info_count, info);

        offset += info.size;
    }

    //
    // Pre-allocate all of the TPI symbols
    //

    symbols->count = symbol_info_count;
    symbols->symbols = calloc(symbols->count, sizeof(*symbols->symbols));
    assert(symbols->symbols);

    //
    // Read each TPI symbol
    //

    for (uint32_t i = 0; i < symbol_info_count; i++)
    {
        offset = symbol_info[i].offset;
        symbols->symbols[i].size = symbol_info[i].size;
        tpi_symbol_read(&symbols->symbols[i], msf, msf_stream, tpi_header, &offset, file_stream);
    }

    //
    // Cleanup
    //

    free(symbol_info);
}

void tpi_symbols_dispose(struct tpi_symbols *symbols)
{
    assert(symbols);

    for (uint32_t i = 0; i < symbols->count; i++)
        tpi_symbol_dispose(&symbols->symbols[i]);

    free(symbols->symbols);
}

void tpi_symbols_print(struct tpi_symbols *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    TPI_SYMBOLS_STRUCT
}

uint32_t tpi_index_to_absolute_index(struct tpi_header *tpi_header, struct tpi_symbols *tpi_symbols, uint32_t index)
{
    assert(tpi_header);
    assert(tpi_symbols);
    
    if (index < tpi_header->minimum_index || index >= tpi_header->maximum_index)
        return UINT32_MAX;
    
    uint32_t absolute_index = index - tpi_header->minimum_index;
    assert(absolute_index < tpi_symbols->count);

    return absolute_index;
}

int tpi_primitive_get(struct tpi_primitive *primitive, struct tpi_header *tpi_header, struct tpi_symbols *tpi_symbols, uint32_t index)
{
    assert(primitive);
    assert(tpi_header);
    assert(tpi_symbols);
    
    memset(primitive, 0, sizeof(*primitive));

    if (index >= 0x1000)
        return 0;
    
    uint32_t type = index & 0xff;

    switch (type)
    {
    case 0x00:
        primitive->type = TPI_PRIMITIVE_TYPE_NONE;
        break;
    case 0x03:
        primitive->type = TPI_PRIMITIVE_TYPE_VOID;
        break;
    case 0x08:
        primitive->type = TPI_PRIMITIVE_TYPE_HRESULT;
        break;
    case 0x10:
        primitive->type = TPI_PRIMITIVE_TYPE_CHAR;
        break;
    case 0x20:
        primitive->type = TPI_PRIMITIVE_TYPE_UCHAR;
        break;
    case 0x68:
        primitive->type = TPI_PRIMITIVE_TYPE_INT8;
        break;
    case 0x69:
        primitive->type = TPI_PRIMITIVE_TYPE_UINT8;
        break;
    case 0x70:
        primitive->type = TPI_PRIMITIVE_TYPE_RCHAR;
        break;
    case 0x71:
        primitive->type = TPI_PRIMITIVE_TYPE_WCHAR;
        break;
    case 0x7a:
        primitive->type = TPI_PRIMITIVE_TYPE_RCHAR16;
        break;
    case 0x7b:
        primitive->type = TPI_PRIMITIVE_TYPE_RCHAR32;
        break;
    case 0x11:
        primitive->type = TPI_PRIMITIVE_TYPE_SHORT;
        break;
    case 0x21:
        primitive->type = TPI_PRIMITIVE_TYPE_USHORT;
        break;
    case 0x72:
        primitive->type = TPI_PRIMITIVE_TYPE_INT16;
        break;
    case 0x73:
        primitive->type = TPI_PRIMITIVE_TYPE_UINT16;
        break;
    case 0x12:
        primitive->type = TPI_PRIMITIVE_TYPE_LONG;
        break;
    case 0x22:
        primitive->type = TPI_PRIMITIVE_TYPE_ULONG;
        break;
    case 0x74:
        primitive->type = TPI_PRIMITIVE_TYPE_INT32;
        break;
    case 0x75:
        primitive->type = TPI_PRIMITIVE_TYPE_UINT32;
        break;
    case 0x13:
        primitive->type = TPI_PRIMITIVE_TYPE_QUAD;
        break;
    case 0x23:
        primitive->type = TPI_PRIMITIVE_TYPE_UQUAD;
        break;
    case 0x76:
        primitive->type = TPI_PRIMITIVE_TYPE_INT64;
        break;
    case 0x77:
        primitive->type = TPI_PRIMITIVE_TYPE_UINT64;
        break;
    case 0x14:
        primitive->type = TPI_PRIMITIVE_TYPE_OCTA;
        break;
    case 0x24:
        primitive->type = TPI_PRIMITIVE_TYPE_UOCTA;
        break;
    case 0x78:
        primitive->type = TPI_PRIMITIVE_TYPE_INT128;
        break;
    case 0x79:
        primitive->type = TPI_PRIMITIVE_TYPE_UINT128;
        break;
    case 0x46:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT16;
        break;
    case 0x40:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT32;
        break;
    case 0x45:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT32_PP;
        break;
    case 0x44:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT48;
        break;
    case 0x41:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT64;
        break;
    case 0x42:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT80;
        break;
    case 0x43:
        primitive->type = TPI_PRIMITIVE_TYPE_FLOAT128;
        break;
    case 0x50:
        primitive->type = TPI_PRIMITIVE_TYPE_COMPLEX32;
        break;
    case 0x51:
        primitive->type = TPI_PRIMITIVE_TYPE_COMPLEX64;
        break;
    case 0x52:
        primitive->type = TPI_PRIMITIVE_TYPE_COMPLEX80;
        break;
    case 0x53:
        primitive->type = TPI_PRIMITIVE_TYPE_COMPLEX128;
        break;
    case 0x30:
        primitive->type = TPI_PRIMITIVE_TYPE_BOOL8;
        break;
    case 0x31:
        primitive->type = TPI_PRIMITIVE_TYPE_BOOL16;
        break;
    case 0x32:
        primitive->type = TPI_PRIMITIVE_TYPE_BOOL32;
        break;
    case 0x33:
        primitive->type = TPI_PRIMITIVE_TYPE_BOOL64;
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_primitive_type value: 0x%X\n", __FILE__, __LINE__, type);
        exit(EXIT_FAILURE);
    }
    
    uint32_t indirection = index & 0xf00;

    switch (indirection)
    {
    case 0x000:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_NONE;
        break;
    case 0x100:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_NEAR16;
        break;
    case 0x200:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_FAR16;
        break;
    case 0x300:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_HUGE16;
        break;
    case 0x400:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_NEAR32;
        break;
    case 0x500:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_FAR32;
        break;
    case 0x600:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_NEAR64;
        break;
    case 0x700:
        primitive->indirection = TPI_PRIMITIVE_INDIRECTION_NEAR128;
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_primitive_indirection value: 0x%X\n", __FILE__, __LINE__, indirection);
        exit(EXIT_FAILURE);
    }

    return 1;
}

struct tpi_symbol *tpi_symbol_get(struct tpi_header *tpi_header, struct tpi_symbols *tpi_symbols, uint32_t index)
{
    assert(tpi_header);
    assert(tpi_symbols);

    uint32_t absolute_index = tpi_index_to_absolute_index(tpi_header, tpi_symbols, index);

    if (absolute_index == UINT32_MAX)
        return NULL;
    
    return &tpi_symbols->symbols[absolute_index];
}
