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
            struct cpp_class_member member;
            memset(&member, 0, sizeof(member));

            member.type = CPP_CLASS_MEMBER_TYPE_METHOD;
            
            member.method.name = strdup(symbol->method.name);
            assert(member.method.name);

            struct tpi_symbol *method_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, symbol->method.header.method_type_index);

            if (!method_symbol)
            {
                // HACK: need to check this...
                member.method.return_type_name = strdup("void");
                assert(member.method.return_type_name);
            }
            else
            {
                assert(method_symbol->leaf == LF_MFUNCTION);
                
                // TODO: get return_type_name
                // TODO: get arguments (dynamic array of type names)
                memcpy(&member.method.field_attributes, &symbol->method.header.attributes, sizeof(member.method.field_attributes));
                // TODO: copy procedure attributes
            }

            DYNARRAY_PUSH(item->members, item->member_count, member);
            break;
        }

        case LF_METHOD:
        case LF_METHOD_ST:
        {
            struct tpi_symbol *method_list_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, symbol->overloaded_method.header.method_list_type_index);
            assert(method_list_symbol);
            assert(method_list_symbol->leaf == LF_METHODLIST);

            for (uint32_t i = 0; i < method_list_symbol->method_list.count; i++)
            {
                struct tpi_method_list_entry *entry = &method_list_symbol->method_list.entries[i];
                
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_METHOD;
                
                member.method.name = strdup(symbol->overloaded_method.name);
                assert(member.method.name);

                struct tpi_symbol *method_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, entry->header.method_type_index);

                if (!method_symbol)
                {
                    // HACK: need to check this...
                    member.method.return_type_name = strdup("void");
                    assert(member.method.return_type_name);
                }
                else
                {
                    assert(method_symbol->leaf == LF_MFUNCTION);
                    
                    // TODO: get return_type_name
                    // TODO: get arguments (dynamic array of type names)
                    memcpy(&member.method.field_attributes, &entry->header.attributes, sizeof(member.method.field_attributes));
                    // TODO: copy procedure attributes
                }

                DYNARRAY_PUSH(item->members, item->member_count, member);
            }
            break;
        }

        case LF_NESTTYPE:
        case LF_NESTTYPE_ST:
        case LF_NESTTYPEEX:
        case LF_NESTTYPEEX_ST:
        {
            struct tpi_primitive nested_primitive;
            if (tpi_primitive_get(&nested_primitive, &pdb->tpi_header, &pdb->tpi_symbols, symbol->nested_type.header.nested_type_index))
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_TYPEDEF;

                member.typedef_.type_name = NULL; // TODO:
                // let mut type_name = primitive_name(data.kind).to_string();
                // if data.indirection.is_some() {
                //     type_name.push_str(" *");
                // } else {
                //     type_name.push(' ');
                // }
                // type_name.push_str(nested_data.name.to_string().to_string().as_str());
                
                member.typedef_.underlying_type_index = symbol->nested_type.header.nested_type_index;
                
                memcpy(
                    &member.typedef_.field_attributes,
                    &symbol->nested_type.header.attributes,
                    sizeof(member.typedef_.field_attributes));
                
                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }

            struct tpi_symbol *nested_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, symbol->nested_type.header.nested_type_index);
            assert(nested_symbol);

            switch (nested_symbol->leaf)
            {
            case LF_CLASS:
            case LF_CLASS_ST:
            case LF_STRUCTURE:
            case LF_STRUCTURE_ST:
            case LF_STRUCTURE19:
            case LF_INTERFACE:
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_CLASS;

                switch (nested_symbol->leaf)
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

                if (nested_symbol->class_.name)
                {
                    member.class_.name = strdup(nested_symbol->class_.name);
                    assert(member.class_.name);
                }

                member.class_.type_index = symbol->nested_type.header.nested_type_index;
                member.class_.size = nested_symbol->class_.size;

                struct tpi_symbol *derived_from_symbol = tpi_symbol_get(&pdb->tpi_header, &pdb->tpi_symbols, nested_symbol->class_.derived_from_type_index);

                if (derived_from_symbol)
                {
                    //
                    // TODO: add derived from?
                    //
                }

                if (nested_symbol->class_.header.properties.fwdref)
                    member.class_.flags |= CPP_CLASS_IS_DECLARATION;
                else
                    cpp_class_add_members(&member.class_, pdb, nested_symbol->class_.header.fields_type_index);

                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }

            case LF_UNION:
            case LF_UNION_ST:
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_CLASS;
                member.class_.type = CPP_CLASS_TYPE_STRUCT;
                member.class_.flags |= CPP_CLASS_IS_UNION;

                if (nested_symbol->union_.name)
                {
                    member.class_.name = strdup(nested_symbol->union_.name);
                    assert(member.class_.name);
                }

                member.class_.type_index = symbol->nested_type.header.nested_type_index;
                member.class_.size = nested_symbol->union_.size;

                if (nested_symbol->union_.header.properties.fwdref)
                    member.class_.flags |= CPP_CLASS_IS_DECLARATION;
                else
                    cpp_class_add_members(&member.class_, pdb, nested_symbol->union_.header.fields_type_index);

                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }

            case LF_ENUM:
            case LF_ENUM_ST:
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_ENUM;

                if (nested_symbol->enumeration.name)
                {
                    member.enum_.name = strdup(nested_symbol->enumeration.name);
                    assert(member.enum_.name);
                }

                member.enum_.type_index = symbol->nested_type.header.nested_type_index;

                //
                // TODO: get underlying_type_name here
                //

                if (nested_symbol->enumeration.header.properties.fwdref)
                    member.enum_.flags |= CPP_ENUM_IS_DECLARATION;
                else
                    cpp_enum_add_members(&member.enum_, pdb, nested_symbol->enumeration.header.fields_type_index);

                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }

            case LF_POINTER:
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_TYPEDEF;

                member.typedef_.type_name = NULL; // TODO:
                // let type_name = type_name(machine_type, type_info, type_finder, data.underlying_type, Some(nested_data.name.to_string().to_string()), None, true)?;

                member.typedef_.underlying_type_index = nested_symbol->pointer.header.underlying_type_index;

                memcpy(
                    &member.typedef_.field_attributes,
                    &symbol->nested_type.header.attributes,
                    sizeof(member.typedef_.field_attributes));

                memcpy(
                    &member.typedef_.pointer_attributes,
                    &nested_symbol->pointer.header.attributes,
                    sizeof(member.typedef_.pointer_attributes));
                
                member.typedef_.containing_class_type_index = nested_symbol->pointer.containing_class_type_index;

                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }
            
            case LF_MODIFIER:
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_TYPEDEF;

                member.typedef_.type_name = NULL; // TODO:
                // let mut type_name = String::new();
                // if data.constant {
                //     type_name.push_str("const ");
                // }
                // if data.volatile {
                //     type_name.push_str("volatile ");
                // }
                // type_name.push_str(self::type_name(machine_type, type_info, type_finder, data.underlying_type, Some(nested_data.name.to_string().to_string()), None, true)?.as_str());

                member.typedef_.underlying_type_index = nested_symbol->pointer.header.underlying_type_index;

                memcpy(
                    &member.typedef_.field_attributes,
                    &symbol->nested_type.header.attributes,
                    sizeof(member.typedef_.field_attributes));

                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }
            
            case LF_ARRAY:
            case LF_ARRAY_ST:
            case LF_STRIDED_ARRAY:
            {
                struct cpp_class_member member;
                memset(&member, 0, sizeof(member));

                member.type = CPP_CLASS_MEMBER_TYPE_TYPEDEF;

                member.typedef_.type_name = NULL; // TODO:
                // let mut type_name = type_name(machine_type, type_info, type_finder, data.element_type, Some(nested_data.name.to_string().to_string()), None, true)?;
                // let mut element_size = type_size(machine_type, type_info, type_finder, data.element_type)?;
                // if element_size == 0 {
                //     let element_type_data = type_finder.find(data.element_type)?.parse()?;
                //     let mut type_iter = type_info.iter();
                //     loop {
                //         let current_type_item = match type_iter.next() {
                //             Ok(Some(current_type_item)) => current_type_item,
                //             Ok(None) | Err(_) => break,
                //         };
                //         let current_type_data = match current_type_item.parse() {
                //             Ok(current_type_data) => current_type_data,
                //             Err(_) => continue,
                //         };
                //         match &current_type_data {
                //             pdb::TypeData::Primitive(_) if matches!(element_type_data, pdb::TypeData::Primitive(_)) => (),
                //             pdb::TypeData::Class(_) if matches!(element_type_data, pdb::TypeData::Class(_)) => (),
                //             pdb::TypeData::Member(_) if matches!(element_type_data, pdb::TypeData::Member(_)) => (),
                //             pdb::TypeData::MemberFunction(_) if matches!(element_type_data, pdb::TypeData::MemberFunction(_)) => (),
                //             pdb::TypeData::OverloadedMethod(_) if matches!(element_type_data, pdb::TypeData::OverloadedMethod(_)) => (),
                //             pdb::TypeData::Method(_) if matches!(element_type_data, pdb::TypeData::Method(_)) => (),
                //             pdb::TypeData::StaticMember(_) if matches!(element_type_data, pdb::TypeData::StaticMember(_)) => (),
                //             pdb::TypeData::Nested(_) if matches!(element_type_data, pdb::TypeData::Nested(_)) => (),
                //             pdb::TypeData::BaseClass(_) if matches!(element_type_data, pdb::TypeData::BaseClass(_)) => (),
                //             pdb::TypeData::VirtualBaseClass(_) if matches!(element_type_data, pdb::TypeData::VirtualBaseClass(_)) => (),
                //             pdb::TypeData::VirtualFunctionTablePointer(_) if matches!(element_type_data, pdb::TypeData::VirtualFunctionTablePointer(_)) => (),
                //             pdb::TypeData::Procedure(_) if matches!(element_type_data, pdb::TypeData::Procedure(_)) => (),
                //             pdb::TypeData::Pointer(_) if matches!(element_type_data, pdb::TypeData::Pointer(_)) => (),
                //             pdb::TypeData::Modifier(_) if matches!(element_type_data, pdb::TypeData::Modifier(_)) => (),
                //             pdb::TypeData::Enumeration(_) if matches!(element_type_data, pdb::TypeData::Enumeration(_)) => (),
                //             pdb::TypeData::Enumerate(_) if matches!(element_type_data, pdb::TypeData::Enumerate(_)) => (),
                //             pdb::TypeData::Array(_) if matches!(element_type_data, pdb::TypeData::Array(_)) => (),
                //             pdb::TypeData::Union(_) if matches!(element_type_data, pdb::TypeData::Union(_)) => (),
                //             pdb::TypeData::Bitfield(_) if matches!(element_type_data, pdb::TypeData::Bitfield(_)) => (),
                //             pdb::TypeData::FieldList(_) if matches!(element_type_data, pdb::TypeData::FieldList(_)) => (),
                //             pdb::TypeData::ArgumentList(_) if matches!(element_type_data, pdb::TypeData::ArgumentList(_)) => (),
                //             pdb::TypeData::MethodList(_) if matches!(element_type_data, pdb::TypeData::MethodList(_)) => (),
                //             _ => continue
                //         }
                //         if current_type_data.name() == element_type_data.name() {
                //             if let Ok(current_type_size) = type_size(machine_type, type_info, type_finder, current_type_item.index()) {
                //                 if current_type_size != 0 {
                //                     element_size = current_type_size;
                //                     break;
                //                 }
                //             }
                //         }
                //     }
                // }
                // for &size in data.dimensions.iter() {
                //     type_name = format!("{}[{}]", type_name, if element_size == 0 { size } else { size / element_size as u32 });
                //     element_size = size as usize;
                // }

                member.typedef_.underlying_type_index = symbol->nested_type.header.nested_type_index;

                memcpy(
                    &member.typedef_.field_attributes,
                    &symbol->nested_type.header.attributes,
                    sizeof(member.typedef_.field_attributes));

                DYNARRAY_PUSH(item->members, item->member_count, member);
                break;
            }
            
            case LF_PROCEDURE:
                // TODO
                break;
            
            default:
                fprintf(stderr, "%s:%i: ERROR: Unhandled tpi_leaf value: ", __FILE__, __LINE__);
                tpi_leaf_print(nested_symbol->leaf, stderr);
                fprintf(stderr, "\n");
                exit(EXIT_FAILURE);
            }
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

    case CPP_MODULE_MEMBER_TYPE_TYPEDEF:
        cpp_typedef_dispose(&member->typedef_);
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
