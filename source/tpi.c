#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "tpi.h"

void tpi_slice_print(
    struct tpi_slice *slice,
    uint32_t depth,
    FILE *stream)
{
    assert(slice);
    assert(stream);

    fprintf(stream, "tpi_slice {\n");
    fprintf_depth(stream, depth + 1, "offset: %u,\n", slice->offset);
    fprintf_depth(stream, depth + 1, "size: %u,\n", slice->size);
    fprintf_depth(stream, depth, "}");
}

void tpi_header_read(
    struct msf *msf,
    struct tpi_header *out_header,
    FILE *stream)
{
    msf_stream_read_data(msf, &msf->streams[MSF_STREAM_TPI], 0, sizeof(*out_header), out_header, stream);
}

void tpi_header_print(
    struct tpi_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_header {\n");
    fprintf_depth(stream, depth + 1, "version: %u,\n", header->version);
    fprintf_depth(stream, depth + 1, "header_size: %u,\n", header->header_size);
    fprintf_depth(stream, depth + 1, "minimum_index: %u,\n", header->minimum_index);
    fprintf_depth(stream, depth + 1, "maximum_index: %u,\n", header->maximum_index);
    fprintf_depth(stream, depth + 1, "gprec_size: %u,\n", header->gprec_size);
    fprintf_depth(stream, depth + 1, "tpi_hash_stream: %u,\n", header->tpi_hash_stream);
    fprintf_depth(stream, depth + 1, "tpi_hash_pad_stream: %u,\n", header->tpi_hash_pad_stream);
    fprintf_depth(stream, depth + 1, "hash_key_size: %u,\n", header->hash_key_size);
    fprintf_depth(stream, depth + 1, "hash_bucket_size: %u,\n", header->hash_bucket_size);

    fprintf_depth(stream, depth + 1, "hash_values: ");
    tpi_slice_print(&header->hash_values, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "ti_off: ");
    tpi_slice_print(&header->ti_off, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "hash_adj: ");
    tpi_slice_print(&header->hash_adj, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth, "}");
}

void tpi_leaf_print(
    enum tpi_leaf leaf,
    FILE *stream)
{
    assert(stream);

    switch (leaf)
    {
    case LF_MODIFIER_16t:
        fprintf(stream, "LF_MODIFIER_16t");
        break;
    case LF_POINTER_16t:
        fprintf(stream, "LF_POINTER_16t");
        break;
    case LF_ARRAY_16t:
        fprintf(stream, "LF_ARRAY_16t");
        break;
    case LF_CLASS_16t:
        fprintf(stream, "LF_CLASS_16t");
        break;
    case LF_STRUCTURE_16t:
        fprintf(stream, "LF_STRUCTURE_16t");
        break;
    case LF_UNION_16t:
        fprintf(stream, "LF_UNION_16t");
        break;
    case LF_ENUM_16t:
        fprintf(stream, "LF_ENUM_16t");
        break;
    case LF_PROCEDURE_16t:
        fprintf(stream, "LF_PROCEDURE_16t");
        break;
    case LF_MFUNCTION_16t:
        fprintf(stream, "LF_MFUNCTION_16t");
        break;
    case LF_VTSHAPE:
        fprintf(stream, "LF_VTSHAPE");
        break;
    case LF_COBOL0_16t:
        fprintf(stream, "LF_COBOL0_16t");
        break;
    case LF_COBOL1:
        fprintf(stream, "LF_COBOL1");
        break;
    case LF_BARRAY_16t:
        fprintf(stream, "LF_BARRAY_16t");
        break;
    case LF_LABEL:
        fprintf(stream, "LF_LABEL");
        break;
    case LF_NULL:
        fprintf(stream, "LF_NULL");
        break;
    case LF_NOTTRAN:
        fprintf(stream, "LF_NOTTRAN");
        break;
    case LF_DIMARRAY_16t:
        fprintf(stream, "LF_DIMARRAY_16t");
        break;
    case LF_VFTPATH_16t:
        fprintf(stream, "LF_VFTPATH_16t");
        break;
    case LF_PRECOMP_16t:
        fprintf(stream, "LF_PRECOMP_16t");
        break;
    case LF_ENDPRECOMP:
        fprintf(stream, "LF_ENDPRECOMP");
        break;
    case LF_OEM_16t:
        fprintf(stream, "LF_OEM_16t");
        break;
    case LF_TYPESERVER_ST:
        fprintf(stream, "LF_TYPESERVER_ST");
        break;
    case LF_SKIP_16t:
        fprintf(stream, "LF_SKIP_16t");
        break;
    case LF_ARGLIST_16t:
        fprintf(stream, "LF_ARGLIST_16t");
        break;
    case LF_DEFARG_16t:
        fprintf(stream, "LF_DEFARG_16t");
        break;
    case LF_LIST:
        fprintf(stream, "LF_LIST");
        break;
    case LF_FIELDLIST_16t:
        fprintf(stream, "LF_FIELDLIST_16t");
        break;
    case LF_DERIVED_16t:
        fprintf(stream, "LF_DERIVED_16t");
        break;
    case LF_BITFIELD_16t:
        fprintf(stream, "LF_BITFIELD_16t");
        break;
    case LF_METHODLIST_16t:
        fprintf(stream, "LF_METHODLIST_16t");
        break;
    case LF_DIMCONU_16t:
        fprintf(stream, "LF_DIMCONU_16t");
        break;
    case LF_DIMCONLU_16t:
        fprintf(stream, "LF_DIMCONLU_16t");
        break;
    case LF_DIMVARU_16t:
        fprintf(stream, "LF_DIMVARU_16t");
        break;
    case LF_DIMVARLU_16t:
        fprintf(stream, "LF_DIMVARLU_16t");
        break;
    case LF_REFSYM:
        fprintf(stream, "LF_REFSYM");
        break;
    case LF_BCLASS_16t:
        fprintf(stream, "LF_BCLASS_16t");
        break;
    case LF_VBCLASS_16t:
        fprintf(stream, "LF_VBCLASS_16t");
        break;
    case LF_IVBCLASS_16t:
        fprintf(stream, "LF_IVBCLASS_16t");
        break;
    case LF_ENUMERATE_ST:
        fprintf(stream, "LF_ENUMERATE_ST");
        break;
    case LF_FRIENDFCN_16t:
        fprintf(stream, "LF_FRIENDFCN_16t");
        break;
    case LF_INDEX_16t:
        fprintf(stream, "LF_INDEX_16t");
        break;
    case LF_MEMBER_16t:
        fprintf(stream, "LF_MEMBER_16t");
        break;
    case LF_STMEMBER_16t:
        fprintf(stream, "LF_STMEMBER_16t");
        break;
    case LF_METHOD_16t:
        fprintf(stream, "LF_METHOD_16t");
        break;
    case LF_NESTTYPE_16t:
        fprintf(stream, "LF_NESTTYPE_16t");
        break;
    case LF_VFUNCTAB_16t:
        fprintf(stream, "LF_VFUNCTAB_16t");
        break;
    case LF_FRIENDCLS_16t:
        fprintf(stream, "LF_FRIENDCLS_16t");
        break;
    case LF_ONEMETHOD_16t:
        fprintf(stream, "LF_ONEMETHOD_16t");
        break;
    case LF_VFUNCOFF_16t:
        fprintf(stream, "LF_VFUNCOFF_16t");
        break;
    case LF_TI16_MAX:
        fprintf(stream, "LF_TI16_MAX");
        break;
    case LF_MODIFIER:
        fprintf(stream, "LF_MODIFIER");
        break;
    case LF_POINTER:
        fprintf(stream, "LF_POINTER");
        break;
    case LF_ARRAY_ST:
        fprintf(stream, "LF_ARRAY_ST");
        break;
    case LF_CLASS_ST:
        fprintf(stream, "LF_CLASS_ST");
        break;
    case LF_STRUCTURE_ST:
        fprintf(stream, "LF_STRUCTURE_ST");
        break;
    case LF_UNION_ST:
        fprintf(stream, "LF_UNION_ST");
        break;
    case LF_ENUM_ST:
        fprintf(stream, "LF_ENUM_ST");
        break;
    case LF_PROCEDURE:
        fprintf(stream, "LF_PROCEDURE");
        break;
    case LF_MFUNCTION:
        fprintf(stream, "LF_MFUNCTION");
        break;
    case LF_COBOL0:
        fprintf(stream, "LF_COBOL0");
        break;
    case LF_BARRAY:
        fprintf(stream, "LF_BARRAY");
        break;
    case LF_DIMARRAY_ST:
        fprintf(stream, "LF_DIMARRAY_ST");
        break;
    case LF_VFTPATH:
        fprintf(stream, "LF_VFTPATH");
        break;
    case LF_PRECOMP_ST:
        fprintf(stream, "LF_PRECOMP_ST");
        break;
    case LF_OEM:
        fprintf(stream, "LF_OEM");
        break;
    case LF_ALIAS_ST:
        fprintf(stream, "LF_ALIAS_ST");
        break;
    case LF_OEM2:
        fprintf(stream, "LF_OEM2");
        break;
    case LF_SKIP:
        fprintf(stream, "LF_SKIP");
        break;
    case LF_ARGLIST:
        fprintf(stream, "LF_ARGLIST");
        break;
    case LF_DEFARG_ST:
        fprintf(stream, "LF_DEFARG_ST");
        break;
    case LF_FIELDLIST:
        fprintf(stream, "LF_FIELDLIST");
        break;
    case LF_DERIVED:
        fprintf(stream, "LF_DERIVED");
        break;
    case LF_BITFIELD:
        fprintf(stream, "LF_BITFIELD");
        break;
    case LF_METHODLIST:
        fprintf(stream, "LF_METHODLIST");
        break;
    case LF_DIMCONU:
        fprintf(stream, "LF_DIMCONU");
        break;
    case LF_DIMCONLU:
        fprintf(stream, "LF_DIMCONLU");
        break;
    case LF_DIMVARU:
        fprintf(stream, "LF_DIMVARU");
        break;
    case LF_DIMVARLU:
        fprintf(stream, "LF_DIMVARLU");
        break;
    case LF_BCLASS:
        fprintf(stream, "LF_BCLASS");
        break;
    case LF_VBCLASS:
        fprintf(stream, "LF_VBCLASS");
        break;
    case LF_IVBCLASS:
        fprintf(stream, "LF_IVBCLASS");
        break;
    case LF_FRIENDFCN_ST:
        fprintf(stream, "LF_FRIENDFCN_ST");
        break;
    case LF_INDEX:
        fprintf(stream, "LF_INDEX");
        break;
    case LF_MEMBER_ST:
        fprintf(stream, "LF_MEMBER_ST");
        break;
    case LF_STMEMBER_ST:
        fprintf(stream, "LF_STMEMBER_ST");
        break;
    case LF_METHOD_ST:
        fprintf(stream, "LF_METHOD_ST");
        break;
    case LF_NESTTYPE_ST:
        fprintf(stream, "LF_NESTTYPE_ST");
        break;
    case LF_VFUNCTAB:
        fprintf(stream, "LF_VFUNCTAB");
        break;
    case LF_FRIENDCLS:
        fprintf(stream, "LF_FRIENDCLS");
        break;
    case LF_ONEMETHOD_ST:
        fprintf(stream, "LF_ONEMETHOD_ST");
        break;
    case LF_VFUNCOFF:
        fprintf(stream, "LF_VFUNCOFF");
        break;
    case LF_NESTTYPEEX_ST:
        fprintf(stream, "LF_NESTTYPEEX_ST");
        break;
    case LF_MEMBERMODIFY_ST:
        fprintf(stream, "LF_MEMBERMODIFY_ST");
        break;
    case LF_MANAGED_ST:
        fprintf(stream, "LF_MANAGED_ST");
        break;
    case LF_ST_MAX:
        fprintf(stream, "LF_ST_MAX");
        break;
    case LF_TYPESERVER:
        fprintf(stream, "LF_TYPESERVER");
        break;
    case LF_ENUMERATE:
        fprintf(stream, "LF_ENUMERATE");
        break;
    case LF_ARRAY:
        fprintf(stream, "LF_ARRAY");
        break;
    case LF_CLASS:
        fprintf(stream, "LF_CLASS");
        break;
    case LF_STRUCTURE:
        fprintf(stream, "LF_STRUCTURE");
        break;
    case LF_UNION:
        fprintf(stream, "LF_UNION");
        break;
    case LF_ENUM:
        fprintf(stream, "LF_ENUM");
        break;
    case LF_DIMARRAY:
        fprintf(stream, "LF_DIMARRAY");
        break;
    case LF_PRECOMP:
        fprintf(stream, "LF_PRECOMP");
        break;
    case LF_ALIAS:
        fprintf(stream, "LF_ALIAS");
        break;
    case LF_DEFARG:
        fprintf(stream, "LF_DEFARG");
        break;
    case LF_FRIENDFCN:
        fprintf(stream, "LF_FRIENDFCN");
        break;
    case LF_MEMBER:
        fprintf(stream, "LF_MEMBER");
        break;
    case LF_STMEMBER:
        fprintf(stream, "LF_STMEMBER");
        break;
    case LF_METHOD:
        fprintf(stream, "LF_METHOD");
        break;
    case LF_NESTTYPE:
        fprintf(stream, "LF_NESTTYPE");
        break;
    case LF_ONEMETHOD:
        fprintf(stream, "LF_ONEMETHOD");
        break;
    case LF_NESTTYPEEX:
        fprintf(stream, "LF_NESTTYPEEX");
        break;
    case LF_MEMBERMODIFY:
        fprintf(stream, "LF_MEMBERMODIFY");
        break;
    case LF_MANAGED:
        fprintf(stream, "LF_MANAGED");
        break;
    case LF_TYPESERVER2:
        fprintf(stream, "LF_TYPESERVER2");
        break;
    case LF_STRIDED_ARRAY:
        fprintf(stream, "LF_STRIDED_ARRAY");
        break;
    case LF_HLSL:
        fprintf(stream, "LF_HLSL");
        break;
    case LF_MODIFIER_EX:
        fprintf(stream, "LF_MODIFIER_EX");
        break;
    case LF_INTERFACE:
        fprintf(stream, "LF_INTERFACE");
        break;
    case LF_BINTERFACE:
        fprintf(stream, "LF_BINTERFACE");
        break;
    case LF_VECTOR:
        fprintf(stream, "LF_VECTOR");
        break;
    case LF_MATRIX:
        fprintf(stream, "LF_MATRIX");
        break;
    case LF_VFTABLE:
        fprintf(stream, "LF_VFTABLE");
        break;
    // case LF_ENDOFLEAFRECORD:
    //     fprintf(stream, "LF_ENDOFLEAFRECORD");
    //     break;
    case LF_TYPE_LAST:
        fprintf(stream, "LF_TYPE_LAST");
        break;
    // case LF_TYPE_MAX:
    //     fprintf(stream, "LF_TYPE_MAX");
    //     break;
    case LF_FUNC_ID:
        fprintf(stream, "LF_FUNC_ID");
        break;
    case LF_MFUNC_ID:
        fprintf(stream, "LF_MFUNC_ID");
        break;
    case LF_BUILDINFO:
        fprintf(stream, "LF_BUILDINFO");
        break;
    case LF_SUBSTR_LIST:
        fprintf(stream, "LF_SUBSTR_LIST");
        break;
    case LF_STRING_ID:
        fprintf(stream, "LF_STRING_ID");
        break;
    case LF_UDT_SRC_LINE:
        fprintf(stream, "LF_UDT_SRC_LINE");
        break;
    case LF_UDT_MOD_SRC_LINE:
        fprintf(stream, "LF_UDT_MOD_SRC_LINE");
        break;
    case LF_STRUCTURE19:
        fprintf(stream, "LF_STRUCTURE19");
        break;
    case LF_ID_LAST:
        fprintf(stream, "LF_ID_LAST");
        break;
    // case LF_ID_MAX:
    //     fprintf(stream, "LF_ID_MAX");
    //     break;
    // case LF_NUMERIC:
    //     fprintf(stream, "LF_NUMERIC");
    //     break;
    case LF_CHAR:
        fprintf(stream, "LF_CHAR");
        break;
    case LF_SHORT:
        fprintf(stream, "LF_SHORT");
        break;
    case LF_USHORT:
        fprintf(stream, "LF_USHORT");
        break;
    case LF_LONG:
        fprintf(stream, "LF_LONG");
        break;
    case LF_ULONG:
        fprintf(stream, "LF_ULONG");
        break;
    case LF_REAL32:
        fprintf(stream, "LF_REAL32");
        break;
    case LF_REAL64:
        fprintf(stream, "LF_REAL64");
        break;
    case LF_REAL80:
        fprintf(stream, "LF_REAL80");
        break;
    case LF_REAL128:
        fprintf(stream, "LF_REAL128");
        break;
    case LF_QUADWORD:
        fprintf(stream, "LF_QUADWORD");
        break;
    case LF_UQUADWORD:
        fprintf(stream, "LF_UQUADWORD");
        break;
    case LF_REAL48:
        fprintf(stream, "LF_REAL48");
        break;
    case LF_COMPLEX32:
        fprintf(stream, "LF_COMPLEX32");
        break;
    case LF_COMPLEX64:
        fprintf(stream, "LF_COMPLEX64");
        break;
    case LF_COMPLEX80:
        fprintf(stream, "LF_COMPLEX80");
        break;
    case LF_COMPLEX128:
        fprintf(stream, "LF_COMPLEX128");
        break;
    case LF_VARSTRING:
        fprintf(stream, "LF_VARSTRING");
        break;
    case LF_OCTWORD:
        fprintf(stream, "LF_OCTWORD");
        break;
    case LF_UOCTWORD:
        fprintf(stream, "LF_UOCTWORD");
        break;
    case LF_DECIMAL:
        fprintf(stream, "LF_DECIMAL");
        break;
    case LF_DATE:
        fprintf(stream, "LF_DATE");
        break;
    case LF_UTF8STRING:
        fprintf(stream, "LF_UTF8STRING");
        break;
    case LF_REAL16:
        fprintf(stream, "LF_REAL16");
        break;
    case LF_PAD0:
        fprintf(stream, "LF_PAD0");
        break;
    case LF_PAD1:
        fprintf(stream, "LF_PAD1");
        break;
    case LF_PAD2:
        fprintf(stream, "LF_PAD2");
        break;
    case LF_PAD3:
        fprintf(stream, "LF_PAD3");
        break;
    case LF_PAD4:
        fprintf(stream, "LF_PAD4");
        break;
    case LF_PAD5:
        fprintf(stream, "LF_PAD5");
        break;
    case LF_PAD6:
        fprintf(stream, "LF_PAD6");
        break;
    case LF_PAD7:
        fprintf(stream, "LF_PAD7");
        break;
    case LF_PAD8:
        fprintf(stream, "LF_PAD8");
        break;
    case LF_PAD9:
        fprintf(stream, "LF_PAD9");
        break;
    case LF_PAD10:
        fprintf(stream, "LF_PAD10");
        break;
    case LF_PAD11:
        fprintf(stream, "LF_PAD11");
        break;
    case LF_PAD12:
        fprintf(stream, "LF_PAD12");
        break;
    case LF_PAD13:
        fprintf(stream, "LF_PAD13");
        break;
    case LF_PAD14:
        fprintf(stream, "LF_PAD14");
        break;
    case LF_PAD15:
        fprintf(stream, "LF_PAD15");
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_leaf value: %i\n", __FILE__, __LINE__, leaf);
        exit(EXIT_FAILURE);
    }
}

void tpi_properties_print(
    struct tpi_properties *properties,
    uint32_t depth,
    FILE *stream)
{
    assert(properties);
    assert(stream);

    fprintf(stream, "tpi_properties {\n");
    fprintf_depth(stream, depth + 1, "packed: %u,\n", properties->packed);
    fprintf_depth(stream, depth + 1, "ctor: %u,\n", properties->ctor);
    fprintf_depth(stream, depth + 1, "ovlops: %u,\n", properties->ovlops);
    fprintf_depth(stream, depth + 1, "isnested: %u,\n", properties->isnested);
    fprintf_depth(stream, depth + 1, "cnested: %u,\n", properties->cnested);
    fprintf_depth(stream, depth + 1, "opassign: %u,\n", properties->opassign);
    fprintf_depth(stream, depth + 1, "opcast: %u,\n", properties->opcast);
    fprintf_depth(stream, depth + 1, "fwdref: %u,\n", properties->fwdref);
    fprintf_depth(stream, depth + 1, "scoped: %u,\n", properties->scoped);
    fprintf_depth(stream, depth + 1, "hasuniquename: %u,\n", properties->hasuniquename);
    fprintf_depth(stream, depth + 1, "sealed: %u,\n", properties->sealed);
    fprintf_depth(stream, depth + 1, "hfa: %u,\n", properties->hfa);
    fprintf_depth(stream, depth + 1, "intrinsic: %u,\n", properties->intrinsic);
    fprintf_depth(stream, depth + 1, "mocom: %u,\n", properties->mocom);
    fprintf_depth(stream, depth, "}");
}

void tpi_member_header_print(
    struct tpi_member_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_member_header {\n");
    fprintf_depth(stream, depth + 1, "attributes: %u,\n", header->attributes);
    fprintf_depth(stream, depth + 1, "field_type: %u,\n", header->field_type);
    fprintf_depth(stream, depth, "}");
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

    item->offset = msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);
}

void tpi_member_dispose(
    struct tpi_member *item)
{
    assert(item);

    free(item->name);
}

void tpi_member_print(
    struct tpi_member *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_member {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_member_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "offset: %llu,\n", item->offset);
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_static_member_header_print(
    struct tpi_static_member_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_static_member_header {\n");

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_field_attributes_print(&header->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "field_type_index: %u,\n", header->field_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_static_member_dispose(struct tpi_static_member *item)
{
    assert(item);

    free(item->name);
}

void tpi_static_member_print(
    struct tpi_static_member *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_static_member {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_static_member_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_pointer_attributes_print(
    struct tpi_pointer_attributes *attributes,
    uint32_t depth,
    FILE *stream)
{
    assert(attributes);
    assert(stream);

    fprintf(stream, "tpi_pointer_attributes {\n");
    fprintf_depth(stream, depth + 1, "ptrtype: %u,\n", attributes->ptrtype);
    fprintf_depth(stream, depth + 1, "ptrmode: %u,\n", attributes->ptrmode);
    fprintf_depth(stream, depth + 1, "isflat32: %u,\n", attributes->isflat32);
    fprintf_depth(stream, depth + 1, "isvolatile: %u,\n", attributes->isvolatile);
    fprintf_depth(stream, depth + 1, "isconst: %u,\n", attributes->isconst);
    fprintf_depth(stream, depth + 1, "isunaligned: %u,\n", attributes->isunaligned);
    fprintf_depth(stream, depth + 1, "isrestrict: %u,\n", attributes->isrestrict);
    fprintf_depth(stream, depth + 1, "size: %u,\n", attributes->size);
    fprintf_depth(stream, depth + 1, "ismocom: %u,\n", attributes->ismocom);
    fprintf_depth(stream, depth + 1, "islref: %u,\n", attributes->islref);
    fprintf_depth(stream, depth + 1, "isrref: %u,\n", attributes->isrref);
    fprintf_depth(stream, depth + 1, "unused: %u,\n", attributes->unused);
    fprintf_depth(stream, depth, "}");
}

void tpi_pointer_header_print(
    struct tpi_pointer_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_pointer_header {\n");
    fprintf_depth(stream, depth + 1, "underlying_type_index: %u,\n", header->underlying_type_index);

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_pointer_attributes_print(&header->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth, "}");
}

void tpi_pointer_print(
    struct tpi_pointer *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_pointer {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_pointer_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "containing_class_type_index: %u,\n", item->containing_class_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_argument_list_dispose(
    struct tpi_argument_list *item)
{
    assert(item);

    free(item->type_indices);
}

void tpi_argument_list_print(
    struct tpi_argument_list *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_argument_list {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", item->count);
    fprintf_depth(stream, depth + 1, "type_indices: [");
    for (uint32_t i = 0; i < item->count; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");

        fprintf(stream, "%u", item->type_indices[i]);
    }
    fprintf(stream, "],");
    fprintf(stream, "\n");
    fprintf_depth(stream, depth, "}");
}

void tpi_procedure_attributes_print(
    struct tpi_procedure_attributes *attributes,
    uint32_t depth,
    FILE *stream)
{
    assert(attributes);
    assert(stream);

    fprintf(stream, "tpi_procedure_attributes {\n");
    fprintf_depth(stream, depth + 1, "cxxreturnudt: %u,\n", attributes->cxxreturnudt);
    fprintf_depth(stream, depth + 1, "ctor: %u,\n", attributes->ctor);
    fprintf_depth(stream, depth + 1, "ctorvbase: %u,\n", attributes->ctorvbase);
    fprintf_depth(stream, depth + 1, "unused: %u,\n", attributes->unused);
    fprintf_depth(stream, depth, "}");
}

void tpi_procedure_print(
    struct tpi_procedure *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_procedure {\n");
    fprintf_depth(stream, depth + 1, "return_type_index: %u,\n", item->return_type_index);

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_procedure_attributes_print(&item->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "parameter_count: %u,\n", item->parameter_count);
    fprintf_depth(stream, depth + 1, "argument_list_type_index: %u,\n", item->argument_list_type_index);
    fprintf_depth(stream, depth + 1, "padding: %u,\n", item->padding);
    fprintf_depth(stream, depth, "}");
}

void tpi_modifier_print(
    struct tpi_modifier *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_modifier {\n");
    fprintf_depth(stream, depth + 1, "underlying_type_index: %u,\n", item->underlying_type_index);
    fprintf_depth(stream, depth + 1, "flags: %u,\n", item->flags);
    fprintf_depth(stream, depth, "}");
}

void tpi_enumerate_variant_type_print(
    enum tpi_enumerate_variant_type type,
    FILE *stream)
{
    assert(stream);

    switch (type)
    {
    case TPI_ENUMERATE_VARIANT_UINT8:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_UINT8");
        break;

    case TPI_ENUMERATE_VARIANT_UINT16:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_UINT16");
        break;

    case TPI_ENUMERATE_VARIANT_UINT32:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_UINT32");
        break;

    case TPI_ENUMERATE_VARIANT_UINT64:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_UINT64");
        break;

    case TPI_ENUMERATE_VARIANT_INT8:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_INT8");
        break;

    case TPI_ENUMERATE_VARIANT_INT16:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_INT16");
        break;

    case TPI_ENUMERATE_VARIANT_INT32:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_INT32");
        break;

    case TPI_ENUMERATE_VARIANT_INT64:
        fprintf(stream, "TPI_ENUMERATE_VARIANT_INT64");
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_enumerate_variant_type value: %i\n", __FILE__, __LINE__, type);
        exit(EXIT_FAILURE);
    }
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
}

void tpi_enumerate_variant_print(
    struct tpi_enumerate_variant *variant,
    uint32_t depth,
    FILE *stream)
{
    assert(variant);
    assert(stream);

    fprintf(stream, "tpi_enumerate_variant {\n");

    fprintf_depth(stream, depth + 1, "type: ");
    tpi_enumerate_variant_type_print(variant->type, stream);
    fprintf(stream, ",\n");

    switch (variant->type)
    {
    case TPI_ENUMERATE_VARIANT_UINT8:
        fprintf_depth(stream, depth + 1, "uint8: %u,\n", variant->uint8);
        break;

    case TPI_ENUMERATE_VARIANT_UINT16:
        fprintf_depth(stream, depth + 1, "uint16: %u,\n", variant->uint16);
        break;

    case TPI_ENUMERATE_VARIANT_UINT32:
        fprintf_depth(stream, depth + 1, "uint32: %u,\n", variant->uint32);
        break;

    case TPI_ENUMERATE_VARIANT_UINT64:
        fprintf_depth(stream, depth + 1, "uint64: %llu,\n", variant->uint64);
        break;

    case TPI_ENUMERATE_VARIANT_INT8:
        fprintf_depth(stream, depth + 1, "int8: %i,\n", variant->int8);
        break;

    case TPI_ENUMERATE_VARIANT_INT16:
        fprintf_depth(stream, depth + 1, "int16: %i,\n", variant->int16);
        break;

    case TPI_ENUMERATE_VARIANT_INT32:
        fprintf_depth(stream, depth + 1, "int32: %i,\n", variant->int32);
        break;

    case TPI_ENUMERATE_VARIANT_INT64:
        fprintf_depth(stream, depth + 1, "int64: %lli,\n", variant->int64);
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_enumerate_variant_type value: %i\n", __FILE__, __LINE__, variant->type);
        exit(EXIT_FAILURE);
    }

    fprintf_depth(stream, depth, "}");
}

void tpi_enumerate_dispose(
    struct tpi_enumerate *item)
{
    assert(item);

    free(item->name);
}

void tpi_enumerate_print(
    struct tpi_enumerate *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_enumerate {\n");

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_field_attributes_print(&item->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "variant: ");
    tpi_enumerate_variant_print(&item->variant, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_enumeration_header_print(
    struct tpi_enumeration_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_enumeration_header {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", header->count);

    fprintf_depth(stream, depth + 1, "properties: ");
    tpi_properties_print(&header->properties, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "underlying_type_index: %u,\n", header->underlying_type_index);
    fprintf_depth(stream, depth + 1, "fields_type_index: %u,\n", header->fields_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_enumeration_dispose(
    struct tpi_enumeration *item)
{
    assert(item);

    free(item->name);
    free(item->unique_name);
}

void tpi_enumeration_print(
    struct tpi_enumeration *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_enumeration {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_enumeration_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth + 1, "unique_name: \"%s\",\n", item->unique_name);
    fprintf_depth(stream, depth, "}");
}

void tpi_array_header_print(struct tpi_array_header *header, uint32_t depth, FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_array_header {\n");
    fprintf_depth(stream, depth + 1, "element_type_index: %u,\n", header->element_type_index);
    fprintf_depth(stream, depth + 1, "indexing_type_index: %u,\n", header->indexing_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_array_dispose(struct tpi_array *item)
{
    assert(item);

    free(item->dimensions);
}

void tpi_array_print(struct tpi_array *item, uint32_t depth, FILE *stream)
{
    assert(item);
    fprintf(stream, "tpi_array {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_array_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "stride: %u,\n", item->stride);
    fprintf_depth(stream, depth + 1, "dimension_count: %u,\n", item->dimension_count);

    fprintf_depth(stream, depth + 1, "dimensions: [");
    for (uint32_t i = 0; i < item->dimension_count; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");

        fprintf(stream, "%u", item->dimensions[i]);
    }
    fprintf(stream, "],\n");
    fprintf_depth(stream, depth, "}");
}

void tpi_field_attributes_print(
    struct tpi_field_attributes *attributes,
    uint32_t depth,
    FILE *stream)
{
    assert(attributes);
    assert(stream);

    fprintf(stream, "tpi_field_attributes {\n");
    fprintf_depth(stream, depth + 1, "access: %u,\n", attributes->access);
    fprintf_depth(stream, depth + 1, "mprop: %u,\n", attributes->mprop);
    fprintf_depth(stream, depth + 1, "pseudo: %u,\n", attributes->pseudo);
    fprintf_depth(stream, depth + 1, "noinherit: %u,\n", attributes->noinherit);
    fprintf_depth(stream, depth + 1, "noconstruct: %u,\n", attributes->noconstruct);
    fprintf_depth(stream, depth + 1, "compgenx: %u,\n", attributes->compgenx);
    fprintf_depth(stream, depth + 1, "sealed: %u,\n", attributes->sealed);
    fprintf_depth(stream, depth + 1, "unused: %u,\n", attributes->unused);
    fprintf_depth(stream, depth, "}");
}

void tpi_field_list_dispose(
    struct tpi_field_list *item)
{
    assert(item);

    for (uint32_t i = 0; i < item->count; i++)
        tpi_symbol_dispose(&item->fields[i]);

    free(item->fields);
}

void tpi_field_list_print(
    struct tpi_field_list *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_field_list {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", item->count);
    fprintf_depth(stream, depth + 1, "fields: [\n");
    for (uint32_t i = 0; i < item->count; i++)
    {
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        tpi_symbol_print(&item->fields[i], depth + 2, stream);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}

void tpi_union_header_print(
    struct tpi_union_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_union_header {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", header->count);

    fprintf_depth(stream, depth + 1, "properties: ");
    tpi_properties_print(&header->properties, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "fields_type_index: %u,\n", header->fields_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_union_dispose(
    struct tpi_union *item)
{
    assert(item);

    free(item->name);
    free(item->unique_name);
}

void tpi_union_print(
    struct tpi_union *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_union {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_union_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "size: %llu,\n", item->size);
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth + 1, "unique_name: \"%s\",\n", item->unique_name);
    fprintf_depth(stream, depth, "}");
}

void tpi_bitfield_print(
    struct tpi_bitfield *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_bitfield {\n");
    fprintf_depth(stream, depth + 1, "underlying_type_index: %u,\n", item->underlying_type_index);
    fprintf_depth(stream, depth + 1, "length: %u,\n", item->length);
    fprintf_depth(stream, depth + 1, "position: %u,\n", item->position);
    fprintf_depth(stream, depth + 1, "padding: %u,\n", item->padding);
    fprintf_depth(stream, depth, "}");
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

void tpi_member_function_print(
    struct tpi_member_function *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_member_function {\n");
    fprintf_depth(stream, depth + 1, "return_type_index: %u,\n", item->return_type_index);
    fprintf_depth(stream, depth + 1, "class_type_index: %u,\n", item->class_type_index);
    fprintf_depth(stream, depth + 1, "this_pointer_type_index: %u,\n", item->this_pointer_type_index);
    fprintf_depth(stream, depth + 1, "attributes: %u,\n", item->attributes);
    fprintf_depth(stream, depth + 1, "parameter_count: %u,\n", item->parameter_count);
    fprintf_depth(stream, depth + 1, "argument_list_type_index: %u,\n", item->argument_list_type_index);
    fprintf_depth(stream, depth + 1, "this_adjustment: %u,\n", item->this_adjustment);
    fprintf_depth(stream, depth, "}");
}

void tpi_overloaded_method_header_print(
    struct tpi_overloaded_method_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_overloaded_method_header {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", header->count);
    fprintf_depth(stream, depth + 1, "method_list_type_index: %u,\n", header->method_list_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_overloaded_method_dispose(
    struct tpi_overloaded_method *item)
{
    assert(item);

    free(item->name);
}

void tpi_overloaded_method_print(
    struct tpi_overloaded_method *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_overloaded_method {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_overloaded_method_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_method_header_print(
    struct tpi_method_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_method_header {\n");

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_field_attributes_print(&header->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "method_type_index: %u,\n", header->method_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_method_dispose(
    struct tpi_method *item)
{
    assert(item);

    free(item->name);
}

void tpi_method_print(
    struct tpi_method *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_method {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_method_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "vtable_offset: %u,\n", item->vtable_offset);
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_method_list_entry_header_print(
    struct tpi_method_list_entry_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_method_list_entry_header {\n");

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_field_attributes_print(&header->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "padding: %u,\n", header->padding);
    fprintf_depth(stream, depth + 1, "method_type_index: %u,\n", header->method_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_method_list_entry_print(
    struct tpi_method_list_entry *entry,
    uint32_t depth,
    FILE *stream)
{
    assert(entry);
    assert(stream);

    fprintf(stream, "tpi_method_list_entry {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_method_list_entry_header_print(&entry->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "vtable_offset: %u,\n", entry->vtable_offset);
    fprintf_depth(stream, depth, "}");
}

void tpi_method_list_dispose(
    struct tpi_method_list *item)
{
    assert(item);

    free(item->entries);
}

void tpi_method_list_print(
    struct tpi_method_list *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_method_list {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", item->count);
    fprintf_depth(stream, depth + 1, "entries: [\n");
    for (uint32_t i = 0; i < item->count; i++)
    {
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        tpi_method_list_entry_print(&item->entries[i], depth + 2, stream);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}

void tpi_vtable_print(
    struct tpi_vtable *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_vtable {\n");
    fprintf_depth(stream, depth + 1, "padding: %u,\n", item->padding);
    fprintf_depth(stream, depth + 1, "type_index: %u,\n", item->type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_nested_type_header_print(
    struct tpi_nested_type_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_nested_type_header {\n");
    fprintf_depth(stream, depth + 1, "attributes: %u,\n", header->attributes);
    fprintf_depth(stream, depth + 1, "nested_type_index: %u,\n", header->nested_type_index);
    fprintf_depth(stream, depth, "}");
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

void tpi_nested_type_dispose(
    struct tpi_nested_type *item)
{
    assert(item);

    free(item->name);
}

void tpi_nested_type_print(
    struct tpi_nested_type *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_nested_type {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_nested_type_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth, "}");
}

void tpi_base_class_header_print(
    struct tpi_base_class_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_base_class_header {\n");

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_field_attributes_print(&header->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "base_class_type_index: %u,\n", header->base_class_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_base_class_print(
    struct tpi_base_class *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_base_class {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_base_class_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "offset: %u,\n", item->offset);
    fprintf_depth(stream, depth, "}");
}

void tpi_virtual_base_class_header_print(
    struct tpi_virtual_base_class_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_virtual_base_class_header {\n");

    fprintf_depth(stream, depth + 1, "attributes: ");
    tpi_field_attributes_print(&header->attributes, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "base_class_type_index: %u,\n", header->base_class_type_index);
    fprintf_depth(stream, depth + 1, "base_pointer_type_index: %u,\n", header->base_pointer_type_index);
    fprintf_depth(stream, depth, "}");
}

void tpi_virtual_base_class_print(
    struct tpi_virtual_base_class *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_virtual_base_class {\n");

    fprintf_depth(stream, depth + 1, "header: ");
    tpi_virtual_base_class_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");

    fprintf_depth(stream, depth + 1, "base_pointer_offset: %u,\n", item->base_pointer_offset);
    fprintf_depth(stream, depth + 1, "virtual_base_offset: %u,\n", item->virtual_base_offset);
    fprintf_depth(stream, depth, "}");
}

void tpi_class_header_print(
    struct tpi_class_header *header,
    uint32_t depth,
    FILE *stream)
{
    assert(header);
    assert(stream);

    fprintf(stream, "tpi_class_header {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", header->count);
    fprintf_depth(stream, depth + 1, "properties: ");
    tpi_properties_print(&header->properties, depth + 1, stream);
    fprintf(stream, ",\n");
    fprintf_depth(stream, depth + 1, "fields_type_index: %u,\n", header->fields_type_index);
    fprintf_depth(stream, depth, "}");
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

    item->size = msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
    item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);

    if (item->header.properties.hasuniquename)
        item->unique_name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, leaf, file_stream);

    msf_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
}

void tpi_class_dispose(
    struct tpi_class *item)
{
    assert(item);

    free(item->name);
    free(item->unique_name);
}

void tpi_class_print(
    struct tpi_class *item,
    uint32_t depth,
    FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "tpi_class {\n");
    fprintf_depth(stream, depth + 1, "header: ");
    tpi_class_header_print(&item->header, depth + 1, stream);
    fprintf(stream, ",\n");
    fprintf_depth(stream, depth + 1, "derived_from_type_index: %u,\n", item->derived_from_type_index);
    fprintf_depth(stream, depth + 1, "vtable_shape_type_index: %u,\n", item->vtable_shape_type_index);
    fprintf_depth(stream, depth + 1, "size: %llu,\n", item->size);
    fprintf_depth(stream, depth + 1, "name: \"%s\",\n", item->name);
    fprintf_depth(stream, depth + 1, "unique_name: \"%s\",\n", item->unique_name);
    fprintf_depth(stream, depth, "}");
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
        item->offset = (uint32_t)msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
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
        msf_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
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
        msf_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
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
            uint64_t dimension = msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
            assert(dimension <= UINT32_MAX);
            assert(*out_offset < msf_stream->size);

            uint32_t index = item->dimension_count++;
            item->dimensions = realloc(item->dimensions, item->dimension_count * sizeof(*item->dimensions));
            assert(item->dimensions);
            item->dimensions[index] = dimension;

            uint8_t next = 0;
            msf_stream_read_data(msf, msf_stream, *out_offset, sizeof(next), &next, file_stream);

            if (next == 0)
            {
                *out_offset += 2;
                break;
            }
        }

        msf_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
        assert(*out_offset < msf_stream->size);
        break;
    }

    case LF_UNION:
    case LF_UNION_ST:
    {
        struct tpi_union *item = &symbol->union_;
        MSF_STREAM_READ(msf, msf_stream, out_offset, item->header, file_stream);

        item->size = msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
        item->name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);

        if (item->header.properties.hasuniquename)
            item->unique_name = msf_read_tpi_lf_string(msf, msf_stream, out_offset, symbol->leaf, file_stream);

        msf_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
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

        item->base_pointer_offset = (uint32_t)msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
        item->virtual_base_offset = (uint32_t)msf_read_tpi_lf_unsigned(msf, msf_stream, out_offset, file_stream);
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

            msf_read_padding(msf, msf_stream, end_offset, out_offset, file_stream);
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
            MSF_STREAM_READ(msf, msf_stream, out_offset, item->type_indices[item->count - 1], file_stream);
        
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

void tpi_symbol_print(struct tpi_symbol *symbol, uint32_t depth, FILE *stream)
{
    assert(symbol);
    assert(stream);

    fprintf(stream, "tpi_symbol {\n");

    fprintf_depth(stream, depth + 1, "leaf: ");
    tpi_leaf_print(symbol->leaf, stream);
    fprintf(stream, ",\n");

    switch (symbol->leaf)
    {
    case LF_CLASS:
    case LF_CLASS_ST:
    case LF_STRUCTURE:
    case LF_STRUCTURE_ST:
    case LF_STRUCTURE19:
    case LF_INTERFACE:
        fprintf_depth(stream, depth + 1, "class_: ");
        tpi_class_print(&symbol->class_, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_MEMBER:
    case LF_MEMBER_ST:
        fprintf_depth(stream, depth + 1, "member: ");
        tpi_member_print(&symbol->member, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_NESTTYPE:
    case LF_NESTTYPE_ST:
    case LF_NESTTYPEEX:
    case LF_NESTTYPEEX_ST:
        fprintf_depth(stream, depth + 1, "nested_type: ");
        tpi_nested_type_print(&symbol->nested_type, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_MFUNCTION:
        fprintf_depth(stream, depth + 1, "member_function: ");
        tpi_member_function_print(&symbol->member_function, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_METHOD:
    case LF_METHOD_ST:
        fprintf_depth(stream, depth + 1, "overloaded_method: ");
        tpi_overloaded_method_print(&symbol->overloaded_method, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_ONEMETHOD:
    case LF_ONEMETHOD_ST:
        fprintf_depth(stream, depth + 1, "method: ");
        tpi_method_print(&symbol->method, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_BCLASS:
    case LF_BINTERFACE:
        fprintf_depth(stream, depth + 1, "base_class: ");
        tpi_base_class_print(&symbol->base_class, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_VFUNCTAB:
        fprintf_depth(stream, depth + 1, "vtable: ");
        tpi_vtable_print(&symbol->vtable, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_STMEMBER:
    case LF_STMEMBER_ST:
        fprintf_depth(stream, depth + 1, "static_member: ");
        tpi_static_member_print(&symbol->static_member, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_POINTER:
        fprintf_depth(stream, depth + 1, "pointer: ");
        tpi_pointer_print(&symbol->pointer, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_PROCEDURE:
        fprintf_depth(stream, depth + 1, "procedure: ");
        tpi_procedure_print(&symbol->procedure, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_MODIFIER:
        fprintf_depth(stream, depth + 1, "modifier: ");
        tpi_modifier_print(&symbol->modifier, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_ENUM:
    case LF_ENUM_ST:
        fprintf_depth(stream, depth + 1, "enumeration: ");
        tpi_enumeration_print(&symbol->enumeration, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_ENUMERATE:
    case LF_ENUMERATE_ST:
        fprintf_depth(stream, depth + 1, "enumerate: ");
        tpi_enumerate_print(&symbol->enumerate, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_ARRAY:
    case LF_ARRAY_ST:
    case LF_STRIDED_ARRAY:
        fprintf_depth(stream, depth + 1, "array: ");
        tpi_array_print(&symbol->array, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_UNION:
    case LF_UNION_ST:
        fprintf_depth(stream, depth + 1, "union_: ");
        tpi_union_print(&symbol->union_, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_BITFIELD:
        fprintf_depth(stream, depth + 1, "bitfield: ");
        tpi_bitfield_print(&symbol->bitfield, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_VTSHAPE:
        // TODO
        break;

    case LF_VFTABLE:
        // TODO
        break;

    case LF_VBCLASS:
    case LF_IVBCLASS:
        fprintf_depth(stream, depth + 1, "virtual_base_class: ");
        tpi_virtual_base_class_print(&symbol->virtual_base_class, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_FIELDLIST:
        fprintf_depth(stream, depth + 1, "field_list: ");
        tpi_field_list_print(&symbol->field_list, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_ARGLIST:
        fprintf_depth(stream, depth + 1, "argument_list: ");
        tpi_argument_list_print(&symbol->argument_list, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    case LF_METHODLIST:
        fprintf_depth(stream, depth + 1, "method_list: ");
        tpi_method_list_print(&symbol->method_list, depth + 1, stream);
        fprintf(stream, ",\n");
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unsupported TPI symbol leaf: ", __FILE__, __LINE__);
        tpi_leaf_print(symbol->leaf, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }

    fprintf_depth(stream, depth, "}");
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

    uint32_t offset = tpi_header->header_size;

    while (offset < msf_stream->size)
    {
        symbols->count++;
        symbols->symbols = realloc(symbols->symbols, symbols->count * sizeof(*symbols->symbols));
        assert(symbols->symbols);

        struct tpi_symbol *symbol = &symbols->symbols[symbols->count - 1];
        memset(symbol, 0, sizeof(*symbol));

        MSF_STREAM_READ(msf, msf_stream, &offset, symbol->size, file_stream);
        assert(symbol->size >= sizeof(uint16_t));

        uint32_t start_offset = offset;

        tpi_symbol_read(symbol, msf, msf_stream, tpi_header, &offset, file_stream);

        if (offset != start_offset + symbol->size)
        {
            uint32_t size_read = offset - start_offset;

            // TODO: remove this check when stuff is read correctly...
            if (symbol->leaf != LF_POINTER &&
                symbol->leaf != LF_UNION &&
                symbol->leaf != LF_STRUCTURE &&
                symbol->leaf != LF_CLASS &&
                symbol->leaf != LF_ENUM)
            {
                fprintf(stderr, "%s:%i: ERROR: Incorrect TPI symbol data parsing for leaf: ", __FILE__, __LINE__);
                tpi_leaf_print(symbol->leaf, stderr);
                fprintf(stderr, "; read %u, expected %u\n", size_read, symbol->size);
                exit(EXIT_FAILURE);
            }
        }

        offset = start_offset + symbol->size;
    }
}

void tpi_symbols_dispose(struct tpi_symbols *symbols)
{
    assert(symbols);

    for (uint32_t i = 0; i < symbols->count; i++)
        tpi_symbol_dispose(&symbols->symbols[i]);

    free(symbols->symbols);
}

void tpi_symbols_print(struct tpi_symbols *symbols, uint32_t depth, FILE *stream)
{
    assert(symbols);
    assert(stream);

    fprintf(stream, "tpi_symbols {\n");
    fprintf_depth(stream, depth + 1, "count: %u,\n", symbols->count);
    fprintf_depth(stream, depth + 1, "symbols: [\n");
    for (uint32_t i = 0; i < symbols->count; i++)
    {
        fprintf_depth(stream, depth + 2, "[%u] = ", i);
        tpi_symbol_print(&symbols->symbols[i], depth + 2, stream);
        fprintf(stream, ",\n");
    }
    fprintf_depth(stream, depth + 1, "],\n");
    fprintf_depth(stream, depth, "}");
}
