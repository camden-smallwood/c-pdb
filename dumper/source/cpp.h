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
void cpp_label_print(struct cpp_label *item, uint32_t depth, FILE *stream);

/* ---------- variables */

struct cpp_variable
{
    char *signature;
    char *value;
    char *comment;
};

void cpp_variable_dispose(struct cpp_variable *variable);
void cpp_variable_print(struct cpp_variable *item, uint32_t depth, FILE *stream);

/* ---------- code blocks */

struct cpp_block
{
    uint64_t address;
    uint32_t statement_count;
    struct cpp_statement *statements;
};

void cpp_block_dispose(struct cpp_block *block);
void cpp_block_print(struct cpp_block *item, uint32_t depth, FILE *stream);

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
void cpp_statement_print(struct cpp_statement *item, uint32_t depth, FILE *stream);

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
void cpp_procedure_print(struct cpp_procedure *item, uint32_t depth, FILE *stream);

/* ---------- typedefs */

struct cpp_typedef
{
    char *type_name;
};

void cpp_typedef_dispose(struct cpp_typedef *item);
void cpp_typedef_print(struct cpp_typedef *item, uint32_t depth, FILE *stream);

/* ---------- enum values */

struct cpp_enum_value
{
    char *name;
    struct tpi_enumerate_variant variant;
};

void cpp_enum_value_dispose(struct cpp_enum_value *item);
void cpp_enum_value_print(struct cpp_enum_value *item, FILE *stream);

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
void cpp_enum_print(struct cpp_enum *item, uint32_t depth, FILE *stream);
void cpp_enum_add_members(struct cpp_enum *item, struct pdb_data *pdb, uint32_t members_type_index);

/* ---------- fields */

struct cpp_field
{
    char *type_name;
    char *name;

    uint64_t offset;

    struct tpi_field_attributes attributes;
};

void cpp_field_dispose(struct cpp_field *field);
void cpp_field_print(struct cpp_field *field, uint32_t depth, FILE *stream);

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
void cpp_method_print(struct cpp_method *method, uint32_t depth, FILE *stream);

/* ---------- base classes */

struct cpp_base_class
{
    char *type_name;
    uint32_t type_index;
    uint32_t offset;
};

void cpp_base_class_dispose(struct cpp_base_class *item);
void cpp_base_class_print(struct cpp_base_class *item, uint32_t depth, FILE *stream);

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
    uint32_t line;
    uint64_t size;

    uint32_t base_class_count;
    struct cpp_base_class *base_classes;

    uint32_t member_count;
    struct cpp_class_member *members;

    struct tpi_field_attributes field_attributes;
};

void cpp_class_dispose(struct cpp_class *item);
void cpp_class_print(struct cpp_class *item, uint32_t depth, FILE *stream);
void cpp_class_add_members(struct cpp_class *item, struct pdb_data *pdb, uint32_t members_type_index);

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
void cpp_class_member_print(struct cpp_class_member *member, uint32_t depth, FILE *stream);

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

/* ---------- data declarations */

struct cpp_data
{
    char *string;
    uint64_t address;
    uint32_t line;
};

void cpp_data_dispose(struct cpp_data *data);
void cpp_data_print(struct cpp_data *data, FILE *stream);

/* ---------- thread_local declarations */

struct cpp_thread_local
{
    char *string;
    uint64_t address;
    uint32_t line;
};

void cpp_thread_local_dispose(struct cpp_thread_local *data);
void cpp_thread_local_print(struct cpp_thread_local *data, FILE *stream);

/* ---------- module members */

enum cpp_module_member_type
{
    CPP_MODULE_MEMBER_TYPE_CLASS,
    CPP_MODULE_MEMBER_TYPE_ENUM,
    CPP_MODULE_MEMBER_TYPE_TYPEDEF,
    CPP_MODULE_MEMBER_TYPE_USING_NAMESPACE,
    CPP_MODULE_MEMBER_TYPE_CONSTANT,
    CPP_MODULE_MEMBER_TYPE_DATA,
    CPP_MODULE_MEMBER_TYPE_THREAD_LOCAL,
    CPP_MODULE_MEMBER_TYPE_PROCEDURE,
};

struct cpp_module_member
{
    enum cpp_module_member_type type;
    union
    {
        struct cpp_class class_;
        struct cpp_enum enum_;
        struct cpp_typedef typedef_;
        char *using_namespace;
        char *constant;
        struct cpp_data data;
        struct cpp_thread_local thread_local_;
        struct cpp_procedure procedure;
    };
};

void cpp_module_member_dispose(struct cpp_module_member *member);
void cpp_module_member_print(struct cpp_module_member *item, FILE *stream);

/* ---------- modules */

struct cpp_module
{
    char *path;
    char *compiler_path;
    char *pdb_path;
    char *args_string;

    uint32_t header_count;
    struct cpp_header *headers;

    uint32_t member_count;
    struct cpp_module_member *members;
};

void cpp_module_dispose(struct cpp_module *module);
void cpp_module_print(struct cpp_module *module, FILE *stream);
void cpp_module_add_type_definition(struct cpp_module *module, struct pdb_data *pdb, uint32_t type_index, uint32_t line);

/* ---------- type information */

char *cpp_primitive_name(struct pdb_data *pdb, struct tpi_primitive *primitive);
char *cpp_type_name(struct pdb_data *pdb, uint32_t type_index, char *declaration_name, uint32_t argument_count, char **arguments, int is_pointer);
uint64_t cpp_type_size(struct pdb_data *pdb, uint32_t type_index);
