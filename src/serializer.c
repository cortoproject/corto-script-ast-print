#include <corto/script/ast/print/print.h>

typedef struct serdata_t {
    corto_buffer b;
    corto_uint32 indent;
} print_printData;

static
void ser_indent(
    serdata_t *data)
{
    corto_uint32 indent = data->indent;
    while (indent--) {
        corto_buffer_append(&data->b, "|   ");
    }
}

static
int16_t ser_primitive(
    corto_walk_opt* s,
    corto_value *info,
    void* userData)
{
    serdata_t *data = (serdata_t*)userData;
    corto_type t = corto_value_typeof(info);
    void *ptr = corto_value_ptrof(info);
    corto_string str = NULL;
    bool is_string = false;
    corto_primitiveKind primitive_kind = corto_primitive(t)->kind;


    if (primitive_kind == CORTO_TEXT) {
        is_string = true;
    }

    if (is_string && !*(corto_string*)ptr) {
        /* Don't serialize strings when value is NULL */
        return 0;
    }

    if ((primitive_kind == CORTO_BOOLEAN) && (!*(corto_bool*)ptr)) {
        /* Don't serialize booleans when FALSE */
        return 0;
    }

    /* Don't serialize constants when 0 */
    if (primitive_kind == CORTO_ENUM || primitive_kind == CORTO_BITMASK) {
        if (!*(corto_uint32*)ptr) {
            return 0;
        }
    }

    corto_ptr_cast(t, ptr, corto_string_o, &str);
    if (info->kind == CORTO_MEMBER) {
        ser_indent(data);
        corto_buffer_append(&data->b, "%s: ", corto_idof(info->is.member.member));
    } else if (info->kind == CORTO_ELEMENT) {
        if (info->is.element.index) {
            corto_buffer_append(&data->b, ", ");
        }
    } else {
        ser_indent(data);
    }

    corto_buffer_appendstr(&data->b, "'");
    if (!strcmp(str, "'")) {
        corto_buffer_appendstr(&data->b, "\\'");
    } else {
        corto_buffer_appendstr(&data->b, str);
    }
    corto_buffer_appendstr(&data->b, "'");

    if (info->kind == CORTO_MEMBER) {
        corto_buffer_append(&data->b, "\n");
    }
    corto_dealloc(str);

    return 0;
}

static
int16_t ser_ref(
    corto_walk_opt* s,
    corto_value *info,
    void* userData)
{
    serdata_t *data = (serdata_t*)userData;
    corto_type t = corto_value_typeof(info);
    void *ptr = corto_value_ptrof(info);

    if (corto_type_instanceof(ast_Node_o, t)) {
        if (*(corto_object*)ptr) {
            ser_indent(data);
            if (info->kind == CORTO_MEMBER) {
                corto_buffer_append(&data->b, "%s: ", corto_idof(info->is.member.member));
            }
            corto_buffer_append(&data->b, "%s\n", corto_idof(corto_typeof(*(corto_object*)ptr)));
            data->indent ++;
            corto_walk(s, *(corto_object*)ptr, userData);
            data->indent --;
        }
    }

    return 0;
}

static
int16_t ser_collection(
    corto_walk_opt* s,
    corto_value *info,
    void* userData)
{
    serdata_t *data = (serdata_t*)userData;
    corto_type t = corto_value_typeof(info);
    corto_type elementType = corto_collection(t)->elementType;

    if (!elementType->reference || corto_type_instanceof(ast_Node_o, elementType)) {
        if (info->kind == CORTO_MEMBER) {
            void *ptr = corto_value_ptrof(info);
            if (corto_ll_count(*(corto_ll*)ptr)) {
                ser_indent(data);
                if (elementType->reference) {
                    corto_buffer_append(&data->b, "%s:\n", corto_idof(info->is.member.member));
                } else {
                    corto_buffer_append(&data->b, "%s: [", corto_idof(info->is.member.member));
                }

                data->indent ++;
                corto_walk_elements(s, info, userData);
                data->indent --;

                if (!elementType->reference) {
                    corto_buffer_append(&data->b, "]\n", corto_idof(info->is.member.member));
                }
            }
        }
    }

    return 0;
}

char* cortoscript_ast_to_string(
    ast_Node node)
{
    serdata_t serData;
    serData.b = CORTO_BUFFER_INIT;
    serData.indent = 0;
    corto_walk_opt s;
    corto_walk_init(&s);
    s.program[CORTO_PRIMITIVE] = ser_primitive;
    s.program[CORTO_COLLECTION] = ser_collection;
    s.accessKind = CORTO_NOT;
    s.access = CORTO_HIDDEN | CORTO_PRIVATE | CORTO_LOCAL;
    s.reference = ser_ref;

    if (!corto_instanceof(ast_Scope_o, node)) {
        corto_buffer_append(&serData.b, "%s\n", corto_idof(corto_typeof(node)));
        serData.indent ++;
    }

    corto_value v = corto_value_mem(node, corto_typeof(node));
    corto_walk_value(&s, &v, &serData);
    return corto_buffer_str(&serData.b);
}

char* cortoscript_code_to_string(
    const char *code)
{
    char *result = NULL;
    ast_Node node = cortoscript_ast_parse(code, false);
    if (node) {
        result = cortoscript_ast_to_string(node);
        corto_release(node);
    }
    return result;
}

char* cortoscript_expr_to_string(
    const char *code)
{
    char *result = NULL;
    ast_Node node = cortoscript_ast_parse(code, true);
    if (node) {
        result = cortoscript_ast_to_string(node);
        corto_release(node);
    }
    return result;
}
