#pragma once
#include <stdint.h>
#include "tpi.h"
#include "pdb.h"

/* ---------- labels */

struct cpp_label
{
    uint64_t address;
    char *name;
};

void cpp_label_dispose(struct cpp_label *label);

/* ---------- variables */

struct cpp_variable
{
    char *signature;
    char *value;
    char *comment;
};

void cpp_variable_dispose(struct cpp_variable *variable);

/* ---------- code blocks */

struct cpp_block
{
    uint64_t address;
    uint32_t statement_count;
    struct cpp_statement *statements;
};

void cpp_block_dispose(struct cpp_block *block);

/* ---------- statements */

enum cpp_statement_type
{
    CPP_STATEMENT_TYPE_COMMENT,
    CPP_STATEMENT_TYPE_LABEL,
    CPP_STATEMENT_TYPE_VARIABLE,
    CPP_STATEMENT_TYPE_BLOCK,
};

struct cpp_statement
{
    enum cpp_statement_type type;
    union
    {
        char *comment;
        struct cpp_label label;
        struct cpp_variable variable;
        struct cpp_block block;
    };
};

void cpp_statement_dispose(struct cpp_statement *statement);

/* ---------- procedures */

struct cpp_procedure
{
    uint64_t address;
    uint32_t line;
    uint32_t type_index;
    char *signature;
    struct cpp_block *body;
};

void cpp_procedure_dispose(struct cpp_procedure *procedure);

/* ---------- typedefs */

struct cpp_typedef
{
    char *type_name;
    
    uint32_t underlying_type_index;
    uint32_t containing_class_type_index;

    struct tpi_field_attributes field_attributes;
    struct tpi_pointer_attributes pointer_attributes;
};

void cpp_typedef_dispose(struct cpp_typedef *item);

/* ---------- enum values */

struct cpp_enum_value
{
    char *name;
    struct tpi_enumerate_variant variant;
};

void cpp_enum_value_dispose(struct cpp_enum_value *item);

/* ---------- enums */

enum cpp_enum_flags
{
    CPP_ENUM_IS_DECLARATION = 1 << 0,
};

struct cpp_enum
{
    uint32_t type_index;
    uint32_t flags;
    uint32_t depth;
    uint32_t line;

    char *name;
    char *underlying_type_name;

    uint32_t value_count;
    struct cpp_enum_value *values;

    struct tpi_field_attributes field_attributes;
};

void cpp_enum_dispose(struct cpp_enum *item);

/* ---------- fields */

struct cpp_field
{
    char *type_name;
    char *name;

    uint64_t offset;

    struct tpi_field_attributes attributes;
};

void cpp_field_dispose(struct cpp_field *field);

/* ---------- methods */

struct cpp_method
{
    char *name;
    char *return_type_name;
    
    uint32_t argument_count;
    char **arguments;

    struct tpi_field_attributes field_attributes;
    struct tpi_procedure_attributes procedure_attributes;
};

void cpp_method_dispose(struct cpp_method *method);

/* ---------- base classes */

struct cpp_base_class
{
    char *type_name;
    uint32_t type_index;
    uint32_t offset;
};

void cpp_base_class_dispose(struct cpp_base_class *item);

/* ---------- classes */

enum cpp_class_type
{
    CPP_CLASS_TYPE_CLASS,
    CPP_CLASS_TYPE_STRUCT,
    CPP_CLASS_TYPE_INTERFACE,
};

enum cpp_class_flags
{
    CPP_CLASS_IS_UNION = 1 << 0,
    CPP_CLASS_IS_DECLARATION = 1 << 1,
};

struct cpp_class
{
    enum cpp_class_type type;
    
    char *name;

    uint32_t flags;
    uint32_t type_index;
    uint32_t depth;
    uint32_t line;
    uint64_t size;

    uint32_t base_class_count;
    struct cpp_base_class *base_classes;

    uint32_t member_count;
    struct cpp_class_member *members;

    struct tpi_field_attributes field_attributes;
};

void cpp_class_dispose(struct cpp_class *item);

/* ---------- class members */

enum cpp_class_member_type
{
    CPP_CLASS_MEMBER_TYPE_CLASS,
    CPP_CLASS_MEMBER_TYPE_ENUM,
    CPP_CLASS_MEMBER_TYPE_FIELD,
    CPP_CLASS_MEMBER_TYPE_METHOD,
    CPP_CLASS_MEMBER_TYPE_TYPEDEF,
};

struct cpp_class_member
{
    enum cpp_class_member_type type;
    union
    {
        struct cpp_class class_;
        struct cpp_enum enum_;
        struct cpp_field field;
        struct cpp_method method;
        struct cpp_typedef typedef_;
    };
};

void cpp_class_member_dispose(struct cpp_class_member *member);

/* ---------- headers */

enum cpp_header_flags
{
    CPP_HEADER_IS_LOCAL = 1 << 0,
};

struct cpp_header
{
    uint32_t flags;
    char *path;
};

void cpp_header_dispose(struct cpp_header *header);

/* ---------- module members */

enum cpp_module_member_type
{
    CPP_MODULE_MEMBER_TYPE_CLASS,
    CPP_MODULE_MEMBER_TYPE_ENUM,
    CPP_MODULE_MEMBER_TYPE_USER_DEFINED_TYPE,
    CPP_MODULE_MEMBER_TYPE_USING_NAMESPACE,
    CPP_MODULE_MEMBER_TYPE_CONSTANT,
    CPP_MODULE_MEMBER_TYPE_DATA,
    CPP_MODULE_MEMBER_TYPE_THREAD_STORAGE,
    CPP_MODULE_MEMBER_TYPE_PROCEDURE,
};

struct cpp_module_member
{
    enum cpp_module_member_type type;
    union
    {
        struct cpp_class class_;
        struct cpp_enum enum_;
        char *user_defined_type;
        char *using_namespace;
        char *constant;
        char *data;
        char *thread_storage;
        struct cpp_procedure procedure;
    };
};

void cpp_module_member_dispose(struct cpp_module_member *member);

/* ---------- modules */

struct cpp_module
{
    char *path;

    uint32_t header_count;
    struct cpp_header *headers;

    uint32_t member_count;
    struct cpp_module_member *members;
};

void cpp_module_dispose(struct cpp_module *module);
void cpp_module_add_type_definition(struct cpp_module *module, struct pdb_data *pdb, uint32_t type_index, uint32_t line);
