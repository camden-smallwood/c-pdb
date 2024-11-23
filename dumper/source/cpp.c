#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cpp.h"
#include "utils.h"

/* ---------- labels */

void cpp_label_dispose(struct cpp_label *label)
{
    assert(label);

    free(label->name);
}

/* ---------- variables */

void cpp_variable_dispose(struct cpp_variable *variable)
{
    assert(variable);

    free(variable->signature);
    free(variable->value);
    free(variable->comment);
}

/* ---------- code blocks */

void cpp_block_dispose(struct cpp_block *block)
{
    assert(block);

    for (uint32_t i = 0; i < block->statement_count; i++)
        cpp_statement_dispose(&block->statements[i]);

    free(block->statements);
}

/* ---------- statements */

void cpp_statement_dispose(struct cpp_statement *statement)
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

void cpp_procedure_dispose(struct cpp_procedure *procedure)
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

void cpp_typedef_dispose(struct cpp_typedef *item)
{
    assert(item);

    free(item->type_name);
}

void cpp_typedef_print(struct cpp_typedef *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    // TODO: determine if we need this
    (void)depth;

    // TODO: get underlying_type_name
    fprintf(stream, "typedef %s %s;", item->type_name, "<TODO>");
}

/* ---------- enum values */

void cpp_enum_value_dispose(struct cpp_enum_value *item)
{
    assert(item);

    free(item->name);
}

void cpp_enum_value_print(struct cpp_enum_value *item, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "%s = ", item->name);

    switch (item->variant.type)
    {
    case TPI_ENUMERATE_VARIANT_UINT8:
        fprintf(stream, "%u", item->variant.uint8);
        break;
    case TPI_ENUMERATE_VARIANT_UINT16:
        fprintf(stream, "%u", item->variant.uint16);
        break;
    case TPI_ENUMERATE_VARIANT_UINT32:
        fprintf(stream, "%u", item->variant.uint32);
        break;
    case TPI_ENUMERATE_VARIANT_UINT64:
        fprintf(stream, "%llu", item->variant.uint64);
        break;
    case TPI_ENUMERATE_VARIANT_INT8:
        fprintf(stream, "%i", item->variant.int8);
        break;
    case TPI_ENUMERATE_VARIANT_INT16:
        fprintf(stream, "%i", item->variant.int16);
        break;
    case TPI_ENUMERATE_VARIANT_INT32:
        fprintf(stream, "%i", item->variant.int32);
        break;
    case TPI_ENUMERATE_VARIANT_INT64:
        fprintf(stream, "%lli", item->variant.int64);
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled tpi_enumerate_variant value: ", __FILE__, __LINE__);
        tpi_enumerate_variant_print(&item->variant, 0, stderr);
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
}

/* ---------- enums */

void cpp_enum_dispose(struct cpp_enum *item)
{
    assert(item);

    free(item->name);
    free(item->underlying_type_name);

    for (uint32_t i = 0; i < item->value_count; i++)
        cpp_enum_value_dispose(&item->values[i]);

    free(item->values);
}

void cpp_enum_print(struct cpp_enum *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    fprintf(stream, "enum %s", item->name);

    if (item->underlying_type_name)
        fprintf(stream, " : %s", item->underlying_type_name);

    if (item->flags & CPP_ENUM_IS_DECLARATION)
    {
        fprintf(stream, ";");
        return;
    }

    fprintf(stream, "\n");
    fprintf_depth(stream, depth, "{\n");

    for (uint32_t i = 0; i < item->value_count; i++)
    {
        fprintf_depth(stream, depth + 1, "");
        cpp_enum_value_print(&item->values[i], stream);
        fprintf(stream, ",\n");
    }

    fprintf_depth(stream, depth, "};");
}

void cpp_enum_add_members(struct cpp_enum *item, struct pdb_data *pdb, uint32_t members_type_index)
{
    assert(item);
    assert(pdb);

    struct tpi_primitive primitive;
    if (tpi_primitive_get(&primitive, &pdb->tpi_header, &pdb->tpi_symbols, members_type_index))
    {
        if (primitive.type != TPI_PRIMITIVE_TYPE_NONE || primitive.indirection != TPI_PRIMITIVE_INDIRECTION_NONE)
        {
            fprintf(stderr, "%s:%i: ERROR: unhandled enum primitive type: ", __FILE__, __LINE__);
            tpi_primitive_print(&primitive, 0, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        assert(item->value_count == 0);
        item->flags |= CPP_ENUM_IS_DECLARATION;
        return;
    }

    struct tpi_symbol *members_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, members_type_index);
    
    // HACK: idk why some enums are showing up with huge indices
    if (!members_symbol)
    {
        assert(item->value_count == 0);
        item->flags |= CPP_ENUM_IS_DECLARATION;
        return;
    }

    assert(members_symbol->leaf == LF_FIELDLIST);
    item->flags &= ~CPP_ENUM_IS_DECLARATION;

    for (uint32_t i = 0; i < members_symbol->field_list.count; i++)
    {
        struct tpi_symbol *value_symbol = &members_symbol->field_list.fields[i];
        assert(value_symbol->leaf == LF_ENUMERATE || value_symbol->leaf == LF_ENUMERATE_ST);

        struct cpp_enum_value value;
        memset(&value, 0, sizeof(value));

        value.name = strdup(value_symbol->enumerate.name);
        assert(value.name);

        memcpy(&value.variant, &value_symbol->enumerate.variant, sizeof(value.variant));

        DYNARRAY_PUSH(item->values, item->value_count, value);
    }

    if (members_symbol->field_list.continuation_type_index)
        cpp_enum_add_members(item, pdb, members_symbol->field_list.continuation_type_index);
}

/* ---------- fields */

void cpp_field_dispose(struct cpp_field *field)
{
    assert(field);

    free(field->type_name);
    free(field->name);
}

void cpp_field_print(struct cpp_field *field, uint32_t depth, FILE *stream)
{
    assert(field);
    assert(stream);

    // TODO: determine if we need this
    (void)depth;

    fprintf(stream, "%s %s;", field->type_name, field->name);
}

/* ---------- methods */

void cpp_method_dispose(struct cpp_method *method)
{
    assert(method);

    free(method->name);
    free(method->return_type_name);

    for (uint32_t i = 0; i < method->argument_count; i++)
        free(method->arguments[i]);

    free(method->arguments);
}

void cpp_method_print(struct cpp_method *method, uint32_t depth, FILE *stream)
{
    assert(method);
    assert(stream);

    // TODO: determine if we need this
    (void)depth;

    fprintf(stream, "%s %s(", method->return_type_name, method->name);

    for (uint32_t i = 0; i < method->argument_count; i++)
    {
        if (i > 0)
            fprintf(stream, ", ");
        
        fprintf(stream, "%s", method->arguments[i]);
    }

    fprintf(stream, ");");
}

/* ---------- base classes */

void cpp_base_class_dispose(struct cpp_base_class *item)
{
    assert(item);

    free(item->type_name);
}

void cpp_base_class_print(struct cpp_base_class *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    // TODO: determine if we need this
    (void)depth;

    fprintf(stream, "%s", item->type_name);
}

/* ---------- classes */

void cpp_class_dispose(struct cpp_class *item)
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

void cpp_class_print(struct cpp_class *item, uint32_t depth, FILE *stream)
{
    assert(item);
    assert(stream);

    if (item->flags & CPP_CLASS_IS_UNION)
    {
        fprintf(stream, "union");
    }
    else
    {
        switch (item->type)
        {
        case CPP_CLASS_TYPE_CLASS:
            fprintf(stream, "class");
            break;
        case CPP_CLASS_TYPE_STRUCT:
            fprintf(stream, "struct");
            break;
        case CPP_CLASS_TYPE_INTERFACE:
            fprintf(stream, "interface");
            break;
        default:
            fprintf(stderr, "%s:%i: ERROR: unhandled cpp_class_type value: %i\n", __FILE__, __LINE__, item->type);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stream, " %s", item->name);

    for (uint32_t i = 0; i < item->base_class_count; i++)
    {
        if (i == 0)
            fprintf(stream, " :\n");
        
        fprintf_depth(stream, depth + 1, "");
        cpp_base_class_print(&item->base_classes[i], depth + 1, stream);

        if (i < item->base_class_count - 1)
            fprintf(stream, ",\n");
    }

    if (item->flags & CPP_CLASS_IS_DECLARATION)
    {
        fprintf(stream, ";");
        return;
    }

    fprintf(stream, "\n");
    fprintf_depth(stream, depth, "{\n");

    for (uint32_t i = 0; i < item->member_count; i++)
    {
        fprintf_depth(stream, depth + 1, "");
        cpp_class_member_print(&item->members[i], depth + 1, stream);
        fprintf(stream, "\n");
    }

    fprintf_depth(stream, depth, "};");
}

void cpp_class_add_members(struct cpp_class *item, struct pdb_data *pdb, uint32_t members_type_index)
{
    assert(item);
    assert(pdb);

    struct tpi_primitive primitive;
    if (tpi_primitive_get(&primitive, &pdb->tpi_header, &pdb->tpi_symbols, members_type_index))
    {
        if (primitive.type != TPI_PRIMITIVE_TYPE_NONE || primitive.indirection != TPI_PRIMITIVE_INDIRECTION_NONE)
        {
            fprintf(stderr, "%s:%i: ERROR: unhandled class primitive type: ", __FILE__, __LINE__);
            tpi_primitive_print(&primitive, 0, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }

        assert(item->member_count == 0);
        item->flags |= CPP_CLASS_IS_DECLARATION;
        return;
    }

    struct tpi_symbol *members_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, members_type_index);
    assert(members_symbol);
    assert(members_symbol->leaf == LF_FIELDLIST);

    for (uint32_t i = 0; i < members_symbol->field_list.count; i++)
    {
        struct tpi_symbol *symbol = &members_symbol->field_list.fields[i];

        switch (symbol->leaf)
        {
        case LF_MEMBER:
        case LF_MEMBER_ST:
        {
            struct cpp_class_member member;
            memset(&member, 0, sizeof(member));

            member.type = CPP_CLASS_MEMBER_TYPE_FIELD;

            // TODO: get type_name

            member.field.name = strdup(symbol->member.name);
            assert(member.field.name);

            member.field.offset = symbol->member.offset;

            memcpy(&member.field.attributes, &symbol->member.header.attributes, sizeof(member.field.attributes));

            DYNARRAY_PUSH(item->members, item->member_count, member);
            break;
        }

        case LF_STMEMBER:
        case LF_STMEMBER_ST:
        {
            struct cpp_class_member member;
            memset(&member, 0, sizeof(member));

            member.type = CPP_CLASS_MEMBER_TYPE_FIELD;

            // TODO: get type_name, prepend `static` to it

            member.field.name = strdup(symbol->static_member.name);
            assert(member.field.name);

            memcpy(&member.field.attributes, &symbol->static_member.header.attributes, sizeof(member.field.attributes));

            DYNARRAY_PUSH(item->members, item->member_count, member);
            break;
        }

        case LF_BCLASS:
        case LF_BINTERFACE:
        {
            struct cpp_base_class base_class;
            memset(&base_class, 0, sizeof(base_class));

            base_class.type_index = symbol->base_class.header.base_class_type_index;
            
            // TODO: get type_name

            base_class.offset = symbol->base_class.offset;

            DYNARRAY_PUSH(item->base_classes, item->base_class_count, base_class);
            break;
        }

        case LF_VBCLASS:
        case LF_IVBCLASS:
        {
            struct cpp_base_class base_class;
            memset(&base_class, 0, sizeof(base_class));

            base_class.type_index = symbol->virtual_base_class.header.base_class_type_index;
            
            // TODO: get type_name, prepend `virtual` to it?

            base_class.offset = symbol->virtual_base_class.base_pointer_offset;

            DYNARRAY_PUSH(item->base_classes, item->base_class_count, base_class);
            break;
        }

        case LF_VFUNCTAB:
        case LF_VFTABLE:
        {
            //
            // TODO: does this need handling?
            //
            break;
        }

        case LF_ONEMETHOD:
        case LF_ONEMETHOD_ST:
        {
            struct tpi_symbol *method_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, symbol->method.header.method_type_index);

            // HACK: need to check this...
            if (!method_symbol)
                break;
            
            assert(method_symbol);
            assert(method_symbol->leaf == LF_MFUNCTION);
            
            struct cpp_class_member member;
            memset(&member, 0, sizeof(member));

            member.type = CPP_CLASS_MEMBER_TYPE_METHOD;
            
            member.method.name = strdup(symbol->method.name);
            assert(member.method.name);

            // TODO: get return_type_name
            // TODO: get arguments (dynamic array of type names)

            memcpy(&member.method.field_attributes, &symbol->method.header.attributes, sizeof(member.method.field_attributes));

            DYNARRAY_PUSH(item->members, item->member_count, member);
            break;
        }

        case LF_METHOD:
        case LF_METHOD_ST:
        {
            // TODO
            break;
        }

        case LF_NESTTYPE:
        case LF_NESTTYPE_ST:
        case LF_NESTTYPEEX:
        case LF_NESTTYPEEX_ST:
        {
            // TODO
            break;
        }

        default:
            fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_leaf value: ", __FILE__, __LINE__);
            tpi_leaf_print(symbol->leaf, stderr);
            fprintf(stderr, "\n");
            exit(EXIT_FAILURE);
        }
    }
}

/* ---------- class members */

void cpp_class_member_dispose(struct cpp_class_member *member)
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

void cpp_class_member_print(struct cpp_class_member *member, uint32_t depth, FILE *stream)
{
    assert(member);
    assert(stream);

    switch (member->type)
    {
    case CPP_CLASS_MEMBER_TYPE_CLASS:
        cpp_class_print(&member->class_, depth, stream);
        break;
    case CPP_CLASS_MEMBER_TYPE_ENUM:
        cpp_enum_print(&member->enum_, depth, stream);
        break;
    case CPP_CLASS_MEMBER_TYPE_FIELD:
        cpp_field_print(&member->field, depth, stream);
        break;
    case CPP_CLASS_MEMBER_TYPE_METHOD:
        cpp_method_print(&member->method, depth, stream);
        break;
    case CPP_CLASS_MEMBER_TYPE_TYPEDEF:
        cpp_typedef_print(&member->typedef_, depth, stream);
        break;
    default:
        fprintf(stderr, "%s:%i: ERROR: unhandled cpp_class_member_type value: %i\n", __FILE__, __LINE__, member->type);
        exit(EXIT_FAILURE);
    }
}

/* ---------- headers */

void cpp_header_dispose(struct cpp_header *header)
{
    assert(header);

    free(header->path);
}

/* ---------- module members */

void cpp_module_member_dispose(struct cpp_module_member *member)
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

void cpp_module_dispose(struct cpp_module *module)
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
            cpp_class_add_members(&member.class_, pdb, symbol->class_.header.fields_type_index);

        // TODO: remove this VVV
        cpp_class_print(&member.class_, 0, stdout);
        printf("\n");

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
        member.class_.flags |= CPP_CLASS_IS_UNION;

        if (symbol->union_.name)
        {
            member.class_.name = strdup(symbol->union_.name);
            assert(member.class_.name);
        }

        member.class_.type_index = type_index;
        member.class_.line = line;
        member.class_.size = symbol->union_.size;

        if (symbol->union_.header.properties.fwdref)
            member.class_.flags |= CPP_CLASS_IS_DECLARATION;
        else
            cpp_class_add_members(&member.class_, pdb, symbol->union_.header.fields_type_index);

        // TODO: remove this VVV
        cpp_class_print(&member.class_, 0, stdout);
        printf("\n");

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

        //
        // TODO: get underlying_type_name here
        //

        if (symbol->enumeration.header.properties.fwdref)
            member.enum_.flags |= CPP_ENUM_IS_DECLARATION;
        else
            cpp_enum_add_members(&member.enum_, pdb, symbol->enumeration.header.fields_type_index);

        // TODO: remove this VVV
        cpp_enum_print(&member.enum_, 0, stdout);
        printf("\n");

        DYNARRAY_PUSH(module->members, module->member_count, member);
        break;
    }

    default:
        break;
        // fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_leaf value in %s: ", __FILE__, __LINE__, __FUNCTION__);
        // tpi_leaf_print(symbol->leaf, stderr);
        // fprintf(stderr, "\n");
        // exit(EXIT_FAILURE);
    }
}
