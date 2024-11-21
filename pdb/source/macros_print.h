
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

#define STRUCT_DECL(name) \
    fprintf(stream, #name " {\n");

#define FIELD_PRIMITIVE(type, name, fmt) \
    fprintf_depth(stream, depth + 1, #name ": " fmt ",\n", item->name);

#define FIELD_PRIMITIVE_FMT(type, name, fmt_fn)   \
    fprintf_depth(stream, depth + 1, #name ": "); \
    fmt_fn(item->name, stream);                   \
    fprintf(stream, ",\n");

#define FIELD_PRIMITIVE_FIXED_ARRAY(type, name, count, fmt) \
    fprintf_depth(stream, depth + 1, #name ": [");          \
    for (uint32_t i = 0; i < count; i++)                    \
    {                                                       \
        if (i > 0)                                          \
            fprintf(stream, ", ");                          \
        fprintf(stream, fmt, item->name[i]);                \
    }                                                       \
    fprintf(stream, "],\n");

#define FIELD_PRIMITIVE_FIXED_ARRAY_FMT(type, name, count, fmt_fn) \
    fprintf_depth(stream, depth + 1, #name ": ");                  \
    fmt_fn(item->name, count, stream);                             \
    fprintf(stream, ",\n");

#define FIELD_PRIMITIVE_DYNAMIC_ARRAY(type, name, count_field, fmt) \
    fprintf_depth(stream, depth + 1, #name ": [");                  \
    for (uint32_t i = 0; i < item->count_field; i++)                \
    {                                                               \
        if (i > 0)                                                  \
            fprintf(stream, ", ");                                  \
        fprintf(stream, fmt, item->name[i]);                        \
    }                                                               \
    fprintf(stream, "],\n");

#define FIELD_PRIMITIVE_DYNAMIC_ARRAY_FMT(type, name, count_field, fmt) \
    fprintf_depth(stream, depth + 1, #name ": ");                       \
    fmt_fn(item->name, item->count_field, depth + 1, stream);

#define FIELD_STRUCT(type, name, fmt_fn)          \
    fprintf_depth(stream, depth + 1, #name ": "); \
    fmt_fn(&item->name, depth + 1, stream);       \
    fprintf(stream, ",\n");

#define FIELD_STRUCT_DYNAMIC_ARRAY(type, name, count_field, fmt_fn) \
    fprintf_depth(stream, depth + 1, #name ": [\n");                \
    for (uint32_t i = 0; i < item->count_field; i++)                \
    {                                                               \
        fprintf_depth(stream, depth + 2, "[%u] = ", i);             \
        fmt_fn(&item->name[i], depth + 2, stream);                  \
        fprintf(stream, ",\n");                                     \
    }                                                               \
    fprintf_depth(stream, depth + 1, "],\n");

#define FIELD_UNION_DECL()

#define FIELD_UNION_FIELD_STRUCT(type, name, tag_field, tag, fmt_fn) \
    if (item->tag_field == tag)                                      \
    {                                                                \
        fprintf_depth(stream, depth + 1, #name ": ");                \
        fmt_fn(&item->name, depth + 1, stream);                      \
        fprintf(stream, ",\n");                                      \
    }

#define FIELD_UNION_END()

#define STRUCT_END(name) \
    fprintf_depth(stream, depth, "}");

/* ---------- enum macros */

#undef ENUM_DECL
#undef ENUM_VALUE
#undef ENUM_END

#define ENUM_DECL(name)

#define ENUM_VALUE(name, ...)   \
    if (item == name)           \
    {                           \
        fprintf(stream, #name); \
        return;                 \
    }

#define ENUM_END(name)                                                                         \
    fprintf(stderr, "%s:%i: ERROR: Unhandled " #name "value: %i\n", __FILE__, __LINE__, item); \
    exit(EXIT_FAILURE);
