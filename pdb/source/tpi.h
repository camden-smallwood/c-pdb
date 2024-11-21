#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"

struct tpi_slice
{
    uint32_t offset;
    uint32_t size;
};
static_assert(sizeof(struct tpi_slice) == 8);

void tpi_slice_print(struct tpi_slice *slice, uint32_t depth, FILE *stream);

struct tpi_header
{
    uint32_t version;
    uint32_t header_size;
    uint32_t minimum_index;
    uint32_t maximum_index;
    uint32_t gprec_size;
    uint16_t tpi_hash_stream;
    uint16_t tpi_hash_pad_stream;
    uint32_t hash_key_size;
    uint32_t hash_bucket_size;
    struct tpi_slice hash_values;
    struct tpi_slice ti_off;
    struct tpi_slice hash_adj;
};
static_assert(sizeof(struct tpi_header) == 56);

void tpi_header_read(struct msf *msf, struct tpi_header *out_header, FILE *stream);
void tpi_header_print(struct tpi_header *header, uint32_t depth, FILE *stream);

enum tpi_leaf
{
    // leaf indices starting records but referenced from symbol records

    LF_MODIFIER_16t = 0x0001,
    LF_POINTER_16t = 0x0002,
    LF_ARRAY_16t = 0x0003,
    LF_CLASS_16t = 0x0004,
    LF_STRUCTURE_16t = 0x0005,
    LF_UNION_16t = 0x0006,
    LF_ENUM_16t = 0x0007,
    LF_PROCEDURE_16t = 0x0008,
    LF_MFUNCTION_16t = 0x0009,
    LF_VTSHAPE = 0x000a,
    LF_COBOL0_16t = 0x000b,
    LF_COBOL1 = 0x000c,
    LF_BARRAY_16t = 0x000d,
    LF_LABEL = 0x000e,
    LF_NULL = 0x000f,
    LF_NOTTRAN = 0x0010,
    LF_DIMARRAY_16t = 0x0011,
    LF_VFTPATH_16t = 0x0012,
    LF_PRECOMP_16t = 0x0013,   // not referenced from symbol
    LF_ENDPRECOMP = 0x0014,    // not referenced from symbol
    LF_OEM_16t = 0x0015,       // oem definable type string
    LF_TYPESERVER_ST = 0x0016, // not referenced from symbol

    // leaf indices starting records but referenced only from type records

    LF_SKIP_16t = 0x0200,
    LF_ARGLIST_16t = 0x0201,
    LF_DEFARG_16t = 0x0202,
    LF_LIST = 0x0203,
    LF_FIELDLIST_16t = 0x0204,
    LF_DERIVED_16t = 0x0205,
    LF_BITFIELD_16t = 0x0206,
    LF_METHODLIST_16t = 0x0207,
    LF_DIMCONU_16t = 0x0208,
    LF_DIMCONLU_16t = 0x0209,
    LF_DIMVARU_16t = 0x020a,
    LF_DIMVARLU_16t = 0x020b,
    LF_REFSYM = 0x020c,

    LF_BCLASS_16t = 0x0400,
    LF_VBCLASS_16t = 0x0401,
    LF_IVBCLASS_16t = 0x0402,
    LF_ENUMERATE_ST = 0x0403,
    LF_FRIENDFCN_16t = 0x0404,
    LF_INDEX_16t = 0x0405,
    LF_MEMBER_16t = 0x0406,
    LF_STMEMBER_16t = 0x0407,
    LF_METHOD_16t = 0x0408,
    LF_NESTTYPE_16t = 0x0409,
    LF_VFUNCTAB_16t = 0x040a,
    LF_FRIENDCLS_16t = 0x040b,
    LF_ONEMETHOD_16t = 0x040c,
    LF_VFUNCOFF_16t = 0x040d,

    // 32-bit type index versions of leaves  all have the 0x1000 bit set

    LF_TI16_MAX = 0x1000,

    LF_MODIFIER = 0x1001,
    LF_POINTER = 0x1002,
    LF_ARRAY_ST = 0x1003,
    LF_CLASS_ST = 0x1004,
    LF_STRUCTURE_ST = 0x1005,
    LF_UNION_ST = 0x1006,
    LF_ENUM_ST = 0x1007,
    LF_PROCEDURE = 0x1008,
    LF_MFUNCTION = 0x1009,
    LF_COBOL0 = 0x100a,
    LF_BARRAY = 0x100b,
    LF_DIMARRAY_ST = 0x100c,
    LF_VFTPATH = 0x100d,
    LF_PRECOMP_ST = 0x100e, // not referenced from symbol
    LF_OEM = 0x100f,        // oem definable type string
    LF_ALIAS_ST = 0x1010,   // alias (typedef) type
    LF_OEM2 = 0x1011,       // oem definable type string

    // leaf indices starting records but referenced only from type records

    LF_SKIP = 0x1200,
    LF_ARGLIST = 0x1201,
    LF_DEFARG_ST = 0x1202,
    LF_FIELDLIST = 0x1203,
    LF_DERIVED = 0x1204,
    LF_BITFIELD = 0x1205,
    LF_METHODLIST = 0x1206,
    LF_DIMCONU = 0x1207,
    LF_DIMCONLU = 0x1208,
    LF_DIMVARU = 0x1209,
    LF_DIMVARLU = 0x120a,

    LF_BCLASS = 0x1400,
    LF_VBCLASS = 0x1401,
    LF_IVBCLASS = 0x1402,
    LF_FRIENDFCN_ST = 0x1403,
    LF_INDEX = 0x1404,
    LF_MEMBER_ST = 0x1405,
    LF_STMEMBER_ST = 0x1406,
    LF_METHOD_ST = 0x1407,
    LF_NESTTYPE_ST = 0x1408,
    LF_VFUNCTAB = 0x1409,
    LF_FRIENDCLS = 0x140a,
    LF_ONEMETHOD_ST = 0x140b,
    LF_VFUNCOFF = 0x140c,
    LF_NESTTYPEEX_ST = 0x140d,
    LF_MEMBERMODIFY_ST = 0x140e,
    LF_MANAGED_ST = 0x140f,

    // Types w/ SZ names

    LF_ST_MAX = 0x1500,

    LF_TYPESERVER = 0x1501, // not referenced from symbol
    LF_ENUMERATE = 0x1502,
    LF_ARRAY = 0x1503,
    LF_CLASS = 0x1504,
    LF_STRUCTURE = 0x1505,
    LF_UNION = 0x1506,
    LF_ENUM = 0x1507,
    LF_DIMARRAY = 0x1508,
    LF_PRECOMP = 0x1509, // not referenced from symbol
    LF_ALIAS = 0x150a,   // alias (typedef) type
    LF_DEFARG = 0x150b,
    LF_FRIENDFCN = 0x150c,
    LF_MEMBER = 0x150d,
    LF_STMEMBER = 0x150e,
    LF_METHOD = 0x150f,
    LF_NESTTYPE = 0x1510,
    LF_ONEMETHOD = 0x1511,
    LF_NESTTYPEEX = 0x1512,
    LF_MEMBERMODIFY = 0x1513,
    LF_MANAGED = 0x1514,
    LF_TYPESERVER2 = 0x1515,

    LF_STRIDED_ARRAY = 0x1516, // same as LF_ARRAY  but with stride between adjacent elements
    LF_HLSL = 0x1517,
    LF_MODIFIER_EX = 0x1518,
    LF_INTERFACE = 0x1519,
    LF_BINTERFACE = 0x151a,
    LF_VECTOR = 0x151b,
    LF_MATRIX = 0x151c,

    LF_VFTABLE = 0x151d, // a virtual function table
    
    LF_FUNC_ID = 0x1601,     // global func ID
    LF_MFUNC_ID = 0x1602,    // member func ID
    LF_BUILDINFO = 0x1603,   // build info: tool  version  command line  src/pdb file
    LF_SUBSTR_LIST = 0x1604, // similar to LF_ARGLIST  for list of sub strings
    LF_STRING_ID = 0x1605,   // string ID

    LF_UDT_SRC_LINE = 0x1606, // source and line on where an UDT is defined
                              // only generated by compiler

    LF_UDT_MOD_SRC_LINE = 0x1607, // module  source and line on where an UDT is defined
                                  // only generated by linker

    LF_STRUCTURE19 = 0x1609,

    LF_NUMERIC = 0x8000,
    LF_CHAR = 0x8000,
    LF_SHORT = 0x8001,
    LF_USHORT = 0x8002,
    LF_LONG = 0x8003,
    LF_ULONG = 0x8004,
    LF_REAL32 = 0x8005,
    LF_REAL64 = 0x8006,
    LF_REAL80 = 0x8007,
    LF_REAL128 = 0x8008,
    LF_QUADWORD = 0x8009,
    LF_UQUADWORD = 0x800a,
    LF_REAL48 = 0x800b,
    LF_COMPLEX32 = 0x800c,
    LF_COMPLEX64 = 0x800d,
    LF_COMPLEX80 = 0x800e,
    LF_COMPLEX128 = 0x800f,
    LF_VARSTRING = 0x8010,

    LF_OCTWORD = 0x8017,
    LF_UOCTWORD = 0x8018,

    LF_DECIMAL = 0x8019,
    LF_DATE = 0x801a,
    LF_UTF8STRING = 0x801b,

    LF_REAL16 = 0x801c,

    LF_PAD0 = 0xf0,
    LF_PAD1 = 0xf1,
    LF_PAD2 = 0xf2,
    LF_PAD3 = 0xf3,
    LF_PAD4 = 0xf4,
    LF_PAD5 = 0xf5,
    LF_PAD6 = 0xf6,
    LF_PAD7 = 0xf7,
    LF_PAD8 = 0xf8,
    LF_PAD9 = 0xf9,
    LF_PAD10 = 0xfa,
    LF_PAD11 = 0xfb,
    LF_PAD12 = 0xfc,
    LF_PAD13 = 0xfd,
    LF_PAD14 = 0xfe,
    LF_PAD15 = 0xff,
};

void tpi_leaf_print(enum tpi_leaf leaf, FILE *stream);

struct tpi_properties
{
    uint16_t packed : 1;        // true if structure is packed
    uint16_t ctor : 1;          // true if constructors or destructors present
    uint16_t ovlops : 1;        // true if overloaded operators present
    uint16_t isnested : 1;      // true if this is a nested class
    uint16_t cnested : 1;       // true if this class contains nested types
    uint16_t opassign : 1;      // true if overloaded assignment (=)
    uint16_t opcast : 1;        // true if casting methods
    uint16_t fwdref : 1;        // true if forward reference (incomplete defn)
    uint16_t scoped : 1;        // scoped definition
    uint16_t hasuniquename : 1; // true if there is a decorated name following the regular name
    uint16_t sealed : 1;        // true if class cannot be used as a base class
    uint16_t hfa : 2;           // CV_HFA_e
    uint16_t intrinsic : 1;     // true if class is an intrinsic type (e.g. __m128d)
    uint16_t mocom : 2;         // CV_MOCOM_UDT_e
};
static_assert(sizeof(struct tpi_properties) == sizeof(uint16_t));

void tpi_properties_print(struct tpi_properties *properties, uint32_t depth, FILE *stream);

struct tpi_field_attributes
{
    uint16_t access : 2;      // access protection CV_access_t
    uint16_t mprop : 3;       // method properties CV_methodprop_t
    uint16_t pseudo : 1;      // compiler generated fcn and does not exist
    uint16_t noinherit : 1;   // true if class cannot be inherited
    uint16_t noconstruct : 1; // true if class cannot be constructed
    uint16_t compgenx : 1;    // compiler generated fcn and does exist
    uint16_t sealed : 1;      // true if method cannot be overridden
    uint16_t unused : 6;      // unused
};
static_assert(sizeof(struct tpi_field_attributes) == sizeof(uint16_t));

void tpi_field_attributes_print(struct tpi_field_attributes *attributes, uint32_t depth, FILE *stream);

struct tpi_field_list
{
    uint32_t count;
    struct tpi_symbol *fields;
    uint32_t continuation_type_index;
};

void tpi_field_list_dispose(struct tpi_field_list *item);
void tpi_field_list_print(struct tpi_field_list *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_member_header
{
    uint16_t attributes;
    uint32_t field_type;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_member_header) == 6);

void tpi_member_header_print(struct tpi_member_header *header, uint32_t depth, FILE *stream);

struct tpi_member
{
    struct tpi_member_header header;
    uint64_t offset;
    char *name;
};

void tpi_member_read(struct tpi_member *item, uint32_t leaf, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_member_dispose(struct tpi_member *item);
void tpi_member_print(struct tpi_member *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_static_member_header
{
    struct tpi_field_attributes attributes;
    uint32_t field_type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_static_member_header) == 6);

void tpi_static_member_header_print(struct tpi_static_member_header *header, uint32_t depth, FILE *stream);

struct tpi_static_member
{
    struct tpi_static_member_header header;
    char *name;
};

void tpi_static_member_dispose(struct tpi_static_member *item);
void tpi_static_member_print(struct tpi_static_member *item, uint32_t depth, FILE *stream);

enum tpi_pointer_type
{
    CV_PTR_NEAR = 0x00,         // 16 bit pointer
    CV_PTR_FAR = 0x01,          // 16:16 far pointer
    CV_PTR_HUGE = 0x02,         // 16:16 huge pointer
    CV_PTR_BASE_SEG = 0x03,     // based on segment
    CV_PTR_BASE_VAL = 0x04,     // based on value of base
    CV_PTR_BASE_SEGVAL = 0x05,  // based on segment value of base
    CV_PTR_BASE_ADDR = 0x06,    // based on address of base
    CV_PTR_BASE_SEGADDR = 0x07, // based on segment address of base
    CV_PTR_BASE_TYPE = 0x08,    // based on type
    CV_PTR_BASE_SELF = 0x09,    // based on self
    CV_PTR_NEAR32 = 0x0a,       // 32 bit pointer
    CV_PTR_FAR32 = 0x0b,        // 16:32 pointer
    CV_PTR_64 = 0x0c,           // 64 bit pointer
    CV_PTR_UNUSEDPTR = 0x0d     // first unused pointer type
};

enum tpi_pointer_mode
{
    CV_PTR_MODE_PTR = 0x00,     // "normal" pointer
    CV_PTR_MODE_REF = 0x01,     // "old" reference
    CV_PTR_MODE_LVREF = 0x01,   // l-value reference
    CV_PTR_MODE_PMEM = 0x02,    // pointer to data member
    CV_PTR_MODE_PMFUNC = 0x03,  // pointer to member function
    CV_PTR_MODE_RVREF = 0x04,   // r-value reference
    CV_PTR_MODE_RESERVED = 0x05 // first unused pointer mode
};

struct tpi_pointer_attributes
{
    uint32_t ptrtype : 5;     // ordinal specifying pointer type (CV_ptrtype_e)
    uint32_t ptrmode : 3;     // ordinal specifying pointer mode (CV_ptrmode_e)
    uint32_t isflat32 : 1;    // true if 0:32 pointer
    uint32_t isvolatile : 1;  // TRUE if volatile pointer
    uint32_t isconst : 1;     // TRUE if const pointer
    uint32_t isunaligned : 1; // TRUE if unaligned pointer
    uint32_t isrestrict : 1;  // TRUE if restricted pointer (allow agressive opts)
    uint32_t size : 6;        // size of pointer (in bytes)
    uint32_t ismocom : 1;     // TRUE if it is a MoCOM pointer (^ or %)
    uint32_t islref : 1;      // TRUE if it is this pointer of member function with & ref-qualifier
    uint32_t isrref : 1;      // TRUE if it is this pointer of member function with && ref-qualifier
    uint32_t unused : 10;     // pad out to 32-bits for following cv_typ_t's
};
static_assert(sizeof(struct tpi_pointer_attributes) == sizeof(uint32_t));

void tpi_pointer_attributes_print(struct tpi_pointer_attributes *attributes, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_pointer_header
{
    uint32_t underlying_type_index;
    struct tpi_pointer_attributes attributes;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_pointer_header) == 8);

void tpi_pointer_header_print(struct tpi_pointer_header *header, uint32_t depth, FILE *stream);

struct tpi_pointer
{
    struct tpi_pointer_header header;
    uint32_t containing_class_type_index;
};

void tpi_pointer_print(struct tpi_pointer *item, uint32_t depth, FILE *stream);

struct tpi_argument_list
{
    uint32_t count;
    uint32_t *type_indices;
};

void tpi_argument_list_dispose(struct tpi_argument_list *item);
void tpi_argument_list_print(struct tpi_argument_list *item, uint32_t depth, FILE *stream);

struct tpi_procedure_attributes
{
    uint8_t cxxreturnudt : 1; // true if C++ style ReturnUDT
    uint8_t ctor : 1;         // true if func is an instance constructor
    uint8_t ctorvbase : 1;    // true if func is an instance constructor of a class with virtual bases
    uint8_t unused : 5;       // unused
};
static_assert(sizeof(struct tpi_procedure_attributes) == sizeof(uint8_t));

void tpi_procedure_attributes_print(struct tpi_procedure_attributes *attributes, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_procedure
{
    uint32_t return_type_index;
    struct tpi_procedure_attributes attributes;
    uint16_t parameter_count;
    uint32_t argument_list_type_index;
    uint8_t padding;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_procedure) == 12);

void tpi_procedure_print(struct tpi_procedure *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_modifier
{
    uint32_t underlying_type_index;
    uint16_t flags;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_modifier) == 6);

void tpi_modifier_print(struct tpi_modifier *item, uint32_t depth, FILE *stream);

enum tpi_enumerate_variant_type
{
    TPI_ENUMERATE_VARIANT_UINT8,
    TPI_ENUMERATE_VARIANT_UINT16,
    TPI_ENUMERATE_VARIANT_UINT32,
    TPI_ENUMERATE_VARIANT_UINT64,
    TPI_ENUMERATE_VARIANT_INT8,
    TPI_ENUMERATE_VARIANT_INT16,
    TPI_ENUMERATE_VARIANT_INT32,
    TPI_ENUMERATE_VARIANT_INT64,
};

void tpi_enumerate_variant_type_print(enum tpi_enumerate_variant_type type, FILE *stream);

struct tpi_enumerate_variant
{
    enum tpi_enumerate_variant_type type;
    union
    {
        uint8_t uint8;
        uint16_t uint16;
        uint32_t uint32;
        uint64_t uint64;
        int8_t int8;
        int16_t int16;
        int32_t int32;
        int64_t int64;
    };
};

void tpi_enumerate_variant_read(struct tpi_enumerate_variant *variant, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_enumerate_variant_print(struct tpi_enumerate_variant *variant, uint32_t depth, FILE *stream);

struct tpi_enumerate
{
    struct tpi_field_attributes attributes;
    struct tpi_enumerate_variant variant;
    char *name;
};

void tpi_enumerate_dispose(struct tpi_enumerate *item);
void tpi_enumerate_print(struct tpi_enumerate *item, uint32_t depth, FILE *stream);

struct tpi_enumeration_header
{
    uint16_t count;
    struct tpi_properties properties;
    uint32_t underlying_type_index;
    uint32_t fields_type_index;
};
static_assert(sizeof(struct tpi_enumeration_header) == 12);

void tpi_enumeration_header_print(struct tpi_enumeration_header *header, uint32_t depth, FILE *stream);

struct tpi_enumeration
{
    struct tpi_enumeration_header header;
    char *name;
    char *unique_name;
};

void tpi_enumeration_dispose(struct tpi_enumeration *item);
void tpi_enumeration_print(struct tpi_enumeration *item, uint32_t depth, FILE *stream);

struct tpi_array_header
{
    uint32_t element_type_index;
    uint32_t indexing_type_index;
};
static_assert(sizeof(struct tpi_array_header) == 8);

void tpi_array_header_print(struct tpi_array_header *header, uint32_t depth, FILE *stream);

struct tpi_array
{
    struct tpi_array_header header;
    uint32_t stride;
    uint32_t dimension_count;
    uint32_t *dimensions;
};

void tpi_array_dispose(struct tpi_array *item);
void tpi_array_print(struct tpi_array *item, uint32_t depth, FILE *stream);

struct tpi_union_header
{
    uint16_t count;
    struct tpi_properties properties;
    uint32_t fields_type_index;
};
static_assert(sizeof(struct tpi_union_header) == 8);

void tpi_union_header_print(struct tpi_union_header *header, uint32_t depth, FILE *stream);

struct tpi_union
{
    struct tpi_union_header header;
    uint64_t size;
    char *name;
    char *unique_name;
};

void tpi_union_dispose(struct tpi_union *item);
void tpi_union_print(struct tpi_union *item, uint32_t depth, FILE *stream);

struct tpi_bitfield
{
    uint32_t underlying_type_index;
    uint8_t length;
    uint8_t position;
    uint16_t padding;
};
static_assert(sizeof(struct tpi_bitfield) == 8);

void tpi_bitfield_print(struct tpi_bitfield *item, uint32_t depth, FILE *stream);

struct tpi_member_function
{
    uint32_t return_type_index;
    uint32_t class_type_index;
    uint32_t this_pointer_type_index;
    uint16_t attributes;
    uint16_t parameter_count;
    uint32_t argument_list_type_index;
    uint32_t this_adjustment;
};
static_assert(sizeof(struct tpi_member_function) == 24);

void tpi_member_function_read(struct tpi_member_function *item, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_member_function_print(struct tpi_member_function *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_overloaded_method_header
{
    uint16_t count;
    uint32_t method_list_type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_overloaded_method_header) == 6);

void tpi_overloaded_method_header_print(struct tpi_overloaded_method_header *header, uint32_t depth, FILE *stream);

struct tpi_overloaded_method
{
    struct tpi_overloaded_method_header header;
    char *name;
};

void tpi_overloaded_method_dispose(struct tpi_overloaded_method *item);
void tpi_overloaded_method_print(struct tpi_overloaded_method *item, uint32_t depth, FILE *stream);

enum tpi_method_attributes
{
    TPI_METHOD_VANILLA = 0x00,
    TPI_METHOD_VIRTUAL = 0x01,
    TPI_METHOD_STATIC = 0x02,
    TPI_METHOD_FRIEND = 0x03,
    TPI_METHOD_INTRO = 0x04,
    TPI_METHOD_PUREVIRT = 0x05,
    TPI_METHOD_PUREINTRO = 0x06,
};

#pragma pack(push, 1)
struct tpi_method_header
{
    struct tpi_field_attributes attributes;
    uint32_t method_type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_method_header) == 6);

void tpi_method_header_print(struct tpi_method_header *header, uint32_t depth, FILE *stream);

struct tpi_method
{
    struct tpi_method_header header;
    uint32_t vtable_offset;
    char *name;
};

void tpi_method_dispose(struct tpi_method *item);
void tpi_method_print(struct tpi_method *item, uint32_t depth, FILE *stream);

struct tpi_method_list_entry_header
{
    struct tpi_field_attributes attributes;
    uint16_t padding;
    uint32_t method_type_index;
};
static_assert(sizeof(struct tpi_method_list_entry_header) == 8);

void tpi_method_list_entry_header_print(struct tpi_method_list_entry_header *header, uint32_t depth, FILE *stream);

struct tpi_method_list_entry
{
    struct tpi_method_list_entry_header header;
    uint32_t vtable_offset;
};

void tpi_method_list_entry_print(struct tpi_method_list_entry *entry, uint32_t depth, FILE *stream);

struct tpi_method_list
{
    uint32_t count;
    struct tpi_method_list_entry *entries;
};

void tpi_method_list_dispose(struct tpi_method_list *item);
void tpi_method_list_print(struct tpi_method_list *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_vtable
{
    uint16_t padding;
    uint32_t type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_vtable) == 6);

void tpi_vtable_print(struct tpi_vtable *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_nested_type_header
{
    uint16_t attributes;
    uint32_t nested_type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_nested_type_header) == 6);

void tpi_nested_type_header_print(struct tpi_nested_type_header *header, uint32_t depth, FILE *stream);

struct tpi_nested_type
{
    struct tpi_nested_type_header header;
    char *name;
};

void tpi_nested_type_read(struct tpi_nested_type *item, uint32_t leaf, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);
void tpi_nested_type_dispose(struct tpi_nested_type *item);
void tpi_nested_type_print(struct tpi_nested_type *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_base_class_header
{
    struct tpi_field_attributes attributes;
    uint32_t base_class_type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_base_class_header) == 6);

void tpi_base_class_header_print(struct tpi_base_class_header *header, uint32_t depth, FILE *stream);

struct tpi_base_class
{
    struct tpi_base_class_header header;
    uint32_t offset;
};

void tpi_base_class_print(struct tpi_base_class *item, uint32_t depth, FILE *stream);

#pragma pack(push, 1)
struct tpi_virtual_base_class_header
{
    struct tpi_field_attributes attributes;
    uint32_t base_class_type_index;
    uint32_t base_pointer_type_index;
};
#pragma pack(pop)
static_assert(sizeof(struct tpi_virtual_base_class_header) == 10);

void tpi_virtual_base_class_header_print(struct tpi_virtual_base_class_header *header, uint32_t depth, FILE *stream);

struct tpi_virtual_base_class
{
    struct tpi_virtual_base_class_header header;
    uint32_t base_pointer_offset;
    uint32_t virtual_base_offset;
};

void tpi_virtual_base_class_print(struct tpi_virtual_base_class *item, uint32_t depth, FILE *stream);

struct tpi_class_header
{
    uint16_t count;
    struct tpi_properties properties;
    uint32_t fields_type_index;
};
static_assert(sizeof(struct tpi_class_header) == 8);

void tpi_class_header_print(struct tpi_class_header *header, uint32_t depth, FILE *stream);

struct tpi_class
{
    struct tpi_class_header header;
    uint32_t derived_from_type_index;
    uint32_t vtable_shape_type_index;
    uint64_t size;
    char *name;
    char *unique_name;
};

void tpi_class_read(struct tpi_class *item, uint32_t leaf, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, uint32_t end_offset, FILE *file_stream);
void tpi_class_dispose(struct tpi_class *item);
void tpi_class_print(struct tpi_class *item, uint32_t depth, FILE *stream);

struct tpi_symbol
{
    uint16_t leaf;
    uint16_t size;
    union
    {
        struct tpi_class class_;
        struct tpi_member member;
        struct tpi_nested_type nested_type;
        struct tpi_member_function member_function;
        struct tpi_overloaded_method overloaded_method;
        struct tpi_method method;
        struct tpi_base_class base_class;
        struct tpi_vtable vtable;
        struct tpi_static_member static_member;
        struct tpi_pointer pointer;
        struct tpi_procedure procedure;
        struct tpi_modifier modifier;
        struct tpi_enumeration enumeration;
        struct tpi_enumerate enumerate;
        struct tpi_array array;
        struct tpi_union union_;
        struct tpi_bitfield bitfield;
        struct tpi_virtual_base_class virtual_base_class;
        struct tpi_field_list field_list;
        struct tpi_argument_list argument_list;
        struct tpi_method_list method_list;
    };
};

void tpi_symbol_read(struct tpi_symbol *symbol, struct msf *msf, struct msf_stream *msf_stream, struct tpi_header *tpi_header, uint32_t *out_offset, FILE *file_stream);
void tpi_symbol_dispose(struct tpi_symbol *symbol);
void tpi_symbol_print(struct tpi_symbol *symbol, uint32_t depth, FILE *stream);

struct tpi_symbols
{
    uint32_t count;
    struct tpi_symbol *symbols;
};

void tpi_symbols_read(struct tpi_symbols *symbols, struct msf *msf, struct msf_stream *msf_stream, struct tpi_header *tpi_header, FILE *file_stream);
void tpi_symbols_dispose(struct tpi_symbols *symbols);
void tpi_symbols_print(struct tpi_symbols *symbols, uint32_t depth, FILE *stream);
