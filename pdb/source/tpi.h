#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"

/* ---------- TPI slice */

#define TPI_SLICE_STRUCT \
STRUCT_DECL(tpi_slice) \
    FIELD_PRIMITIVE(uint32_t, offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, size, "%u") \
STRUCT_END(tpi_slice)

TPI_SLICE_STRUCT
static_assert(sizeof(struct tpi_slice) == 8, "invalid tpi_slice size");

void tpi_slice_print(struct tpi_slice *slice, uint32_t depth, FILE *stream);

/* ---------- TPI header */

#define TPI_HEADER_STRUCT \
STRUCT_DECL(tpi_header) \
    FIELD_PRIMITIVE(uint32_t, version, "%u") \
    FIELD_PRIMITIVE(uint32_t, header_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, minimum_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, maximum_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, gprec_size, "%u") \
    FIELD_PRIMITIVE(uint16_t, tpi_hash_stream, "%u") \
    FIELD_PRIMITIVE(uint16_t, tpi_hash_pad_stream, "%u") \
    FIELD_PRIMITIVE(uint32_t, hash_key_size, "%u") \
    FIELD_PRIMITIVE(uint32_t, hash_bucket_size, "%u") \
    FIELD_STRUCT(struct tpi_slice, hash_values, tpi_slice_print) \
    FIELD_STRUCT(struct tpi_slice, ti_off, tpi_slice_print) \
    FIELD_STRUCT(struct tpi_slice, hash_adj, tpi_slice_print) \
STRUCT_END(tpi_header)

TPI_HEADER_STRUCT
static_assert(sizeof(struct tpi_header) == 56, "invalid tpi_header size");

void tpi_header_read(struct msf *msf, struct tpi_header *out_header, FILE *stream);
void tpi_header_print(struct tpi_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI leaf */

#define TPI_LEAF_ENUM \
ENUM_DECL(tpi_leaf) \
    ENUM_VALUE(LF_MODIFIER_16t, 0x0001) \
    ENUM_VALUE(LF_POINTER_16t, 0x0002) \
    ENUM_VALUE(LF_ARRAY_16t, 0x0003) \
    ENUM_VALUE(LF_CLASS_16t, 0x0004) \
    ENUM_VALUE(LF_STRUCTURE_16t, 0x0005) \
    ENUM_VALUE(LF_UNION_16t, 0x0006) \
    ENUM_VALUE(LF_ENUM_16t, 0x0007) \
    ENUM_VALUE(LF_PROCEDURE_16t, 0x0008) \
    ENUM_VALUE(LF_MFUNCTION_16t, 0x0009) \
    ENUM_VALUE(LF_VTSHAPE, 0x000a) \
    ENUM_VALUE(LF_COBOL0_16t, 0x000b) \
    ENUM_VALUE(LF_COBOL1, 0x000c) \
    ENUM_VALUE(LF_BARRAY_16t, 0x000d) \
    ENUM_VALUE(LF_LABEL, 0x000e) \
    ENUM_VALUE(LF_NULL, 0x000f) \
    ENUM_VALUE(LF_NOTTRAN, 0x0010) \
    ENUM_VALUE(LF_DIMARRAY_16t, 0x0011) \
    ENUM_VALUE(LF_VFTPATH_16t, 0x0012) \
    ENUM_VALUE(LF_PRECOMP_16t, 0x0013) \
    ENUM_VALUE(LF_ENDPRECOMP, 0x0014) \
    ENUM_VALUE(LF_OEM_16t, 0x0015) \
    ENUM_VALUE(LF_TYPESERVER_ST, 0x0016) \
    ENUM_VALUE(LF_SKIP_16t, 0x0200) \
    ENUM_VALUE(LF_ARGLIST_16t, 0x0201) \
    ENUM_VALUE(LF_DEFARG_16t, 0x0202) \
    ENUM_VALUE(LF_LIST, 0x0203) \
    ENUM_VALUE(LF_FIELDLIST_16t, 0x0204) \
    ENUM_VALUE(LF_DERIVED_16t, 0x0205) \
    ENUM_VALUE(LF_BITFIELD_16t, 0x0206) \
    ENUM_VALUE(LF_METHODLIST_16t, 0x0207) \
    ENUM_VALUE(LF_DIMCONU_16t, 0x0208) \
    ENUM_VALUE(LF_DIMCONLU_16t, 0x0209) \
    ENUM_VALUE(LF_DIMVARU_16t, 0x020a) \
    ENUM_VALUE(LF_DIMVARLU_16t, 0x020b) \
    ENUM_VALUE(LF_REFSYM, 0x020c) \
    ENUM_VALUE(LF_BCLASS_16t, 0x0400) \
    ENUM_VALUE(LF_VBCLASS_16t, 0x0401) \
    ENUM_VALUE(LF_IVBCLASS_16t, 0x0402) \
    ENUM_VALUE(LF_ENUMERATE_ST, 0x0403) \
    ENUM_VALUE(LF_FRIENDFCN_16t, 0x0404) \
    ENUM_VALUE(LF_INDEX_16t, 0x0405) \
    ENUM_VALUE(LF_MEMBER_16t, 0x0406) \
    ENUM_VALUE(LF_STMEMBER_16t, 0x0407) \
    ENUM_VALUE(LF_METHOD_16t, 0x0408) \
    ENUM_VALUE(LF_NESTTYPE_16t, 0x0409) \
    ENUM_VALUE(LF_VFUNCTAB_16t, 0x040a) \
    ENUM_VALUE(LF_FRIENDCLS_16t, 0x040b) \
    ENUM_VALUE(LF_ONEMETHOD_16t, 0x040c) \
    ENUM_VALUE(LF_VFUNCOFF_16t, 0x040d) \
    ENUM_VALUE(LF_TI16_MAX, 0x1000) \
    ENUM_VALUE(LF_MODIFIER, 0x1001) \
    ENUM_VALUE(LF_POINTER, 0x1002) \
    ENUM_VALUE(LF_ARRAY_ST, 0x1003) \
    ENUM_VALUE(LF_CLASS_ST, 0x1004) \
    ENUM_VALUE(LF_STRUCTURE_ST, 0x1005) \
    ENUM_VALUE(LF_UNION_ST, 0x1006) \
    ENUM_VALUE(LF_ENUM_ST, 0x1007) \
    ENUM_VALUE(LF_PROCEDURE, 0x1008) \
    ENUM_VALUE(LF_MFUNCTION, 0x1009) \
    ENUM_VALUE(LF_COBOL0, 0x100a) \
    ENUM_VALUE(LF_BARRAY, 0x100b) \
    ENUM_VALUE(LF_DIMARRAY_ST, 0x100c) \
    ENUM_VALUE(LF_VFTPATH, 0x100d) \
    ENUM_VALUE(LF_PRECOMP_ST, 0x100e) \
    ENUM_VALUE(LF_OEM, 0x100f) \
    ENUM_VALUE(LF_ALIAS_ST, 0x1010) \
    ENUM_VALUE(LF_OEM2, 0x1011) \
    ENUM_VALUE(LF_SKIP, 0x1200) \
    ENUM_VALUE(LF_ARGLIST, 0x1201) \
    ENUM_VALUE(LF_DEFARG_ST, 0x1202) \
    ENUM_VALUE(LF_FIELDLIST, 0x1203) \
    ENUM_VALUE(LF_DERIVED, 0x1204) \
    ENUM_VALUE(LF_BITFIELD, 0x1205) \
    ENUM_VALUE(LF_METHODLIST, 0x1206) \
    ENUM_VALUE(LF_DIMCONU, 0x1207) \
    ENUM_VALUE(LF_DIMCONLU, 0x1208) \
    ENUM_VALUE(LF_DIMVARU, 0x1209) \
    ENUM_VALUE(LF_DIMVARLU, 0x120a) \
    ENUM_VALUE(LF_BCLASS, 0x1400) \
    ENUM_VALUE(LF_VBCLASS, 0x1401) \
    ENUM_VALUE(LF_IVBCLASS, 0x1402) \
    ENUM_VALUE(LF_FRIENDFCN_ST, 0x1403) \
    ENUM_VALUE(LF_INDEX, 0x1404) \
    ENUM_VALUE(LF_MEMBER_ST, 0x1405) \
    ENUM_VALUE(LF_STMEMBER_ST, 0x1406) \
    ENUM_VALUE(LF_METHOD_ST, 0x1407) \
    ENUM_VALUE(LF_NESTTYPE_ST, 0x1408) \
    ENUM_VALUE(LF_VFUNCTAB, 0x1409) \
    ENUM_VALUE(LF_FRIENDCLS, 0x140a) \
    ENUM_VALUE(LF_ONEMETHOD_ST, 0x140b) \
    ENUM_VALUE(LF_VFUNCOFF, 0x140c) \
    ENUM_VALUE(LF_NESTTYPEEX_ST, 0x140d) \
    ENUM_VALUE(LF_MEMBERMODIFY_ST, 0x140e) \
    ENUM_VALUE(LF_MANAGED_ST, 0x140f) \
    ENUM_VALUE(LF_ST_MAX, 0x1500) \
    ENUM_VALUE(LF_TYPESERVER, 0x1501) \
    ENUM_VALUE(LF_ENUMERATE, 0x1502) \
    ENUM_VALUE(LF_ARRAY, 0x1503) \
    ENUM_VALUE(LF_CLASS, 0x1504) \
    ENUM_VALUE(LF_STRUCTURE, 0x1505) \
    ENUM_VALUE(LF_UNION, 0x1506) \
    ENUM_VALUE(LF_ENUM, 0x1507) \
    ENUM_VALUE(LF_DIMARRAY, 0x1508) \
    ENUM_VALUE(LF_PRECOMP, 0x1509) \
    ENUM_VALUE(LF_ALIAS, 0x150a) \
    ENUM_VALUE(LF_DEFARG, 0x150b) \
    ENUM_VALUE(LF_FRIENDFCN, 0x150c) \
    ENUM_VALUE(LF_MEMBER, 0x150d) \
    ENUM_VALUE(LF_STMEMBER, 0x150e) \
    ENUM_VALUE(LF_METHOD, 0x150f) \
    ENUM_VALUE(LF_NESTTYPE, 0x1510) \
    ENUM_VALUE(LF_ONEMETHOD, 0x1511) \
    ENUM_VALUE(LF_NESTTYPEEX, 0x1512) \
    ENUM_VALUE(LF_MEMBERMODIFY, 0x1513) \
    ENUM_VALUE(LF_MANAGED, 0x1514) \
    ENUM_VALUE(LF_TYPESERVER2, 0x1515) \
    ENUM_VALUE(LF_STRIDED_ARRAY, 0x1516) \
    ENUM_VALUE(LF_HLSL, 0x1517) \
    ENUM_VALUE(LF_MODIFIER_EX, 0x1518) \
    ENUM_VALUE(LF_INTERFACE, 0x1519) \
    ENUM_VALUE(LF_BINTERFACE, 0x151a) \
    ENUM_VALUE(LF_VECTOR, 0x151b) \
    ENUM_VALUE(LF_MATRIX, 0x151c) \
    ENUM_VALUE(LF_VFTABLE, 0x151d) \
    ENUM_VALUE(LF_FUNC_ID, 0x1601) \
    ENUM_VALUE(LF_MFUNC_ID, 0x1602) \
    ENUM_VALUE(LF_BUILDINFO, 0x1603) \
    ENUM_VALUE(LF_SUBSTR_LIST, 0x1604) \
    ENUM_VALUE(LF_STRING_ID, 0x1605) \
    ENUM_VALUE(LF_UDT_SRC_LINE, 0x1606) \
    ENUM_VALUE(LF_UDT_MOD_SRC_LINE, 0x1607) \
    ENUM_VALUE(LF_STRUCTURE19, 0x1609) \
    ENUM_VALUE(LF_NUMERIC, 0x8000) \
    ENUM_VALUE(LF_CHAR, 0x8000) \
    ENUM_VALUE(LF_SHORT, 0x8001) \
    ENUM_VALUE(LF_USHORT, 0x8002) \
    ENUM_VALUE(LF_LONG, 0x8003) \
    ENUM_VALUE(LF_ULONG, 0x8004) \
    ENUM_VALUE(LF_REAL32, 0x8005) \
    ENUM_VALUE(LF_REAL64, 0x8006) \
    ENUM_VALUE(LF_REAL80, 0x8007) \
    ENUM_VALUE(LF_REAL128, 0x8008) \
    ENUM_VALUE(LF_QUADWORD, 0x8009) \
    ENUM_VALUE(LF_UQUADWORD, 0x800a) \
    ENUM_VALUE(LF_REAL48, 0x800b) \
    ENUM_VALUE(LF_COMPLEX32, 0x800c) \
    ENUM_VALUE(LF_COMPLEX64, 0x800d) \
    ENUM_VALUE(LF_COMPLEX80, 0x800e) \
    ENUM_VALUE(LF_COMPLEX128, 0x800f) \
    ENUM_VALUE(LF_VARSTRING, 0x8010) \
    ENUM_VALUE(LF_OCTWORD, 0x8017) \
    ENUM_VALUE(LF_UOCTWORD, 0x8018) \
    ENUM_VALUE(LF_DECIMAL, 0x8019) \
    ENUM_VALUE(LF_DATE, 0x801a) \
    ENUM_VALUE(LF_UTF8STRING, 0x801b) \
    ENUM_VALUE(LF_REAL16, 0x801c) \
    ENUM_VALUE(LF_PAD0, 0xf0) \
    ENUM_VALUE(LF_PAD1, 0xf1) \
    ENUM_VALUE(LF_PAD2, 0xf2) \
    ENUM_VALUE(LF_PAD3, 0xf3) \
    ENUM_VALUE(LF_PAD4, 0xf4) \
    ENUM_VALUE(LF_PAD5, 0xf5) \
    ENUM_VALUE(LF_PAD6, 0xf6) \
    ENUM_VALUE(LF_PAD7, 0xf7) \
    ENUM_VALUE(LF_PAD8, 0xf8) \
    ENUM_VALUE(LF_PAD9, 0xf9) \
    ENUM_VALUE(LF_PAD10, 0xfa) \
    ENUM_VALUE(LF_PAD11, 0xfb) \
    ENUM_VALUE(LF_PAD12, 0xfc) \
    ENUM_VALUE(LF_PAD13, 0xfd) \
    ENUM_VALUE(LF_PAD14, 0xfe) \
    ENUM_VALUE(LF_PAD15, 0xff) \
ENUM_END(tpi_leaf)

TPI_LEAF_ENUM

void tpi_leaf_print(enum tpi_leaf leaf, FILE *stream);

/* ---------- TPI primitive types */

#define TPI_PRIMITIVE_TYPE_ENUM \
ENUM_DECL(tpi_primitive_type) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_NONE) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_VOID) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_CHAR) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UCHAR) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_RCHAR) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_WCHAR) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_RCHAR16) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_RCHAR32) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_INT8) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UINT8) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_SHORT) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_USHORT) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_INT16) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UINT16) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_LONG) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_ULONG) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_INT32) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UINT32) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_QUAD) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UQUAD) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_INT64) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UINT64) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_OCTA) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UOCTA) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_INT128) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_UINT128) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT16) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT32) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT32_PP) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT48) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT64) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT80) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_FLOAT128) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_COMPLEX32) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_COMPLEX64) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_COMPLEX80) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_COMPLEX128) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_BOOL8) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_BOOL16) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_BOOL32) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_BOOL64) \
    ENUM_VALUE(TPI_PRIMITIVE_TYPE_HRESULT) \
ENUM_END(tpi_primitive_type)

TPI_PRIMITIVE_TYPE_ENUM

void tpi_primitive_type_print(enum tpi_primitive_type type, FILE *stream);

/* ---------- TPI primitive indirection types */

#define TPI_PRIMITIVE_INDIRECTION_ENUM \
ENUM_DECL(tpi_primitive_indirection) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_NONE) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_NEAR16) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_FAR16) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_HUGE16) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_NEAR32) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_FAR32) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_NEAR64) \
    ENUM_VALUE(TPI_PRIMITIVE_INDIRECTION_NEAR128) \
ENUM_END(tpi_primitive_indirection)

TPI_PRIMITIVE_INDIRECTION_ENUM

void tpi_primitive_indirection_print(enum tpi_primitive_indirection indirection, FILE *stream);

/* ---------- TPI primitives */

#define TPI_PRIMITIVE_STRUCT \
STRUCT_DECL(tpi_primitive) \
    FIELD_PRIMITIVE_FMT(enum tpi_primitive_type, type, tpi_primitive_type_print) \
    FIELD_PRIMITIVE_FMT(enum tpi_primitive_indirection, indirection, tpi_primitive_indirection_print) \
STRUCT_END(tpi_primitive)

TPI_PRIMITIVE_STRUCT

void tpi_primitive_print(struct tpi_primitive *item, uint32_t depth, FILE *stream);

/* ---------- TPI HFA types */

#define TPI_HFA_TYPE_ENUM \
ENUM_DECL(tpi_hfa_type) \
    ENUM_VALUE(CV_HFA_none, 0) \
    ENUM_VALUE(CV_HFA_float, 1) \
    ENUM_VALUE(CV_HFA_double, 2) \
    ENUM_VALUE(CV_HFA_other, 3) \
ENUM_END(tpi_hfa_type)

TPI_HFA_TYPE_ENUM

void tpi_hfa_type_print(enum tpi_hfa_type type, FILE *stream);

/* ---------- TPI MoCOM UDT types */

#define TPI_MOCOM_UDT_TYPE_ENUM \
ENUM_DECL(tpi_mocom_udt_type) \
    ENUM_VALUE(CV_MOCOM_UDT_none, 0) \
    ENUM_VALUE(CV_MOCOM_UDT_ref, 1) \
    ENUM_VALUE(CV_MOCOM_UDT_value, 2) \
    ENUM_VALUE(CV_MOCOM_UDT_interface, 3) \
ENUM_END(tpi_mocom_udt_type)

TPI_MOCOM_UDT_TYPE_ENUM

void tpi_mocom_udt_type_print(enum tpi_mocom_udt_type type, FILE *stream);

/* ---------- TPI struct/class/interface properties */

#define TPI_PROPERTIES_STRUCT \
STRUCT_DECL(tpi_properties) \
    FIELD_PRIMITIVE_BITS(uint16_t, packed, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, ctor, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, ovlops, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, isnested, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, cnested, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, opassign, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, opcast, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, fwdref, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, scoped, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, hasuniquename, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, sealed, 1, "%u") \
    FIELD_PRIMITIVE_BITS_FMT(uint16_t, hfa, 2, tpi_hfa_type_print) \
    FIELD_PRIMITIVE_BITS(uint16_t, intrinsic, 1, "%u") \
    FIELD_PRIMITIVE_BITS_FMT(uint16_t, mocom, 2, tpi_mocom_udt_type_print) \
STRUCT_END(tpi_properties)

TPI_PROPERTIES_STRUCT
static_assert(sizeof(struct tpi_properties) == sizeof(uint16_t), "invalid tpi_properties size");

void tpi_properties_print(struct tpi_properties *properties, uint32_t depth, FILE *stream);

/* ---------- TPI field attributes */

#define TPI_FIELD_ATTRIBUTES_STRUCT \
STRUCT_DECL(tpi_field_attributes) \
    FIELD_PRIMITIVE_BITS(uint16_t, access, 2, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, mprop, 3, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, pseudo, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, noinherit, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, noconstruct, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, compgenx, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, sealed, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint16_t, unused, 6, "%u") \
STRUCT_END(tpi_field_attributes)

TPI_FIELD_ATTRIBUTES_STRUCT
static_assert(sizeof(struct tpi_field_attributes) == sizeof(uint16_t), "invalid tpi_field_attributes size");

void tpi_field_attributes_print(struct tpi_field_attributes *attributes, uint32_t depth, FILE *stream);

/* ---------- TPI field list */

#define TPI_FIELD_LIST_STRUCT \
STRUCT_DECL(tpi_field_list) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct tpi_symbol *, fields, count, tpi_symbol_print) \
    FIELD_PRIMITIVE(uint32_t, continuation_type_index, "%u") \
STRUCT_END(tpi_field_list)

TPI_FIELD_LIST_STRUCT

void tpi_field_list_dispose(struct tpi_field_list *item);
void tpi_field_list_print(struct tpi_field_list *item, uint32_t depth, FILE *stream);

/* ---------- TPI member header */

#define TPI_MEMBER_HEADER_STRUCT \
STRUCT_DECL(tpi_member_header) \
    FIELD_PRIMITIVE(uint16_t, attributes, "%u") \
    FIELD_PRIMITIVE(uint32_t, field_type, "%u") \
STRUCT_END(tpi_member_header)

#pragma pack(push, 1)
TPI_MEMBER_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_member_header) == 6, "invalid tpi_member_header size");

void tpi_member_header_print(struct tpi_member_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI member */

#define TPI_MEMBER_STRUCT \
STRUCT_DECL(tpi_member) \
    FIELD_STRUCT(struct tpi_member_header, header, tpi_member_header_print) \
    FIELD_PRIMITIVE(uint64_t, offset, "%llu") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_member)

TPI_MEMBER_STRUCT

void tpi_member_read(struct tpi_member *item, uint32_t leaf, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_member_dispose(struct tpi_member *item);
void tpi_member_print(struct tpi_member *item, uint32_t depth, FILE *stream);

/* ---------- TPI static member header */

#define TPI_STATIC_MEMBER_HEADER_STRUCT \
STRUCT_DECL(tpi_static_member_header) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_PRIMITIVE(uint32_t, field_type_index, "%u") \
STRUCT_END(tpi_static_member_header)

#pragma pack(push, 1)
TPI_STATIC_MEMBER_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_static_member_header) == 6, "invalid tpi_static_member_header size");

void tpi_static_member_header_print(struct tpi_static_member_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI static member */

#define TPI_STATIC_MEMBER_STRUCT \
STRUCT_DECL(tpi_static_member) \
    FIELD_STRUCT(struct tpi_static_member_header, header, tpi_static_member_header_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_static_member)

TPI_STATIC_MEMBER_STRUCT

void tpi_static_member_dispose(struct tpi_static_member *item);
void tpi_static_member_print(struct tpi_static_member *item, uint32_t depth, FILE *stream);

/* ---------- TPI pointer type */

#define TPI_POINTER_TYPE_ENUM \
ENUM_DECL(tpi_pointer_type) \
    ENUM_VALUE(CV_PTR_NEAR, 0x00) \
    ENUM_VALUE(CV_PTR_FAR, 0x01) \
    ENUM_VALUE(CV_PTR_HUGE, 0x02) \
    ENUM_VALUE(CV_PTR_BASE_SEG, 0x03) \
    ENUM_VALUE(CV_PTR_BASE_VAL, 0x04) \
    ENUM_VALUE(CV_PTR_BASE_SEGVAL, 0x05) \
    ENUM_VALUE(CV_PTR_BASE_ADDR, 0x06) \
    ENUM_VALUE(CV_PTR_BASE_SEGADDR, 0x07) \
    ENUM_VALUE(CV_PTR_BASE_TYPE, 0x08) \
    ENUM_VALUE(CV_PTR_BASE_SELF, 0x09) \
    ENUM_VALUE(CV_PTR_NEAR32, 0x0a) \
    ENUM_VALUE(CV_PTR_FAR32, 0x0b) \
    ENUM_VALUE(CV_PTR_64, 0x0c) \
    ENUM_VALUE(CV_PTR_UNUSEDPTR, 0x0d) \
ENUM_END(tpi_pointer_type)

TPI_POINTER_TYPE_ENUM

void tpi_pointer_type_print(enum tpi_pointer_type type, FILE *stream);

/* ---------- TPI pointer mode */

#define TPI_POINTER_MODE_ENUM \
ENUM_DECL(tpi_pointer_mode) \
    ENUM_VALUE(CV_PTR_MODE_PTR, 0x00) \
    ENUM_VALUE(CV_PTR_MODE_REF, 0x01) \
    ENUM_VALUE(CV_PTR_MODE_LVREF, 0x01) \
    ENUM_VALUE(CV_PTR_MODE_PMEM, 0x02) \
    ENUM_VALUE(CV_PTR_MODE_PMFUNC, 0x03) \
    ENUM_VALUE(CV_PTR_MODE_RVREF, 0x04) \
    ENUM_VALUE(CV_PTR_MODE_RESERVED, 0x05) \
ENUM_END(tpi_pointer_mode)

TPI_POINTER_MODE_ENUM

void tpi_pointer_mode_print(enum tpi_pointer_mode mode, FILE *stream);

/* ---------- TPI pointer attributes */

#define TPI_POINTER_ATTRIBUTES_STRUCT \
STRUCT_DECL(tpi_pointer_attributes) \
    FIELD_PRIMITIVE_BITS_FMT(uint32_t, ptrtype, 5, tpi_pointer_type_print) \
    FIELD_PRIMITIVE_BITS_FMT(uint32_t, ptrmode, 3, tpi_pointer_mode_print) \
    FIELD_PRIMITIVE_BITS(uint32_t, isflat32, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, isvolatile, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, isconst, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, isunaligned, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, isrestrict, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, size, 6, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, ismocom, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, islref, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, isrref, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint32_t, unused, 10, "%u") \
STRUCT_END(tpi_pointer_attributes)

TPI_POINTER_ATTRIBUTES_STRUCT
static_assert(sizeof(struct tpi_pointer_attributes) == sizeof(uint32_t), "invalid tpi_pointer_attributes size");

void tpi_pointer_attributes_print(struct tpi_pointer_attributes *attributes, uint32_t depth, FILE *stream);

/* ---------- TPI pointer header */

#define TPI_POINTER_HEADER_STRUCT \
STRUCT_DECL(tpi_pointer_header) \
    FIELD_PRIMITIVE(uint32_t, underlying_type_index, "%u") \
    FIELD_STRUCT(struct tpi_pointer_attributes, attributes, tpi_pointer_attributes_print) \
STRUCT_END(tpi_pointer_header)

#pragma pack(push, 1)
TPI_POINTER_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_pointer_header) == 8, "invalid tpi_pointer_header size");

void tpi_pointer_header_print(struct tpi_pointer_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI pointer */

#define TPI_POINTER_STRUCT \
STRUCT_DECL(tpi_pointer) \
    FIELD_STRUCT(struct tpi_pointer_header, header, tpi_pointer_header_print) \
    FIELD_PRIMITIVE(uint32_t, containing_class_type_index, "%u") \
STRUCT_END(tpi_pointer)

TPI_POINTER_STRUCT

void tpi_pointer_print(struct tpi_pointer *item, uint32_t depth, FILE *stream);

/* ---------- TPI argument list */

#define TPI_ARGUMENT_LIST_STRUCT \
STRUCT_DECL(tpi_argument_list) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, type_indices, count, "%u") \
STRUCT_END(tpi_argument_list)

TPI_ARGUMENT_LIST_STRUCT

void tpi_argument_list_dispose(struct tpi_argument_list *item);
void tpi_argument_list_print(struct tpi_argument_list *item, uint32_t depth, FILE *stream);

/* ---------- TPI procedure attributes */

#define TPI_PROCEDURE_ATTRIBUTES_STRUCT \
STRUCT_DECL(tpi_procedure_attributes) \
    FIELD_PRIMITIVE_BITS(uint8_t, cxxreturnudt, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, ctor, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, ctorvbase, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, unused, 5, "%u") \
STRUCT_END(tpi_procedure_attributes)

TPI_PROCEDURE_ATTRIBUTES_STRUCT
static_assert(sizeof(struct tpi_procedure_attributes) == sizeof(uint8_t), "invalid tpi_procedure_attributes size");

void tpi_procedure_attributes_print(struct tpi_procedure_attributes *attributes, uint32_t depth, FILE *stream);

/* ---------- CV call type */

#define CV_CALL_TYPE_ENUM \
ENUM_DECL(cv_call_type) \
    ENUM_VALUE(CV_CALL_NEAR_C, 0x00) /* near right to left push, caller pops stack */ \
    ENUM_VALUE(CV_CALL_FAR_C, 0x01) /* far right to left push, caller pops stack */ \
    ENUM_VALUE(CV_CALL_NEAR_PASCAL, 0x02) /* near left to right push, callee pops stack */ \
    ENUM_VALUE(CV_CALL_FAR_PASCAL, 0x03) /* far left to right push, callee pops stack */ \
    ENUM_VALUE(CV_CALL_NEAR_FAST, 0x04) /* near left to right push with regs, callee pops stack */ \
    ENUM_VALUE(CV_CALL_FAR_FAST, 0x05) /* far left to right push with regs, callee pops stack */ \
    ENUM_VALUE(CV_CALL_SKIPPED, 0x06) /* skipped (unused) call index */ \
    ENUM_VALUE(CV_CALL_NEAR_STD, 0x07) /* near standard call */ \
    ENUM_VALUE(CV_CALL_FAR_STD, 0x08) /* far standard call */ \
    ENUM_VALUE(CV_CALL_NEAR_SYS, 0x09) /* near sys call */ \
    ENUM_VALUE(CV_CALL_FAR_SYS, 0x0a) /* far sys call */ \
    ENUM_VALUE(CV_CALL_THISCALL, 0x0b) /* this call (this passed in register) */ \
    ENUM_VALUE(CV_CALL_MIPSCALL, 0x0c) /* Mips call */ \
    ENUM_VALUE(CV_CALL_GENERIC, 0x0d) /* Generic call sequence */ \
    ENUM_VALUE(CV_CALL_ALPHACALL, 0x0e) /* Alpha call */ \
    ENUM_VALUE(CV_CALL_PPCCALL, 0x0f) /* PPC call */ \
    ENUM_VALUE(CV_CALL_SHCALL, 0x10) /* Hitachi SuperH call */ \
    ENUM_VALUE(CV_CALL_ARMCALL, 0x11) /* ARM call */ \
    ENUM_VALUE(CV_CALL_AM33CALL, 0x12) /* AM33 call */ \
    ENUM_VALUE(CV_CALL_TRICALL, 0x13) /* TriCore Call */ \
    ENUM_VALUE(CV_CALL_SH5CALL, 0x14) /* Hitachi SuperH-5 call */ \
    ENUM_VALUE(CV_CALL_M32RCALL, 0x15) /* M32R Call */ \
    ENUM_VALUE(CV_CALL_CLRCALL, 0x16) /* clr call */ \
    ENUM_VALUE(CV_CALL_INLINE, 0x17) /* Marker for routines always inlined and thus lacking a convention */ \
    ENUM_VALUE(CV_CALL_NEAR_VECTOR, 0x18) /* near left to right push with regs, callee pops stack */ \
    ENUM_VALUE(CV_CALL_RESERVED, 0x19) /* first unused call enumeration */ \
ENUM_END(cv_call_type)

CV_CALL_TYPE_ENUM

void cv_call_type_print(enum cv_call_type item, FILE *stream);

/* ---------- TPI procedure */

#define TPI_PROCEDURE_STRUCT \
STRUCT_DECL(tpi_procedure) \
    FIELD_PRIMITIVE(uint32_t, return_type_index, "%u") \
    FIELD_PRIMITIVE_FMT(uint8_t, calling_convention, cv_call_type_print) \
    FIELD_STRUCT(struct tpi_procedure_attributes, attributes, tpi_procedure_attributes_print) \
    FIELD_PRIMITIVE(uint16_t, parameter_count, "%u") \
    FIELD_PRIMITIVE(uint32_t, argument_list_type_index, "%u") \
STRUCT_END(tpi_procedure)

#pragma pack(push, 1)
TPI_PROCEDURE_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_procedure) == 12, "invalid tpi_procedure size");

void tpi_procedure_print(struct tpi_procedure *item, uint32_t depth, FILE *stream);

/* ---------- TPI modifier */

enum tpi_modifier_flags
{
    TPI_MODIFIER_IS_CONSTANT = 1 << 0,
    TPI_MODIFIER_IS_VOLATILE = 1 << 1,
    TPI_MODIFIER_IS_UNALIGNED = 1 << 2,
};

#define TPI_MODIFIER_STRUCT \
STRUCT_DECL(tpi_modifier) \
    FIELD_PRIMITIVE(uint32_t, underlying_type_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, flags, "%u") \
STRUCT_END(tpi_modifier)

#pragma pack(push, 1)
TPI_MODIFIER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_modifier) == 6, "invalid tpi_modifier size");

void tpi_modifier_print(struct tpi_modifier *item, uint32_t depth, FILE *stream);

/* ---------- TPI enumerate variant type */

#define TPI_ENUMERATE_VARIANT_TYPE_ENUM \
ENUM_DECL(tpi_enumerate_variant_type) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_UINT8) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_UINT16) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_UINT32) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_UINT64) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_INT8) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_INT16) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_INT32) \
    ENUM_VALUE(TPI_ENUMERATE_VARIANT_INT64) \
ENUM_END(tpi_enumerate_variant_type)

TPI_ENUMERATE_VARIANT_TYPE_ENUM

void tpi_enumerate_variant_type_print(enum tpi_enumerate_variant_type type, FILE *stream);

/* ---------- TPI enumerate variant */

#define TPI_ENUMERATE_VARIANT_STRUCT \
STRUCT_DECL(tpi_enumerate_variant) \
    FIELD_PRIMITIVE_FMT(enum tpi_enumerate_variant_type, type, tpi_enumerate_variant_type_print) \
    FIELD_UNION_DECL() \
        FIELD_UNION_FIELD_PRIMITIVE(uint8_t, uint8, type, TPI_ENUMERATE_VARIANT_UINT8, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint16_t, uint16, type, TPI_ENUMERATE_VARIANT_UINT16, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, uint32, type, TPI_ENUMERATE_VARIANT_UINT32, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint64_t, uint64, type, TPI_ENUMERATE_VARIANT_UINT64, "%llu") \
        FIELD_UNION_FIELD_PRIMITIVE(int8_t, int8, type, TPI_ENUMERATE_VARIANT_INT8, "%i") \
        FIELD_UNION_FIELD_PRIMITIVE(int16_t, int16, type, TPI_ENUMERATE_VARIANT_INT16, "%i") \
        FIELD_UNION_FIELD_PRIMITIVE(int32_t, int32, type, TPI_ENUMERATE_VARIANT_INT32, "%i") \
        FIELD_UNION_FIELD_PRIMITIVE(int64_t, int64, type, TPI_ENUMERATE_VARIANT_INT64, "%lli") \
    FIELD_UNION_END() \
STRUCT_END(tpi_enumerate_variant)

TPI_ENUMERATE_VARIANT_STRUCT

void tpi_enumerate_variant_read(struct tpi_enumerate_variant *variant, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_enumerate_variant_print(struct tpi_enumerate_variant *variant, uint32_t depth, FILE *stream);
char *tpi_enumerate_variant_to_string(struct tpi_enumerate_variant *variant);

/* ---------- TPI enumerate */

#define TPI_ENUMERATE_STRUCT \
STRUCT_DECL(tpi_enumerate) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_STRUCT(struct tpi_enumerate_variant, variant, tpi_enumerate_variant_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_enumerate)

TPI_ENUMERATE_STRUCT

void tpi_enumerate_dispose(struct tpi_enumerate *item);
void tpi_enumerate_print(struct tpi_enumerate *item, uint32_t depth, FILE *stream);

/* ---------- TPI enumeration header */

#define TPI_ENUMERATION_HEADER_STRUCT \
STRUCT_DECL(tpi_enumeration_header) \
    FIELD_PRIMITIVE(uint16_t, count, "%u") \
    FIELD_STRUCT(struct tpi_properties, properties, tpi_properties_print) \
    FIELD_PRIMITIVE(uint32_t, underlying_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, fields_type_index, "%u") \
STRUCT_END(tpi_enumeration_header)

TPI_ENUMERATION_HEADER_STRUCT
static_assert(sizeof(struct tpi_enumeration_header) == 12, "invalid tpi_enumeration_header size");

void tpi_enumeration_header_print(struct tpi_enumeration_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI enumeration */

#define TPI_ENUMERATION_STRUCT \
STRUCT_DECL(tpi_enumeration) \
    FIELD_STRUCT(struct tpi_enumeration_header , header, tpi_enumeration_header_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
    FIELD_PRIMITIVE(char *, unique_name, "\"%s\"") \
STRUCT_END(tpi_enumeration)

TPI_ENUMERATION_STRUCT

void tpi_enumeration_dispose(struct tpi_enumeration *item);
void tpi_enumeration_print(struct tpi_enumeration *item, uint32_t depth, FILE *stream);

/* ---------- TPI array header */

#define TPI_ARRAY_HEADER_STRUCT \
STRUCT_DECL(tpi_array_header) \
    FIELD_PRIMITIVE(uint32_t, element_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, indexing_type_index, "%u") \
STRUCT_END(tpi_array_header)

TPI_ARRAY_HEADER_STRUCT
static_assert(sizeof(struct tpi_array_header) == 8, "invalid tpi_array_header size");

void tpi_array_header_print(struct tpi_array_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI array */

#define TPI_ARRAY_STRUCT \
STRUCT_DECL(tpi_array) \
    FIELD_STRUCT(struct tpi_array_header, header, tpi_array_header_print) \
    FIELD_PRIMITIVE(uint32_t, stride, "%u") \
    FIELD_PRIMITIVE(uint32_t, dimension_count, "%u") \
    FIELD_PRIMITIVE_DYNAMIC_ARRAY(uint32_t *, dimensions, dimension_count, "%u") \
STRUCT_END(tpi_array)

TPI_ARRAY_STRUCT

void tpi_array_dispose(struct tpi_array *item);
void tpi_array_print(struct tpi_array *item, uint32_t depth, FILE *stream);

/* ---------- TPI union header */

#define TPI_UNION_HEADER_STRUCT \
STRUCT_DECL(tpi_union_header) \
    FIELD_PRIMITIVE(uint16_t, count, "%u") \
    FIELD_STRUCT(struct tpi_properties, properties, tpi_properties_print) \
    FIELD_PRIMITIVE(uint32_t, fields_type_index, "%u") \
STRUCT_END(tpi_union_header)

TPI_UNION_HEADER_STRUCT
static_assert(sizeof(struct tpi_union_header) == 8, "invalid tpi_union_header size");

void tpi_union_header_print(struct tpi_union_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI union */

#define TPI_UNION_STRUCT \
STRUCT_DECL(tpi_union) \
    FIELD_STRUCT(struct tpi_union_header, header, tpi_union_header_print) \
    FIELD_PRIMITIVE(uint64_t, size, "%llu") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
    FIELD_PRIMITIVE(char *, unique_name, "\"%s\"") \
STRUCT_END(tpi_union)

TPI_UNION_STRUCT

void tpi_union_dispose(struct tpi_union *item);
void tpi_union_print(struct tpi_union *item, uint32_t depth, FILE *stream);

/* ---------- TPI bitfield */

#define TPI_BITFIELD_STRUCT \
STRUCT_DECL(tpi_bitfield) \
    FIELD_PRIMITIVE(uint32_t, underlying_type_index, "%u") \
    FIELD_PRIMITIVE(uint8_t, length, "%u") \
    FIELD_PRIMITIVE(uint8_t, position, "%u") \
    FIELD_PRIMITIVE(uint16_t, padding, "%u") \
STRUCT_END(tpi_bitfield)

TPI_BITFIELD_STRUCT
static_assert(sizeof(struct tpi_bitfield) == 8, "invalid tpi_bitfield size");

void tpi_bitfield_print(struct tpi_bitfield *item, uint32_t depth, FILE *stream);

/* ---------- TPI member function */

#define TPI_MEMBER_FUNCTION_STRUCT \
STRUCT_DECL(tpi_member_function) \
    FIELD_PRIMITIVE(uint32_t, return_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, class_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, this_pointer_type_index, "%u") \
    FIELD_PRIMITIVE_FMT(uint8_t, calling_convention, cv_call_type_print) \
    FIELD_STRUCT(struct tpi_procedure_attributes, attributes, tpi_procedure_attributes_print) \
    FIELD_PRIMITIVE(uint16_t, parameter_count, "%u") \
    FIELD_PRIMITIVE(uint32_t, argument_list_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, this_adjustment, "%u") \
STRUCT_END(tpi_member_function)

TPI_MEMBER_FUNCTION_STRUCT
static_assert(sizeof(struct tpi_member_function) == 24, "invalid tpi_member_function size");

void tpi_member_function_read(struct tpi_member_function *item, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_member_function_print(struct tpi_member_function *item, uint32_t depth, FILE *stream);

/* ---------- TPI overloaded method header */

#define TPI_OVERLOADED_METHOD_HEADER_STRUCT \
STRUCT_DECL(tpi_overloaded_method_header) \
    FIELD_PRIMITIVE(uint16_t, count, "%u") \
    FIELD_PRIMITIVE(uint32_t, method_list_type_index, "%u") \
STRUCT_END(tpi_overloaded_method_header)

#pragma pack(push, 1)
TPI_OVERLOADED_METHOD_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_overloaded_method_header) == 6, "invalid tpi_overloaded_method_header size");

void tpi_overloaded_method_header_print(struct tpi_overloaded_method_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI overloaded method */

#define TPI_OVERLOADED_METHOD_STRUCT \
STRUCT_DECL(tpi_overloaded_method) \
    FIELD_STRUCT(struct tpi_overloaded_method_header, header, tpi_overloaded_method_header_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_overloaded_method)

TPI_OVERLOADED_METHOD_STRUCT

void tpi_overloaded_method_dispose(struct tpi_overloaded_method *item);
void tpi_overloaded_method_print(struct tpi_overloaded_method *item, uint32_t depth, FILE *stream);

/* ---------- TPI method attributes */

#define TPI_METHOD_ATTRIBUTES_ENUM \
ENUM_DECL(tpi_method_attributes) \
    ENUM_VALUE(TPI_METHOD_VANILLA, 0x00) \
    ENUM_VALUE(TPI_METHOD_VIRTUAL, 0x01) \
    ENUM_VALUE(TPI_METHOD_STATIC, 0x02) \
    ENUM_VALUE(TPI_METHOD_FRIEND, 0x03) \
    ENUM_VALUE(TPI_METHOD_INTRO, 0x04) \
    ENUM_VALUE(TPI_METHOD_PUREVIRT, 0x05) \
    ENUM_VALUE(TPI_METHOD_PUREINTRO, 0x06) \
ENUM_END(tpi_method_attributes)

TPI_METHOD_ATTRIBUTES_ENUM

/* ---------- TPI method header */

#define TPI_METHOD_HEADER_STRUCT \
STRUCT_DECL(tpi_method_header) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_PRIMITIVE(uint32_t, method_type_index, "%u") \
STRUCT_END(tpi_method_header)

#pragma pack(push, 1)
TPI_METHOD_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_method_header) == 6, "invalid tpi_method_header size");

void tpi_method_header_print(struct tpi_method_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI method */

#define TPI_METHOD_STRUCT \
STRUCT_DECL(tpi_method) \
    FIELD_STRUCT(struct tpi_method_header, header, tpi_method_header_print) \
    FIELD_PRIMITIVE(uint32_t, vtable_offset, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_method)

TPI_METHOD_STRUCT

void tpi_method_dispose(struct tpi_method *item);
void tpi_method_print(struct tpi_method *item, uint32_t depth, FILE *stream);

/* ---------- TPI method list entry header */

#define TPI_METHOD_LIST_ENTRY_HEADER_STRUCT \
STRUCT_DECL(tpi_method_list_entry_header) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_PRIMITIVE(uint16_t, padding, "%u") \
    FIELD_PRIMITIVE(uint32_t, method_type_index, "%u") \
STRUCT_END(tpi_method_list_entry_header)

TPI_METHOD_LIST_ENTRY_HEADER_STRUCT
static_assert(sizeof(struct tpi_method_list_entry_header) == 8, "invalid tpi_method_list_entry_header size");

void tpi_method_list_entry_header_print(struct tpi_method_list_entry_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI method list entry */

#define TPI_METHOD_LIST_ENTRY_STRUCT \
STRUCT_DECL(tpi_method_list_entry) \
    FIELD_STRUCT(struct tpi_method_list_entry_header, header, tpi_method_list_entry_header_print) \
    FIELD_PRIMITIVE(uint32_t, vtable_offset, "%u") \
STRUCT_END(tpi_method_list_entry)

TPI_METHOD_LIST_ENTRY_STRUCT

void tpi_method_list_entry_print(struct tpi_method_list_entry *entry, uint32_t depth, FILE *stream);

/* ---------- TPI method list */

#define TPI_METHOD_LIST_STRUCT \
STRUCT_DECL(tpi_method_list) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct tpi_method_list_entry *, entries, count, tpi_method_list_entry_print) \
STRUCT_END(tpi_method_list)

TPI_METHOD_LIST_STRUCT

void tpi_method_list_dispose(struct tpi_method_list *item);
void tpi_method_list_print(struct tpi_method_list *item, uint32_t depth, FILE *stream);

/* ---------- TPI vtable */

#define TPI_VTABLE_STRUCT \
STRUCT_DECL(tpi_vtable) \
    FIELD_PRIMITIVE(uint16_t, padding, "%u") \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
STRUCT_END(tpi_vtable)

#pragma pack(push, 1)
TPI_VTABLE_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_vtable) == 6, "invalid tpi_vtable size");

void tpi_vtable_print(struct tpi_vtable *item, uint32_t depth, FILE *stream);

/* ---------- TPI nested type header */

#define TPI_NESTED_TYPE_HEADER_STRUCT \
STRUCT_DECL(tpi_nested_type_header) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_PRIMITIVE(uint32_t, nested_type_index, "%u") \
STRUCT_END(tpi_nested_type_header)

#pragma pack(push, 1)
TPI_NESTED_TYPE_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_nested_type_header) == 6, "invalid tpi_nested_type_header size");

void tpi_nested_type_header_print(struct tpi_nested_type_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI nested type */

#define TPI_NESTED_TYPE_STRUCT \
STRUCT_DECL(tpi_nested_type) \
    FIELD_STRUCT(struct tpi_nested_type_header, header, tpi_nested_type_header_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(tpi_nested_type)

TPI_NESTED_TYPE_STRUCT

void tpi_nested_type_read(struct tpi_nested_type *item, uint32_t leaf, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_nested_type_dispose(struct tpi_nested_type *item);
void tpi_nested_type_print(struct tpi_nested_type *item, uint32_t depth, FILE *stream);

/* ---------- TPI base class header */

#define TPI_BASE_CLASS_HEADER_STRUCT \
STRUCT_DECL(tpi_base_class_header) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_PRIMITIVE(uint32_t, base_class_type_index, "%u") \
STRUCT_END(tpi_base_class_header)

#pragma pack(push, 1)
TPI_BASE_CLASS_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_base_class_header) == 6, "invalid tpi_base_class_header size");

void tpi_base_class_header_print(struct tpi_base_class_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI base class */

#define TPI_BASE_CLASS_STRUCT \
STRUCT_DECL(tpi_base_class) \
    FIELD_STRUCT(struct tpi_base_class_header, header, tpi_base_class_header_print) \
    FIELD_PRIMITIVE(uint32_t, offset, "%u") \
STRUCT_END(tpi_base_class)

TPI_BASE_CLASS_STRUCT

void tpi_base_class_print(struct tpi_base_class *item, uint32_t depth, FILE *stream);

/* ---------- TPI virtual base class header */

#define TPI_VIRTUAL_BASE_CLASS_HEADER_STRUCT \
STRUCT_DECL(tpi_virtual_base_class_header) \
    FIELD_STRUCT(struct tpi_field_attributes, attributes, tpi_field_attributes_print) \
    FIELD_PRIMITIVE(uint32_t, base_class_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, base_pointer_type_index, "%u") \
STRUCT_END(tpi_virtual_base_class_header)

#pragma pack(push, 1)
TPI_VIRTUAL_BASE_CLASS_HEADER_STRUCT
#pragma pack(pop)
static_assert(sizeof(struct tpi_virtual_base_class_header) == 10, "invalid tpi_virtual_base_class_header size");

void tpi_virtual_base_class_header_print(struct tpi_virtual_base_class_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI virtual base class */

#define TPI_VIRTUAL_BASE_CLASS_STRUCT \
STRUCT_DECL(tpi_virtual_base_class) \
    FIELD_STRUCT(struct tpi_virtual_base_class_header, header, tpi_virtual_base_class_header_print) \
    FIELD_PRIMITIVE(uint32_t, base_pointer_offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, virtual_base_offset, "%u") \
STRUCT_END(tpi_virtual_base_class)

TPI_VIRTUAL_BASE_CLASS_STRUCT

void tpi_virtual_base_class_print(struct tpi_virtual_base_class *item, uint32_t depth, FILE *stream);

/* ---------- TPI class header */

#define TPI_CLASS_HEADER_STRUCT \
STRUCT_DECL(tpi_class_header) \
    FIELD_PRIMITIVE(uint16_t, count, "%u") \
    FIELD_STRUCT(struct tpi_properties, properties, tpi_properties_print) \
    FIELD_PRIMITIVE(uint32_t, fields_type_index, "%u") \
STRUCT_END(tpi_class_header)

TPI_CLASS_HEADER_STRUCT
static_assert(sizeof(struct tpi_class_header) == 8, "invalid tpi_class_header size");

void tpi_class_header_print(struct tpi_class_header *header, uint32_t depth, FILE *stream);

/* ---------- TPI class */

#define TPI_CLASS_STRUCT \
STRUCT_DECL(tpi_class) \
    FIELD_STRUCT(struct tpi_class_header, header, tpi_class_header_print) \
    FIELD_PRIMITIVE(uint32_t, derived_from_type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, vtable_shape_type_index, "%u") \
    FIELD_PRIMITIVE(uint64_t, size, "%llu") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
    FIELD_PRIMITIVE(char *, unique_name, "\"%s\"") \
STRUCT_END(tpi_class)

TPI_CLASS_STRUCT

void tpi_class_read(struct tpi_class *item, uint32_t leaf, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_class_dispose(struct tpi_class *item);
void tpi_class_print(struct tpi_class *item, uint32_t depth, FILE *stream);

/* ---------- TPI symbol */

#define TPI_SYMBOL_STRUCT                                                                       \
STRUCT_DECL(tpi_symbol)                                                                         \
    FIELD_PRIMITIVE_FMT(uint16_t, leaf, tpi_leaf_print)                                         \
    FIELD_PRIMITIVE(uint16_t, size, "%u")                                                       \
    FIELD_UNION_DECL()                                                                          \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_class,                                                                   \
            class_,                                                                             \
            leaf,                                                                               \
            tpi_class_print,                                                                    \
            LF_CLASS, LF_CLASS_ST, LF_STRUCTURE, LF_STRUCTURE_ST, LF_STRUCTURE19, LF_INTERFACE) \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_member,                                                                  \
            member,                                                                             \
            leaf,                                                                               \
            tpi_member_print,                                                                   \
            LF_MEMBER, LF_MEMBER_ST)                                                            \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_nested_type,                                                             \
            nested_type,                                                                        \
            leaf,                                                                               \
            tpi_nested_type_print,                                                              \
            LF_NESTTYPE, LF_NESTTYPE_ST, LF_NESTTYPEEX, LF_NESTTYPEEX_ST)                       \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_member_function,                                                         \
            member_function,                                                                    \
            leaf,                                                                               \
            tpi_member_function_print,                                                          \
            LF_MFUNCTION)                                                                       \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_overloaded_method,                                                       \
            overloaded_method,                                                                  \
            leaf,                                                                               \
            tpi_overloaded_method_print,                                                        \
            LF_METHOD, LF_METHOD_ST)                                                            \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_method,                                                                  \
            method,                                                                             \
            leaf,                                                                               \
            tpi_method_print,                                                                   \
            LF_ONEMETHOD, LF_ONEMETHOD_ST)                                                      \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_base_class,                                                              \
            base_class,                                                                         \
            leaf,                                                                               \
            tpi_base_class_print,                                                               \
            LF_BCLASS, LF_BINTERFACE)                                                           \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_vtable,                                                                  \
            vtable,                                                                             \
            leaf,                                                                               \
            tpi_vtable_print,                                                                   \
            LF_VFUNCTAB)                                                                        \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_static_member,                                                           \
            static_member,                                                                      \
            leaf,                                                                               \
            tpi_static_member_print,                                                            \
            LF_STMEMBER, LF_STMEMBER_ST)                                                        \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_pointer,                                                                 \
            pointer,                                                                            \
            leaf,                                                                               \
            tpi_pointer_print,                                                                  \
            LF_POINTER)                                                                         \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_procedure,                                                               \
            procedure,                                                                          \
            leaf,                                                                               \
            tpi_procedure_print,                                                                \
            LF_PROCEDURE)                                                                       \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_modifier,                                                                \
            modifier,                                                                           \
            leaf,                                                                               \
            tpi_modifier_print,                                                                 \
            LF_MODIFIER)                                                                        \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_enumeration,                                                             \
            enumeration,                                                                        \
            leaf,                                                                               \
            tpi_enumeration_print,                                                              \
            LF_ENUM, LF_ENUM_ST)                                                                \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_enumerate,                                                               \
            enumerate,                                                                          \
            leaf,                                                                               \
            tpi_enumerate_print,                                                                \
            LF_ENUMERATE, LF_ENUMERATE_ST)                                                      \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_array,                                                                   \
            array,                                                                              \
            leaf,                                                                               \
            tpi_array_print,                                                                    \
            LF_ARRAY, LF_ARRAY_ST, LF_STRIDED_ARRAY)                                            \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_union,                                                                   \
            union_,                                                                             \
            leaf,                                                                               \
            tpi_union_print,                                                                    \
            LF_UNION, LF_UNION_ST)                                                              \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_bitfield,                                                                \
            bitfield,                                                                           \
            leaf,                                                                               \
            tpi_bitfield_print,                                                                 \
            LF_BITFIELD)                                                                        \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_virtual_base_class,                                                      \
            virtual_base_class,                                                                 \
            leaf,                                                                               \
            tpi_virtual_base_class_print,                                                       \
            LF_VBCLASS, LF_IVBCLASS)                                                            \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_field_list,                                                              \
            field_list,                                                                         \
            leaf,                                                                               \
            tpi_field_list_print,                                                               \
            LF_FIELDLIST)                                                                       \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_argument_list,                                                           \
            argument_list,                                                                      \
            leaf,                                                                               \
            tpi_argument_list_print,                                                            \
            LF_ARGLIST)                                                                         \
                                                                                                \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(                                                      \
            struct tpi_method_list,                                                             \
            method_list,                                                                        \
            leaf,                                                                               \
            tpi_method_list_print,                                                              \
            LF_METHODLIST)                                                                      \
                                                                                                \
    FIELD_UNION_END()                                                                           \
STRUCT_END(tpi_symbol)

TPI_SYMBOL_STRUCT

void tpi_symbol_read(struct tpi_symbol *symbol, struct msf *msf, struct msf_stream *msf_stream, struct tpi_header *tpi_header, uint32_t *out_offset, FILE *file_stream);
void tpi_symbol_dispose(struct tpi_symbol *symbol);
void tpi_symbol_print(struct tpi_symbol *symbol, uint32_t depth, FILE *stream);

/* ---------- TPI symbols */

#define TPI_SYMBOLS_STRUCT \
STRUCT_DECL(tpi_symbols) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct tpi_symbol *, symbols, count, tpi_symbol_print) \
STRUCT_END(tpi_symbols)

TPI_SYMBOLS_STRUCT

void tpi_symbols_read(struct tpi_symbols *symbols, struct msf *msf, struct msf_stream *msf_stream, struct tpi_header *tpi_header, FILE *file_stream);
void tpi_symbols_dispose(struct tpi_symbols *symbols);
void tpi_symbols_print(struct tpi_symbols *symbols, uint32_t depth, FILE *stream);

uint32_t tpi_index_to_absolute_index(struct tpi_header *tpi_header, struct tpi_symbols *tpi_symbols, uint32_t index);
int tpi_primitive_get(struct tpi_primitive *primitive, struct tpi_header *tpi_header, struct tpi_symbols *tpi_symbols, uint32_t index);
struct tpi_symbol *tpi_symbol_get(struct tpi_header *tpi_header, struct tpi_symbols *tpi_symbols, uint32_t index);
