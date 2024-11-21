
/* ---------- struct macros */

#undef STRUCT_DECL
#undef FIELD_PRIMITIVE
#undef FIELD_PRIMITIVE_FMT
#undef FIELD_PRIMITIVE_FIXED_ARRAY
#undef FIELD_PRIMITIVE_FIXED_ARRAY_FMT
#undef FIELD_PRIMITIVE_DYNAMIC_ARRAY
#undef FIELD_PRIMITIVE_DYNAMIC_ARRAY_FMT
#undef FIELD_STRUCT
#undef FIELD_STRUCT_FIXED_ARRAY
#undef FIELD_STRUCT_DYNAMIC_ARRAY
#undef FIELD_UNION_DECL
#undef FIELD_UNION_FIELD_STRUCT
#undef FIELD_UNION_END
#undef STRUCT_END

#define STRUCT_DECL(name) struct name {

#define FIELD_PRIMITIVE(type, name, fmt) type name;
#define FIELD_PRIMITIVE_FMT FIELD_PRIMITIVE

#define FIELD_PRIMITIVE_FIXED_ARRAY(type, name, count, fmt) type name[count];
#define FIELD_PRIMITIVE_FIXED_ARRAY_FMT FIELD_PRIMITIVE_FIXED_ARRAY

#define FIELD_PRIMITIVE_DYNAMIC_ARRAY(type, name, count_field, fmt) type name;
#define FIELD_PRIMITIVE_DYNAMIC_ARRAY_FMT FIELD_PRIMITIVE_DYNAMIC_ARRAY

#define FIELD_STRUCT(type, name, fmt_fn) type name;
#define FIELD_STRUCT_FIXED_ARRAY(type, name, count, fmt_fn) type name[count];
#define FIELD_STRUCT_DYNAMIC_ARRAY(type, name, count_field, fmt_fn) type name;

#define FIELD_UNION_DECL() union {
#define FIELD_UNION_FIELD_STRUCT(type, name, tag_field, tag, fmt_fn) type name;
#define FIELD_UNION_END() };

#define STRUCT_END(name) };

/* ---------- enum macros */

#undef ENUM_DECL
#undef ENUM_VALUE
#undef ENUM_END

#define ENUM_DECL(name) enum name {
#define ENUM_VALUE(name, ...) name __VA_OPT__(=) __VA_ARGS__,
#define ENUM_END(name) };
