#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cpp.h"
#include "utils.h"

/* ---------- labels */

void cpp_label_dispose(
    struct cpp_label *label)
{
    assert(label);

    free(label->name);
}

/* ---------- variables */

void cpp_variable_dispose(
    struct cpp_variable *variable)
{
    assert(variable);

    free(variable->signature);
    free(variable->value);
    free(variable->comment);
}

/* ---------- code blocks */

void cpp_block_dispose(
    struct cpp_block *block)
{
    assert(block);

    for (uint32_t i = 0; i < block->statement_count; i++)
        cpp_statement_dispose(&block->statements[i]);
    
    free(block->statements);
}

/* ---------- statements */

void cpp_statement_dispose(
    struct cpp_statement *statement)
{
    assert(statement);

    switch (statement->type)
    {
    case CPP_STATEMENT_TYPE_COMMENT:
        free(statement->comment);
        break;

    case CPP_STATEMENT_TYPE_LABEL:
        cpp_label_dispose(&statement->label);
        break;

    case CPP_STATEMENT_TYPE_VARIABLE:
        cpp_variable_dispose(&statement->variable);
        break;

    case CPP_STATEMENT_TYPE_BLOCK:
        cpp_block_dispose(&statement->block);
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled cpp_statement_type value: %i\n", __FILE__, __LINE__, statement->type);
        exit(EXIT_FAILURE);
    }
}

/* ---------- procedures */

void cpp_procedure_dispose(
    struct cpp_procedure *procedure)
{
    assert(procedure);

    free(procedure->signature);

    if (procedure->body)
    {
        cpp_block_dispose(procedure->body);
        free(procedure->body);
    }
}

/* ---------- typedefs */

void cpp_typedef_dispose(
    struct cpp_typedef *item)
{
    assert(item);

    free(item->type_name);
}

/* ---------- enum values */

void cpp_enum_value_dispose(
    struct cpp_enum_value *item)
{
    assert(item);

    free(item->name);
}

/* ---------- enums */

void cpp_enum_dispose(
    struct cpp_enum *item)
{
    assert(item);

    free(item->name);
    free(item->underlying_type_name);

    for (uint32_t i = 0; i < item->value_count; i++)
        cpp_enum_value_dispose(&item->values[i]);
    
    free(item->values);
}

void cpp_enum_add_members(struct cpp_enum *item, struct pdb_data *pdb, uint32_t members_type_index)
{
    assert(item);
    assert(pdb);

    // TODO
    (void)members_type_index;
}

/* ---------- fields */

void cpp_field_dispose(
    struct cpp_field *field)
{
    assert(field);

    free(field->type_name);
    free(field->name);
}

/* ---------- methods */

void cpp_method_dispose(
    struct cpp_method *method)
{
    assert(method);

    free(method->name);
    free(method->return_type_name);

    for (uint32_t i = 0; i < method->argument_count; i++)
        free(method->arguments[i]);
    
    free(method->arguments);
}

/* ---------- base classes */

void cpp_base_class_dispose(
    struct cpp_base_class *item)
{
    assert(item);

    free(item->type_name);
}

/* ---------- classes */

void cpp_class_dispose(
    struct cpp_class *item)
{
    assert(item);

    free(item->name);

    for (uint32_t i = 0; i < item->base_class_count; i++)
        cpp_base_class_dispose(&item->base_classes[i]);
    
    free(item->base_classes);

    for (uint32_t i = 0; i < item->member_count; i++)
        cpp_class_member_dispose(&item->members[i]);
    
    free(item->members);
}

void cpp_class_add_members(struct cpp_class *item, struct pdb_data *pdb, uint32_t members_type_index, uint64_t *offset)
{
    assert(item);
    assert(pdb);

    struct tpi_primitive primitive;
    if (tpi_primitive_get(&primitive, &pdb->tpi_header, &pdb->tpi_symbols, members_type_index))
    {
        // TODO
        printf("%s:%i: TODO: ", __FILE__, __LINE__);
        tpi_primitive_print(&primitive, 0, stdout);
        printf("\n");
    }

    struct tpi_symbol *members_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, members_type_index);
    assert(members_symbol);
    assert(members_symbol->leaf == LF_FIELDLIST);

    printf("%s:%i: TODO: ", __FILE__, __LINE__);
    tpi_symbol_print(members_symbol, 0, stdout);
    printf("\n");

    // TODO
    (void)offset;
}

/* ---------- class members */

void cpp_class_member_dispose(
    struct cpp_class_member *member)
{
    assert(member);

    switch (member->type)
    {
    case CPP_CLASS_MEMBER_TYPE_CLASS:
        cpp_class_dispose(&member->class_);
        break;

    case CPP_CLASS_MEMBER_TYPE_ENUM:
        cpp_enum_dispose(&member->enum_);
        break;

    case CPP_CLASS_MEMBER_TYPE_FIELD:
        cpp_field_dispose(&member->field);
        break;

    case CPP_CLASS_MEMBER_TYPE_METHOD:
        cpp_method_dispose(&member->method);
        break;

    case CPP_CLASS_MEMBER_TYPE_TYPEDEF:
        cpp_typedef_dispose(&member->typedef_);
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled cpp_class_member_type value: %i\n", __FILE__, __LINE__, member->type);
        exit(EXIT_FAILURE);
    }
}

/* ---------- headers */

void cpp_header_dispose(
    struct cpp_header *header)
{
    assert(header);

    free(header->path);
}

/* ---------- module members */

void cpp_module_member_dispose(
    struct cpp_module_member *member)
{
    assert(member);

    switch (member->type)
    {
    case CPP_MODULE_MEMBER_TYPE_CLASS:
        cpp_class_dispose(&member->class_);
        break;

    case CPP_MODULE_MEMBER_TYPE_ENUM:
        cpp_enum_dispose(&member->enum_);
        break;

    case CPP_MODULE_MEMBER_TYPE_USER_DEFINED_TYPE:
        free(member->user_defined_type);
        break;

    case CPP_MODULE_MEMBER_TYPE_USING_NAMESPACE:
        free(member->using_namespace);
        break;

    case CPP_MODULE_MEMBER_TYPE_CONSTANT:
        free(member->constant);
        break;

    case CPP_MODULE_MEMBER_TYPE_DATA:
        free(member->data);
        break;

    case CPP_MODULE_MEMBER_TYPE_THREAD_STORAGE:
        free(member->thread_storage);
        break;

    case CPP_MODULE_MEMBER_TYPE_PROCEDURE:
        cpp_procedure_dispose(&member->procedure);
        break;

    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled cpp_module_member_type value: %i\n", __FILE__, __LINE__, member->type);
        exit(EXIT_FAILURE);
    }
}

/* ---------- modules */

void cpp_module_dispose(
    struct cpp_module *module)
{
    assert(module);

    free(module->path);

    for (uint32_t i = 0; i < module->header_count; i++)
        cpp_header_dispose(&module->headers[i]);
    
    free(module->headers);

    for (uint32_t i = 0; i < module->member_count; i++)
        cpp_module_member_dispose(&module->members[i]);
    
    free(module->members);
}

void cpp_module_add_type_definition(
    struct cpp_module *module,
    struct pdb_data *pdb,
    uint32_t type_index,
    uint32_t line)
{
    assert(module);
    assert(pdb);

    struct tpi_symbol *symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, type_index);

    if (!symbol)
        return;
    
    // Don't add a class or enum if we already have
    for (uint32_t i = 0; i < module->member_count; i++)
    {
        struct cpp_module_member *member = &module->members[i];

        switch (member->type)
        {
        case CPP_MODULE_MEMBER_TYPE_CLASS:
            if (member->class_.type_index == type_index)
                return;
            break;
        
        case CPP_MODULE_MEMBER_TYPE_ENUM:
            if (member->enum_.type_index == type_index)
                return;
            break;
        
        default:
            break;
        }
    }

    switch (symbol->leaf)
    {
    case LF_CLASS:
    case LF_CLASS_ST:
    case LF_STRUCTURE:
    case LF_STRUCTURE_ST:
    case LF_STRUCTURE19:
    case LF_INTERFACE:
    {
        struct cpp_module_member member;
        memset(&member, 0, sizeof(member));

        member.type = CPP_MODULE_MEMBER_TYPE_CLASS;

        switch (symbol->leaf)
        {
        case LF_CLASS:
        case LF_CLASS_ST:
            member.class_.type = CPP_CLASS_TYPE_CLASS;
            break;
        case LF_STRUCTURE:
        case LF_STRUCTURE_ST:
        case LF_STRUCTURE19:
            member.class_.type = CPP_CLASS_TYPE_STRUCT;
            break;
        case LF_INTERFACE:
            member.class_.type = CPP_CLASS_TYPE_INTERFACE;
            break;
        default:
            break;
        }

        if (symbol->class_.name)
        {
            member.class_.name = strdup(symbol->class_.name);
            assert(member.class_.name);
        }

        member.class_.type_index = type_index;
        member.class_.line = line;
        member.class_.size = symbol->class_.size;

        struct tpi_symbol *derived_from_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, symbol->class_.derived_from_type_index);
        
        if (derived_from_symbol)
        {
            //
            // TODO: add derived from?
            //
        }

        if (symbol->class_.header.properties.fwdref)
            member.class_.flags |= CPP_CLASS_IS_DECLARATION;
        else
            cpp_class_add_members(&member.class_, pdb, symbol->class_.header.fields_type_index, NULL);

        DYNARRAY_PUSH(module->members, module->member_count, member);
        break;
    }

    case LF_UNION:
    case LF_UNION_ST:
    {
        struct cpp_module_member member;
        memset(&member, 0, sizeof(member));

        member.type = CPP_MODULE_MEMBER_TYPE_CLASS;
        member.class_.type = CPP_CLASS_TYPE_STRUCT;

        if (symbol->union_.name)
        {
            member.class_.name = strdup(symbol->union_.name);
            assert(member.class_.name);
        }

        member.class_.flags |= CPP_CLASS_IS_UNION;
        member.class_.type_index = type_index;
        member.class_.line = line;
        member.class_.size = symbol->union_.size;

        if (symbol->union_.header.properties.fwdref)
            member.class_.flags |= CPP_CLASS_IS_DECLARATION;
        else
            cpp_class_add_members(&member.class_, pdb, symbol->union_.header.fields_type_index, NULL);

        DYNARRAY_PUSH(module->members, module->member_count, member);
        break;
    }
    
    case LF_ENUM:
    case LF_ENUM_ST:
    {
        struct cpp_module_member member;
        memset(&member, 0, sizeof(member));

        member.type = CPP_MODULE_MEMBER_TYPE_ENUM;

        if (symbol->enumeration.name)
        {
            member.enum_.name = strdup(symbol->enumeration.name);
            assert(member.enum_.name);
        }

        member.enum_.type_index = type_index;
        member.enum_.line = line;

        if (symbol->enumeration.header.properties.fwdref)
            member.enum_.flags |= CPP_CLASS_IS_DECLARATION;
        else
            cpp_enum_add_members(&member.enum_, pdb, symbol->enumeration.header.fields_type_index);

        DYNARRAY_PUSH(module->members, module->member_count, member);
        break;
    }
    
    default:
        // TODO
        break;
    }

    //
    // TODO: finish
    //
}
