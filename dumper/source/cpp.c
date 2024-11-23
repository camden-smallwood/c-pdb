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
    
    (void)line; // TODO

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

    uint32_t absolute_index = tpi_symbol_index_to_absolute_index(&pdb->tpi_header, &pdb->tpi_symbols, type_index);
    if (absolute_index == UINT32_MAX)
        return;
    
    struct tpi_symbol *symbol = &pdb->tpi_symbols.symbols[absolute_index];

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
        member.class_.type = CPP_CLASS_TYPE_CLASS;
        member.class_.type_index = type_index;
        member.class_.line = line;
        member.class_.size = symbol->class_.size;

        if (symbol->class_.name)
        {
            member.class_.name = strdup(symbol->class_.name);
            assert(member.class_.name);
        }

        uint32_t derived_from_type_index = tpi_symbol_index_to_absolute_index(&pdb->tpi_header, &pdb->tpi_symbols, symbol->class_.derived_from_type_index);
        if (derived_from_type_index != UINT32_MAX)
        {
            //
            // TODO: add derived from?
            //
        }

        if (symbol->class_.header.properties.fwdref)
        {
            //
            // TODO: add forward reference
            //
        }
        else
        {
            uint32_t fields_absolute_index = tpi_symbol_index_to_absolute_index(&pdb->tpi_header, &pdb->tpi_symbols, symbol->union_.header.fields_type_index);
            if (fields_absolute_index == UINT32_MAX)
                break;
            
            struct tpi_symbol *fields_symbol = &pdb->tpi_symbols.symbols[fields_absolute_index];
            assert(fields_symbol->leaf == LF_FIELDLIST);

            //
            // TODO: add fields
            //
        }

        DYNARRAY_PUSH(module->members, module->member_count, member);
        break;
    }

    case LF_UNION:
    case LF_UNION_ST:
    {
        // TODO: same as above
        break;
    }
    
    case LF_ENUM:
    case LF_ENUM_ST:
    {
        // TODO: same as above
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
