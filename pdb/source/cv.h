#pragma once
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "msf.h"
#include "macros_decl.h"

/* ---------- CV symbol types */

#define CV_SYMBOL_TYPE_ENUM                                    \
ENUM_DECL(cv_symbol_type)                                      \
    ENUM_VALUE(S_COMPILE, 0x0001)                              \
    ENUM_VALUE(S_REGISTER_16t, 0x0002)                         \
    ENUM_VALUE(S_CONSTANT_16t, 0x0003)                         \
    ENUM_VALUE(S_UDT_16t, 0x0004)                              \
    ENUM_VALUE(S_SSEARCH, 0x0005)                              \
    ENUM_VALUE(S_END, 0x0006)                                  \
    ENUM_VALUE(S_SKIP, 0x0007)                                 \
    ENUM_VALUE(S_CVRESERVE, 0x0008)                            \
    ENUM_VALUE(S_OBJNAME_ST, 0x0009)                           \
    ENUM_VALUE(S_ENDARG, 0x000a)                               \
    ENUM_VALUE(S_COBOLUDT_16t, 0x000b)                         \
    ENUM_VALUE(S_MANYREG_16t, 0x000c)                          \
    ENUM_VALUE(S_RETURN, 0x000d)                               \
    ENUM_VALUE(S_ENTRYTHIS, 0x000e)                            \
    ENUM_VALUE(S_BPREL16, 0x0100)                              \
    ENUM_VALUE(S_LDATA16, 0x0101)                              \
    ENUM_VALUE(S_GDATA16, 0x0102)                              \
    ENUM_VALUE(S_PUB16, 0x0103)                                \
    ENUM_VALUE(S_LPROC16, 0x0104)                              \
    ENUM_VALUE(S_GPROC16, 0x0105)                              \
    ENUM_VALUE(S_THUNK16, 0x0106)                              \
    ENUM_VALUE(S_BLOCK16, 0x0107)                              \
    ENUM_VALUE(S_WITH16, 0x0108)                               \
    ENUM_VALUE(S_LABEL16, 0x0109)                              \
    ENUM_VALUE(S_CEXMODEL16, 0x010a)                           \
    ENUM_VALUE(S_VFTABLE16, 0x010b)                            \
    ENUM_VALUE(S_REGREL16, 0x010c)                             \
    ENUM_VALUE(S_BPREL32_16t, 0x0200)                          \
    ENUM_VALUE(S_LDATA32_16t, 0x0201)                          \
    ENUM_VALUE(S_GDATA32_16t, 0x0202)                          \
    ENUM_VALUE(S_PUB32_16t, 0x0203)                            \
    ENUM_VALUE(S_LPROC32_16t, 0x0204)                          \
    ENUM_VALUE(S_GPROC32_16t, 0x0205)                          \
    ENUM_VALUE(S_THUNK32_ST, 0x0206)                           \
    ENUM_VALUE(S_BLOCK32_ST, 0x0207)                           \
    ENUM_VALUE(S_WITH32_ST, 0x0208)                            \
    ENUM_VALUE(S_LABEL32_ST, 0x0209)                           \
    ENUM_VALUE(S_CEXMODEL32, 0x020a)                           \
    ENUM_VALUE(S_VFTABLE32_16t, 0x020b)                        \
    ENUM_VALUE(S_REGREL32_16t, 0x020c)                         \
    ENUM_VALUE(S_LTHREAD32_16t, 0x020d)                        \
    ENUM_VALUE(S_GTHREAD32_16t, 0x020e)                        \
    ENUM_VALUE(S_SLINK32, 0x020f)                              \
    ENUM_VALUE(S_LPROCMIPS_16t, 0x0300)                        \
    ENUM_VALUE(S_GPROCMIPS_16t, 0x0301)                        \
    ENUM_VALUE(S_PROCREF_ST, 0x0400)                           \
    ENUM_VALUE(S_DATAREF_ST, 0x0401)                           \
    ENUM_VALUE(S_ALIGN, 0x0402)                                \
    ENUM_VALUE(S_LPROCREF_ST, 0x0403)                          \
    ENUM_VALUE(S_OEM, 0x0404)                                  \
    ENUM_VALUE(S_TI16_MAX, 0x1000)                             \
    ENUM_VALUE(S_REGISTER_ST, 0x1001)                          \
    ENUM_VALUE(S_CONSTANT_ST, 0x1002)                          \
    ENUM_VALUE(S_UDT_ST, 0x1003)                               \
    ENUM_VALUE(S_COBOLUDT_ST, 0x1004)                          \
    ENUM_VALUE(S_MANYREG_ST, 0x1005)                           \
    ENUM_VALUE(S_BPREL32_ST, 0x1006)                           \
    ENUM_VALUE(S_LDATA32_ST, 0x1007)                           \
    ENUM_VALUE(S_GDATA32_ST, 0x1008)                           \
    ENUM_VALUE(S_PUB32_ST, 0x1009)                             \
    ENUM_VALUE(S_LPROC32_ST, 0x100a)                           \
    ENUM_VALUE(S_GPROC32_ST, 0x100b)                           \
    ENUM_VALUE(S_VFTABLE32, 0x100c)                            \
    ENUM_VALUE(S_REGREL32_ST, 0x100d)                          \
    ENUM_VALUE(S_LTHREAD32_ST, 0x100e)                         \
    ENUM_VALUE(S_GTHREAD32_ST, 0x100f)                         \
    ENUM_VALUE(S_LPROCMIPS_ST, 0x1010)                         \
    ENUM_VALUE(S_GPROCMIPS_ST, 0x1011)                         \
    ENUM_VALUE(S_FRAMEPROC, 0x1012)                            \
    ENUM_VALUE(S_COMPILE2_ST, 0x1013)                          \
    ENUM_VALUE(S_MANYREG2_ST, 0x1014)                          \
    ENUM_VALUE(S_LPROCIA64_ST, 0x1015)                         \
    ENUM_VALUE(S_GPROCIA64_ST, 0x1016)                         \
    ENUM_VALUE(S_LOCALSLOT_ST, 0x1017)                         \
    ENUM_VALUE(S_PARAMSLOT_ST, 0x1018)                         \
    ENUM_VALUE(S_ANNOTATION, 0x1019)                           \
    ENUM_VALUE(S_GMANPROC_ST, 0x101a)                          \
    ENUM_VALUE(S_LMANPROC_ST, 0x101b)                          \
    ENUM_VALUE(S_RESERVED1, 0x101c)                            \
    ENUM_VALUE(S_RESERVED2, 0x101d)                            \
    ENUM_VALUE(S_RESERVED3, 0x101e)                            \
    ENUM_VALUE(S_RESERVED4, 0x101f)                            \
    ENUM_VALUE(S_LMANDATA_ST, 0x1020)                          \
    ENUM_VALUE(S_GMANDATA_ST, 0x1021)                          \
    ENUM_VALUE(S_MANFRAMEREL_ST, 0x1022)                       \
    ENUM_VALUE(S_MANREGISTER_ST, 0x1023)                       \
    ENUM_VALUE(S_MANSLOT_ST, 0x1024)                           \
    ENUM_VALUE(S_MANMANYREG_ST, 0x1025)                        \
    ENUM_VALUE(S_MANREGREL_ST, 0x1026)                         \
    ENUM_VALUE(S_MANMANYREG2_ST, 0x1027)                       \
    ENUM_VALUE(S_MANTYPREF, 0x1028)                            \
    ENUM_VALUE(S_UNAMESPACE_ST, 0x1029)                        \
    ENUM_VALUE(S_ST_MAX, 0x1100)                               \
    ENUM_VALUE(S_OBJNAME, 0x1101)                              \
    ENUM_VALUE(S_THUNK32, 0x1102)                              \
    ENUM_VALUE(S_BLOCK32, 0x1103)                              \
    ENUM_VALUE(S_WITH32, 0x1104)                               \
    ENUM_VALUE(S_LABEL32, 0x1105)                              \
    ENUM_VALUE(S_REGISTER, 0x1106)                             \
    ENUM_VALUE(S_CONSTANT, 0x1107)                             \
    ENUM_VALUE(S_UDT, 0x1108)                                  \
    ENUM_VALUE(S_COBOLUDT, 0x1109)                             \
    ENUM_VALUE(S_MANYREG, 0x110a)                              \
    ENUM_VALUE(S_BPREL32, 0x110b)                              \
    ENUM_VALUE(S_LDATA32, 0x110c)                              \
    ENUM_VALUE(S_GDATA32, 0x110d)                              \
    ENUM_VALUE(S_PUB32, 0x110e)                                \
    ENUM_VALUE(S_LPROC32, 0x110f)                              \
    ENUM_VALUE(S_GPROC32, 0x1110)                              \
    ENUM_VALUE(S_REGREL32, 0x1111)                             \
    ENUM_VALUE(S_LTHREAD32, 0x1112)                            \
    ENUM_VALUE(S_GTHREAD32, 0x1113)                            \
    ENUM_VALUE(S_LPROCMIPS, 0x1114)                            \
    ENUM_VALUE(S_GPROCMIPS, 0x1115)                            \
    ENUM_VALUE(S_COMPILE2, 0x1116)                             \
    ENUM_VALUE(S_MANYREG2, 0x1117)                             \
    ENUM_VALUE(S_LPROCIA64, 0x1118)                            \
    ENUM_VALUE(S_GPROCIA64, 0x1119)                            \
    ENUM_VALUE(S_LOCALSLOT, 0x111a)                            \
    ENUM_VALUE(S_SLOT, S_LOCALSLOT)                            \
    ENUM_VALUE(S_PARAMSLOT, 0x111b)                            \
    ENUM_VALUE(S_LMANDATA, 0x111c)                             \
    ENUM_VALUE(S_GMANDATA, 0x111d)                             \
    ENUM_VALUE(S_MANFRAMEREL, 0x111e)                          \
    ENUM_VALUE(S_MANREGISTER, 0x111f)                          \
    ENUM_VALUE(S_MANSLOT, 0x1120)                              \
    ENUM_VALUE(S_MANMANYREG, 0x1121)                           \
    ENUM_VALUE(S_MANREGREL, 0x1122)                            \
    ENUM_VALUE(S_MANMANYREG2, 0x1123)                          \
    ENUM_VALUE(S_UNAMESPACE, 0x1124)                           \
    ENUM_VALUE(S_PROCREF, 0x1125)                              \
    ENUM_VALUE(S_DATAREF, 0x1126)                              \
    ENUM_VALUE(S_LPROCREF, 0x1127)                             \
    ENUM_VALUE(S_ANNOTATIONREF, 0x1128)                        \
    ENUM_VALUE(S_TOKENREF, 0x1129)                             \
    ENUM_VALUE(S_GMANPROC, 0x112a)                             \
    ENUM_VALUE(S_LMANPROC, 0x112b)                             \
    ENUM_VALUE(S_TRAMPOLINE, 0x112c)                           \
    ENUM_VALUE(S_MANCONSTANT, 0x112d)                          \
    ENUM_VALUE(S_ATTR_FRAMEREL, 0x112e)                        \
    ENUM_VALUE(S_ATTR_REGISTER, 0x112f)                        \
    ENUM_VALUE(S_ATTR_REGREL, 0x1130)                          \
    ENUM_VALUE(S_ATTR_MANYREG, 0x1131)                         \
    ENUM_VALUE(S_SEPCODE, 0x1132)                              \
    ENUM_VALUE(S_LOCAL_2005, 0x1133)                           \
    ENUM_VALUE(S_DEFRANGE_2005, 0x1134)                        \
    ENUM_VALUE(S_DEFRANGE2_2005, 0x1135)                       \
    ENUM_VALUE(S_SECTION, 0x1136)                              \
    ENUM_VALUE(S_COFFGROUP, 0x1137)                            \
    ENUM_VALUE(S_EXPORT, 0x1138)                               \
    ENUM_VALUE(S_CALLSITEINFO, 0x1139)                         \
    ENUM_VALUE(S_FRAMECOOKIE, 0x113a)                          \
    ENUM_VALUE(S_DISCARDED, 0x113b)                            \
    ENUM_VALUE(S_COMPILE3, 0x113c)                             \
    ENUM_VALUE(S_ENVBLOCK, 0x113d)                             \
    ENUM_VALUE(S_LOCAL, 0x113e)                                \
    ENUM_VALUE(S_DEFRANGE, 0x113f)                             \
    ENUM_VALUE(S_DEFRANGE_SUBFIELD, 0x1140)                    \
    ENUM_VALUE(S_DEFRANGE_REGISTER, 0x1141)                    \
    ENUM_VALUE(S_DEFRANGE_FRAMEPOINTER_REL, 0x1142)            \
    ENUM_VALUE(S_DEFRANGE_SUBFIELD_REGISTER, 0x1143)           \
    ENUM_VALUE(S_DEFRANGE_FRAMEPOINTER_REL_FULL_SCOPE, 0x1144) \
    ENUM_VALUE(S_DEFRANGE_REGISTER_REL, 0x1145)                \
    ENUM_VALUE(S_LPROC32_ID, 0x1146)                           \
    ENUM_VALUE(S_GPROC32_ID, 0x1147)                           \
    ENUM_VALUE(S_LPROCMIPS_ID, 0x1148)                         \
    ENUM_VALUE(S_GPROCMIPS_ID, 0x1149)                         \
    ENUM_VALUE(S_LPROCIA64_ID, 0x114a)                         \
    ENUM_VALUE(S_GPROCIA64_ID, 0x114b)                         \
    ENUM_VALUE(S_BUILDINFO, 0x114c)                            \
    ENUM_VALUE(S_INLINESITE, 0x114d)                           \
    ENUM_VALUE(S_INLINESITE_END, 0x114e)                       \
    ENUM_VALUE(S_PROC_ID_END, 0x114f)                          \
    ENUM_VALUE(S_DEFRANGE_HLSL, 0x1150)                        \
    ENUM_VALUE(S_GDATA_HLSL, 0x1151)                           \
    ENUM_VALUE(S_LDATA_HLSL, 0x1152)                           \
    ENUM_VALUE(S_FILESTATIC, 0x1153)                           \
    ENUM_VALUE(S_LOCAL_DPC_GROUPSHARED, 0x1154)                \
    ENUM_VALUE(S_LPROC32_DPC, 0x1155)                          \
    ENUM_VALUE(S_LPROC32_DPC_ID, 0x1156)                       \
    ENUM_VALUE(S_DEFRANGE_DPC_PTR_TAG, 0x1157)                 \
    ENUM_VALUE(S_DPC_SYM_TAG_MAP, 0x1158)                      \
    ENUM_VALUE(S_ARMSWITCHTABLE, 0x1159)                       \
    ENUM_VALUE(S_CALLEES, 0x115a)                              \
    ENUM_VALUE(S_CALLERS, 0x115b)                              \
    ENUM_VALUE(S_POGODATA, 0x115c)                             \
    ENUM_VALUE(S_INLINESITE2, 0x115d)                          \
    ENUM_VALUE(S_HEAPALLOCSITE, 0x115e)                        \
    ENUM_VALUE(S_MOD_TYPEREF, 0x115f)                          \
    ENUM_VALUE(S_REF_MINIPDB, 0x1160)                          \
    ENUM_VALUE(S_PDBMAP, 0x1161)                               \
    ENUM_VALUE(S_GDATA_HLSL32, 0x1162)                         \
    ENUM_VALUE(S_LDATA_HLSL32, 0x1163)                         \
    ENUM_VALUE(S_GDATA_HLSL32_EX, 0x1164)                      \
    ENUM_VALUE(S_LDATA_HLSL32_EX, 0x1165)                      \
ENUM_END(cv_symbol_type)

CV_SYMBOL_TYPE_ENUM

void cv_symbol_type_print(enum cv_symbol_type type, FILE *stream);

/* ---------- CV PE section offset */

#define CV_PE_SECTION_OFFSET_STRUCT \
STRUCT_DECL(cv_pe_section_offset) \
    FIELD_PRIMITIVE(uint32_t, memory_offset, "%u") \
    FIELD_PRIMITIVE(uint16_t, section_index, "%u") \
STRUCT_END(cv_pe_section_offset)

CV_PE_SECTION_OFFSET_STRUCT

void cv_pe_section_offset_print(struct cv_pe_section_offset *item, uint32_t depth, FILE *stream);
void cv_pe_section_offset_read(struct cv_pe_section_offset *item, struct msf *msf, struct msf_stream *msf_stream, uint32_t *out_offset, FILE *file_stream);

/* ---------- CV obj name symbol */

#define CV_OBJ_NAME_SYMBOL_STRUCT \
STRUCT_DECL(cv_obj_name_symbol) \
    FIELD_PRIMITIVE(uint32_t, signature, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_obj_name_symbol)

CV_OBJ_NAME_SYMBOL_STRUCT

void cv_obj_name_symbol_dispose(struct cv_obj_name_symbol *item);
void cv_obj_name_symbol_print(struct cv_obj_name_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV register variable symbol */

#define CV_REGISTER_VARIABLE_SYMBOL_STRUCT \
STRUCT_DECL(cv_register_variable_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, register_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_register_variable_symbol)

CV_REGISTER_VARIABLE_SYMBOL_STRUCT

void cv_register_variable_symbol_print(struct cv_register_variable_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV constant symbol */

#define CV_CONSTANT_SYMBOL_STRUCT \
STRUCT_DECL(cv_constant_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_PRIMITIVE(uint64_t, value, "%llu") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_constant_symbol)

CV_CONSTANT_SYMBOL_STRUCT

void cv_constant_symbol_dispose(struct cv_constant_symbol *item);
void cv_constant_symbol_print(struct cv_constant_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV user defined type symbol */

#define CV_USER_DEFINED_TYPE_SYMBOL_STRUCT \
STRUCT_DECL(cv_user_defined_type_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_user_defined_type_symbol)

CV_USER_DEFINED_TYPE_SYMBOL_STRUCT

void cv_user_defined_type_symbol_dispose(struct cv_user_defined_type_symbol *item);
void cv_user_defined_type_symbol_print(struct cv_user_defined_type_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV multi register variable symbol */

#define CV_MULTI_REGISTER_ENTRY_STRUCT \
STRUCT_DECL(cv_multi_register_entry) \
    FIELD_PRIMITIVE(uint16_t, register_index, "%u") \
    FIELD_PRIMITIVE(char *, register_name, "\"%s\"") \
STRUCT_END(cv_multi_register_entry)

CV_MULTI_REGISTER_ENTRY_STRUCT

void cv_multi_register_entry_dispose(struct cv_multi_register_entry *entry);
void cv_multi_register_entry_print(struct cv_multi_register_entry *entry, uint32_t depth, FILE *stream);

#define CV_MULTI_REGISTER_VARIABLE_SYMBOL_STRUCT \
STRUCT_DECL(cv_multi_register_variable_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, register_count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct cv_multi_register_entry *, registers, register_count, cv_multi_register_entry_print) \
STRUCT_END(cv_multi_register_variable_symbol)

CV_MULTI_REGISTER_VARIABLE_SYMBOL_STRUCT

void cv_multi_register_variable_symbol_dispose(struct cv_multi_register_variable_symbol *item);
void cv_multi_register_variable_symbol_print(struct cv_multi_register_variable_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV data symbol */

#define CV_DATA_SYMBOL_STRUCT \
STRUCT_DECL(cv_data_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, code_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_data_symbol)

CV_DATA_SYMBOL_STRUCT

void cv_data_symbol_dispose(struct cv_data_symbol *item);
void cv_data_symbol_print(struct cv_data_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV public symbol */

#define CV_PUBLIC_SYMBOL_FLAGS_ENUM \
ENUM_DECL(cv_public_symbol_flags) \
    ENUM_VALUE(CVPSF_CODE, 1 << 0) \
    ENUM_VALUE(CVPSF_FUNCTION, 1 << 1) \
    ENUM_VALUE(CVPSF_MANAGED, 1 << 2) \
    ENUM_VALUE(CVPSF_MSIL, 1 << 3) \
ENUM_END(cv_public_symbol_flags)

CV_PUBLIC_SYMBOL_FLAGS_ENUM

#define CV_PUBLIC_SYMBOL_STRUCT \
STRUCT_DECL(cv_public_symbol) \
    FIELD_PRIMITIVE(uint32_t, flags, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, code_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_public_symbol)

CV_PUBLIC_SYMBOL_STRUCT

void cv_public_symbol_dispose(struct cv_public_symbol *item);
void cv_public_symbol_print(struct cv_public_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV procedure flags */

#define CV_PROCEDURE_FLAGS_ENUM \
ENUM_DECL(cv_procedure_flags) \
    ENUM_VALUE(CV_PFLAG_NOFPO, 0x01) \
    ENUM_VALUE(CV_PFLAG_INT, 0x02) \
    ENUM_VALUE(CV_PFLAG_FAR, 0x04) \
    ENUM_VALUE(CV_PFLAG_NEVER, 0x08) \
    ENUM_VALUE(CV_PFLAG_NOTREACHED, 0x10) \
    ENUM_VALUE(CV_PFLAG_CUST_CALL, 0x20) \
    ENUM_VALUE(CV_PFLAG_NOINLINE, 0x40) \
    ENUM_VALUE(CV_PFLAG_OPTDBGINFO, 0x80) \
ENUM_END(cv_procedure_flags)

CV_PROCEDURE_FLAGS_ENUM

/* ---------- CV procedure symbol */

#define CV_PROCEDURE_SYMBOL_STRUCT \
STRUCT_DECL(cv_procedure_symbol) \
    FIELD_PRIMITIVE(uint32_t, parent_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, end_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, next_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, code_block_length, "%u") \
    FIELD_PRIMITIVE(uint32_t, debug_start_offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, debug_end_offset, "%u") \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, section_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(uint8_t, flags, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_procedure_symbol)

CV_PROCEDURE_SYMBOL_STRUCT

void cv_procedure_symbol_dispose(struct cv_procedure_symbol *item);
void cv_procedure_symbol_print(struct cv_procedure_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV thread storage symbol */

#define CV_THREAD_STORAGE_SYMBOL_STRUCT \
STRUCT_DECL(cv_thread_storage_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, section_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_thread_storage_symbol)

CV_THREAD_STORAGE_SYMBOL_STRUCT

void cv_thread_storage_symbol_dispose(struct cv_thread_storage_symbol *item);
void cv_thread_storage_symbol_print(struct cv_thread_storage_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV source language */

#define CV_SOURCE_LANGUAGE_ENUM \
ENUM_DECL(cv_source_language) \
    ENUM_VALUE(CV_CFL_C, 0x00) \
    ENUM_VALUE(CV_CFL_CXX, 0x01) \
    ENUM_VALUE(CV_CFL_FORTRAN, 0x02) \
    ENUM_VALUE(CV_CFL_MASM, 0x03) \
    ENUM_VALUE(CV_CFL_PASCAL, 0x04) \
    ENUM_VALUE(CV_CFL_BASIC, 0x05) \
    ENUM_VALUE(CV_CFL_COBOL, 0x06) \
    ENUM_VALUE(CV_CFL_LINK, 0x07) \
    ENUM_VALUE(CV_CFL_CVTRES, 0x08) \
    ENUM_VALUE(CV_CFL_CVTPGD, 0x09) \
    ENUM_VALUE(CV_CFL_CSHARP, 0x0A) \
    ENUM_VALUE(CV_CFL_VB, 0x0B) \
    ENUM_VALUE(CV_CFL_ILASM, 0x0C) \
    ENUM_VALUE(CV_CFL_JAVA, 0x0D) \
    ENUM_VALUE(CV_CFL_JSCRIPT, 0x0E) \
    ENUM_VALUE(CV_CFL_MSIL, 0x0F) \
    ENUM_VALUE(CV_CFL_HLSL, 0x10) \
ENUM_END(cv_source_language)

CV_SOURCE_LANGUAGE_ENUM

void cv_source_language_print(enum cv_source_language language, FILE *stream);

/* ---------- CV compiler version */

#define CV_COMPILER_VERSION_STRUCT \
STRUCT_DECL(cv_compiler_version) \
    FIELD_PRIMITIVE(uint16_t, major, "%u") \
    FIELD_PRIMITIVE(uint16_t, minor, "%u") \
    FIELD_PRIMITIVE(uint16_t, build, "%u") \
    FIELD_PRIMITIVE(uint16_t, qfe, "%u") \
STRUCT_END(cv_compiler_version)

CV_COMPILER_VERSION_STRUCT

void cv_compiler_version_print(struct cv_compiler_version *item, uint32_t depth, FILE *stream);

/* ---------- CV compile flags */

#define CV_COMPILE_FLAGS_STRUCT \
STRUCT_DECL(cv_compile_flags) \
    FIELD_PRIMITIVE_BITS(uint8_t, pcode, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, floatprec, 2, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, floatpkg, 2, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, ambdata, 3, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, ambcode, 3, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, mode32, 1, "%u") \
    FIELD_PRIMITIVE_BITS(uint8_t, pad, 4, "%u") \
STRUCT_END(cv_compile_flags)

CV_COMPILE_FLAGS_STRUCT
static_assert(sizeof(struct cv_compile_flags) == 2);

void cv_compile_flags_print(struct cv_compile_flags *item, uint32_t depth, FILE *stream);

/* ---------- CV compile flags symbol */

#define CV_COMPILE_FLAGS_SYMBOL_STRUCT \
STRUCT_DECL(cv_compile_flags_symbol) \
    FIELD_PRIMITIVE(uint8_t, language, "%u") \
    FIELD_STRUCT(struct cv_compile_flags, flags, cv_compile_flags_print) \
    FIELD_PRIMITIVE(uint16_t, cpu_type, "%u") \
    FIELD_STRUCT(struct cv_compiler_version, frontend_version, cv_compiler_version_print) \
    FIELD_STRUCT(struct cv_compiler_version, backend_version, cv_compiler_version_print) \
    FIELD_PRIMITIVE(char *, version_string, "\"%s\"") \
STRUCT_END(cv_compile_flags_symbol)

CV_COMPILE_FLAGS_SYMBOL_STRUCT

void cv_compile_flags_symbol_dispose(struct cv_compile_flags_symbol *item);
void cv_compile_flags_symbol_print(struct cv_compile_flags_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV using namespace symbol */

#define CV_USING_NAMESPACE_SYMBOL_STRUCT \
STRUCT_DECL(cv_using_namespace_symbol) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_using_namespace_symbol)

CV_USING_NAMESPACE_SYMBOL_STRUCT

void cv_using_namespace_symbol_dispose(struct cv_using_namespace_symbol *item);
void cv_using_namespace_symbol_print(struct cv_using_namespace_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV procedure reference symbol */

#define CV_PROCEDURE_REFERENCE_SYMBOL_STRUCT \
STRUCT_DECL(cv_procedure_reference_symbol) \
    FIELD_PRIMITIVE(uint32_t, sum_name, "%u") \
    FIELD_PRIMITIVE(uint32_t, symbol_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, module_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_procedure_reference_symbol)

CV_PROCEDURE_REFERENCE_SYMBOL_STRUCT

void cv_procedure_reference_symbol_dispose(struct cv_procedure_reference_symbol *item);
void cv_procedure_reference_symbol_print(struct cv_procedure_reference_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV data reference symbol */

#define CV_DATA_REFERENCE_SYMBOL_STRUCT \
STRUCT_DECL(cv_data_reference_symbol) \
    FIELD_PRIMITIVE(uint32_t, sum_name, "%u") \
    FIELD_PRIMITIVE(uint32_t, symbol_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, module_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_data_reference_symbol)

CV_DATA_REFERENCE_SYMBOL_STRUCT

void cv_data_reference_symbol_dispose(struct cv_data_reference_symbol *item);
void cv_data_reference_symbol_print(struct cv_data_reference_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV annotation reference symbol */

#define CV_ANNOTATION_REFERENCE_SYMBOL_STRUCT \
STRUCT_DECL(cv_annotation_reference_symbol) \
    FIELD_PRIMITIVE(uint32_t, sum_name, "%u") \
    FIELD_PRIMITIVE(uint32_t, symbol_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, module_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_annotation_reference_symbol)

CV_ANNOTATION_REFERENCE_SYMBOL_STRUCT

void cv_annotation_reference_symbol_dispose(struct cv_annotation_reference_symbol *item);
void cv_annotation_reference_symbol_print(struct cv_annotation_reference_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV trampoline symbol */

#define CV_TRAMPOLINE_TYPE_ENUM \
ENUM_DECL(cv_trampoline_type) \
    ENUM_VALUE(CV_TRAMPOLINE_TYPE_INCREMENTAL) \
    ENUM_VALUE(CV_TRAMPOLINE_TYPE_BRANCH_ISLAND) \
ENUM_END(cv_trampoline_type)

CV_TRAMPOLINE_TYPE_ENUM

void cv_trampoline_type_print(enum cv_trampoline_type type, FILE *stream);

#define CV_TRAMPOLINE_SYMBOL_STRUCT \
STRUCT_DECL(cv_trampoline_symbol) \
    FIELD_PRIMITIVE(uint16_t, type, "%u") \
    FIELD_PRIMITIVE(uint16_t, size, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, thunk_offset, cv_pe_section_offset_print) \
    FIELD_STRUCT(struct cv_pe_section_offset, target_offset, cv_pe_section_offset_print) \
STRUCT_END(cv_trampoline_symbol)

CV_TRAMPOLINE_SYMBOL_STRUCT

void cv_trampoline_symbol_print(struct cv_trampoline_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV export symbol */

#define CV_EXPORT_SYMBOL_STRUCT \
STRUCT_DECL(cv_export_symbol) \
    FIELD_PRIMITIVE(uint16_t, ordinal, "%u") \
    FIELD_PRIMITIVE(uint16_t, flags, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_export_symbol)

CV_EXPORT_SYMBOL_STRUCT

void cv_export_symbol_dispose(struct cv_export_symbol *item);
void cv_export_symbol_print(struct cv_export_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV local symbol */

#define CV_LOCAL_SYMBOL_STRUCT \
STRUCT_DECL(cv_local_symbol) \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, flags, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_local_symbol)

CV_LOCAL_SYMBOL_STRUCT

void cv_local_symbol_dispose(struct cv_local_symbol *item);
void cv_local_symbol_print(struct cv_local_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV build info symbol */

#define CV_BUILD_INFO_SYMBOL_STRUCT \
STRUCT_DECL(cv_build_info_symbol) \
    FIELD_PRIMITIVE(uint32_t, id_index, "%u") \
STRUCT_END(cv_build_info_symbol)

CV_BUILD_INFO_SYMBOL_STRUCT

void cv_build_info_symbol_print(struct cv_build_info_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV annotations */

#define CV_ANNOTATION_TYPE_ENUM \
ENUM_DECL(cv_annotation_type) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CODE_OFFSET) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_CODE_OFFSET_BASE) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_CODE_OFFSET) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_CODE_LENGTH) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_FILE) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_LINE_OFFSET) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_LINE_END_DELTA) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_RANGE_KIND) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_COLUMN_START) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_COLUMN_END_DELTA) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_CODE_AND_LINE_OFFSETS) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_CODE_LENGTH_AND_CODE_OFFSET) \
    ENUM_VALUE(CV_ANNOTATION_TYPE_CHANGE_COLUMN_END) \
ENUM_END(cv_annotation_type)

CV_ANNOTATION_TYPE_ENUM

void cv_annotation_type_print(enum cv_annotation_type type, FILE *stream);

#define CV_ANNOTATION_CODE_AND_LINE_OFFSETS_STRUCT \
STRUCT_DECL(cv_annotation_code_and_line_offsets) \
    FIELD_PRIMITIVE(uint32_t, code_offset, "%u") \
    FIELD_PRIMITIVE(int32_t, line_offset, "%u") \
STRUCT_END(cv_annotation_code_and_line_offsets)

CV_ANNOTATION_CODE_AND_LINE_OFFSETS_STRUCT

void cv_annotation_code_and_line_offsets_print(struct cv_annotation_code_and_line_offsets *item, uint32_t depth, FILE *stream);

#define CV_ANNOTATION_CODE_LENGTH_AND_OFFSET_STRUCT \
STRUCT_DECL(cv_annotation_code_length_and_offset) \
    FIELD_PRIMITIVE(uint32_t, code_length, "%u") \
    FIELD_PRIMITIVE(uint32_t, code_offset, "%u") \
STRUCT_END(cv_annotation_code_length_and_offset)

CV_ANNOTATION_CODE_LENGTH_AND_OFFSET_STRUCT

void cv_annotation_code_length_and_offset_print(struct cv_annotation_code_length_and_offset *item, uint32_t depth, FILE *stream);

#define CV_ANNOTATION_STRUCT \
STRUCT_DECL(cv_annotation) \
    FIELD_PRIMITIVE_FMT(enum cv_annotation_type, type, cv_annotation_type_print) \
    FIELD_UNION_DECL() \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, code_offset, type, CV_ANNOTATION_TYPE_CODE_OFFSET, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_code_offset_base, type, CV_ANNOTATION_TYPE_CHANGE_CODE_OFFSET_BASE, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_code_offset, type, CV_ANNOTATION_TYPE_CHANGE_CODE_OFFSET, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_code_length, type, CV_ANNOTATION_TYPE_CHANGE_CODE_LENGTH, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_file, type, CV_ANNOTATION_TYPE_CHANGE_FILE, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(int32_t, change_line_offset, type, CV_ANNOTATION_TYPE_CHANGE_LINE_OFFSET, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_line_end_delta, type, CV_ANNOTATION_TYPE_CHANGE_LINE_END_DELTA, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_range_kind, type, CV_ANNOTATION_TYPE_CHANGE_RANGE_KIND, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_column_start, type, CV_ANNOTATION_TYPE_CHANGE_COLUMN_START, "%u") \
        FIELD_UNION_FIELD_PRIMITIVE(int32_t, change_column_end_delta, type, CV_ANNOTATION_TYPE_CHANGE_COLUMN_END_DELTA, "%u") \
        FIELD_UNION_FIELD_STRUCT(struct cv_annotation_code_and_line_offsets, change_code_and_line_offsets, type, CV_ANNOTATION_TYPE_CHANGE_CODE_AND_LINE_OFFSETS, cv_annotation_code_and_line_offsets_print) \
        FIELD_UNION_FIELD_STRUCT(struct cv_annotation_code_length_and_offset, change_code_length_and_code_offset, type, CV_ANNOTATION_TYPE_CHANGE_CODE_LENGTH_AND_CODE_OFFSET, cv_annotation_code_length_and_offset_print) \
        FIELD_UNION_FIELD_PRIMITIVE(uint32_t, change_column_end, type, CV_ANNOTATION_TYPE_CHANGE_COLUMN_END, "%u") \
    FIELD_UNION_END() \
STRUCT_END(cv_annotation)

CV_ANNOTATION_STRUCT

void cv_annotation_print(struct cv_annotation *item, uint32_t depth, FILE *stream);

#define CV_ANNOTATIONS_STRUCT \
STRUCT_DECL(cv_annotations) \
    FIELD_PRIMITIVE(uint32_t, count, "%u") \
    FIELD_STRUCT_DYNAMIC_ARRAY(struct cv_annotation *, annotations, count, cv_annotation_print) \
STRUCT_END(cv_annotations)

CV_ANNOTATIONS_STRUCT

void cv_annotations_dispose(struct cv_annotations *item);
void cv_annotations_print(struct cv_annotations *item, uint32_t depth, FILE *stream);

/* ---------- CV inline site symbol */

#define CV_INLINE_SITE_SYMBOL_STRUCT \
STRUCT_DECL(cv_inline_site_symbol) \
    FIELD_PRIMITIVE(uint32_t, parent_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, end_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, inlinee_id_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, invocation_count, "%u") \
    FIELD_STRUCT(struct cv_annotations, annotations, cv_annotations_print) \
STRUCT_END(cv_inline_site_symbol)

CV_INLINE_SITE_SYMBOL_STRUCT

void cv_inline_site_symbol_dispose(struct cv_inline_site_symbol *item);
void cv_inline_site_symbol_print(struct cv_inline_site_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV label symbol */

#define CV_LABEL_SYMBOL_STRUCT \
STRUCT_DECL(cv_label_symbol) \
    FIELD_STRUCT(struct cv_pe_section_offset, section_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(uint8_t, flags, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_label_symbol)

CV_LABEL_SYMBOL_STRUCT

void cv_label_symbol_dispose(struct cv_label_symbol *item);
void cv_label_symbol_print(struct cv_label_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV block symbol */

#define CV_BLOCK_SYMBOL_STRUCT \
STRUCT_DECL(cv_block_symbol) \
    FIELD_PRIMITIVE(uint32_t, parent_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, end_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, length, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, section_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_block_symbol)

CV_BLOCK_SYMBOL_STRUCT

void cv_block_symbol_dispose(struct cv_block_symbol *item);
void cv_block_symbol_print(struct cv_block_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV register relative symbol */

#define CV_REGISTER_RELATIVE_SYMBOL_STRUCT \
STRUCT_DECL(cv_register_relative_symbol) \
    FIELD_PRIMITIVE(int32_t, offset, "%i") \
    FIELD_PRIMITIVE(uint32_t, type_index, "%u") \
    FIELD_PRIMITIVE(uint16_t, register_index, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_register_relative_symbol)

CV_REGISTER_RELATIVE_SYMBOL_STRUCT

void cv_register_relative_symbol_dispose(struct cv_register_relative_symbol *item);
void cv_register_relative_symbol_print(struct cv_register_relative_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV thunk symbol */

#define CV_THUNK_SYMBOL_STRUCT \
STRUCT_DECL(cv_thunk_symbol) \
    FIELD_PRIMITIVE(uint32_t, parent_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, end_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, next_symbol_index, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, section_offset, cv_pe_section_offset_print) \
    FIELD_PRIMITIVE(uint16_t, length, "%u") \
    FIELD_PRIMITIVE(uint8_t, ordinal, "%u") \
    FIELD_PRIMITIVE(char *, name, "\"%s\"") \
STRUCT_END(cv_thunk_symbol)

CV_THUNK_SYMBOL_STRUCT

void cv_thunk_symbol_dispose(struct cv_thunk_symbol *item);
void cv_thunk_symbol_print(struct cv_thunk_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV separated code symbol */

#define CV_SEPARATED_CODE_SYMBOL_STRUCT \
STRUCT_DECL(cv_separated_code_symbol) \
    FIELD_PRIMITIVE(uint32_t, parent_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, end_symbol_index, "%u") \
    FIELD_PRIMITIVE(uint32_t, length, "%u") \
    FIELD_PRIMITIVE(uint32_t, flags, "%u") \
    FIELD_STRUCT(struct cv_pe_section_offset, section_offset, cv_pe_section_offset_print) \
    FIELD_STRUCT(struct cv_pe_section_offset, parent_section_offset, cv_pe_section_offset_print) \
STRUCT_END(cv_separated_code_symbol)

CV_SEPARATED_CODE_SYMBOL_STRUCT

void cv_separated_code_symbol_print(struct cv_separated_code_symbol *item, uint32_t depth, FILE *stream);

/* ---------- CV symbol */

#define CV_SYMBOL_STRUCT \
STRUCT_DECL(cv_symbol) \
    FIELD_PRIMITIVE_FMT(enum cv_symbol_type, type, cv_symbol_type_print) \
    FIELD_UNION_DECL() \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_obj_name_symbol, obj_name_symbol, type, cv_obj_name_symbol_print, S_OBJNAME, S_OBJNAME_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_register_variable_symbol, register_variable_symbol, type, cv_register_variable_symbol_print, S_REGISTER, S_REGISTER_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_constant_symbol, constant_symbol, type, cv_constant_symbol_print, S_CONSTANT, S_CONSTANT_ST, S_MANCONSTANT) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_user_defined_type_symbol, user_defined_type_symbol, type, cv_user_defined_type_symbol_print, S_UDT, S_UDT_ST, S_COBOLUDT, S_COBOLUDT_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_multi_register_variable_symbol, multi_register_variable_symbol, type, cv_multi_register_variable_symbol_print, S_MANYREG, S_MANYREG_ST, S_MANYREG2, S_MANYREG2_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_data_symbol, data_symbol, type, cv_data_symbol_print, S_LDATA32, S_LDATA32_ST, S_GDATA32, S_GDATA32_ST, S_LMANDATA, S_LMANDATA_ST, S_GMANDATA, S_GMANDATA_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_public_symbol, public_symbol, type, cv_public_symbol_print, S_PUB32, S_PUB32_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_procedure_symbol, procedure_symbol, type, cv_procedure_symbol_print, S_LPROC32, S_LPROC32_ST, S_GPROC32, S_GPROC32_ST, S_LPROC32_ID, S_GPROC32_ID, S_LPROC32_DPC, S_LPROC32_DPC_ID) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_thread_storage_symbol, thread_storage_symbol, type, cv_thread_storage_symbol_print, S_LTHREAD32, S_LTHREAD32_ST, S_GTHREAD32, S_GTHREAD32_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_compile_flags_symbol, compile_flags_symbol, type, cv_compile_flags_symbol_print, S_COMPILE2, S_COMPILE2_ST, S_COMPILE3) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_using_namespace_symbol, using_namespace_symbol, type, cv_using_namespace_symbol_print, S_UNAMESPACE, S_UNAMESPACE_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_procedure_reference_symbol, procedure_reference_symbol, type, cv_procedure_reference_symbol_print, S_PROCREF, S_PROCREF_ST, S_LPROCREF, S_LPROCREF_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_data_reference_symbol, data_reference_symbol, type, cv_data_reference_symbol_print, S_DATAREF, S_DATAREF_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_annotation_reference_symbol, annotation_reference_symbol, type, cv_annotation_reference_symbol_print, S_ANNOTATIONREF) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_trampoline_symbol, trampoline_symbol, type, cv_trampoline_symbol_print, S_TRAMPOLINE) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_export_symbol, export_symbol, type, cv_export_symbol_print, S_EXPORT) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_local_symbol, local_symbol, type, cv_local_symbol_print, S_LOCAL) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_build_info_symbol, build_info_symbol, type, cv_build_info_symbol_print, S_BUILDINFO) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_inline_site_symbol, inline_site_symbol, type, cv_inline_site_symbol_print, S_INLINESITE, S_INLINESITE2) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_label_symbol, label_symbol, type, cv_label_symbol_print, S_LABEL32, S_LABEL32_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_block_symbol, block_symbol, type, cv_block_symbol_print, S_BLOCK32, S_BLOCK32_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_register_relative_symbol, register_relative_symbol, type, cv_register_relative_symbol_print, S_REGREL32) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_thunk_symbol, thunk_symbol, type, cv_thunk_symbol_print, S_THUNK32, S_THUNK32_ST) \
        FIELD_UNION_FIELD_STRUCT_MULTITAG(struct cv_separated_code_symbol, separated_code_symbol, type, cv_separated_code_symbol_print, S_SEPCODE) \
    FIELD_UNION_END() \
STRUCT_END(cv_symbol)

CV_SYMBOL_STRUCT

void cv_symbol_dispose(struct cv_symbol *item);
void cv_symbol_print(struct cv_symbol *item, uint32_t depth, FILE *stream);
