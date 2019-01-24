/* print.h
 * This is the main package file. Include this file in other projects.
 * Only modify inside the header-end and body-end sections.
 */

#ifndef CORTO_SCRIPT_AST_PRINT_H
#define CORTO_SCRIPT_AST_PRINT_H

#include "bake_config.h"

#define CORTO_SCRIPT_AST_PRINT_ETC ut_locate("corto.script.ast.print", NULL, UT_LOCATE_ETC)

/* $header() */
/* $end */

#include "_type.h"
#include "_interface.h"
#include "_load.h"
#include "_binding.h"

#include <corto.script.ast.print.c>

/* $body() */
#ifdef __cplusplus
extern "C" {
#endif

CORTO_SCRIPT_AST_PRINT_EXPORT
char* cortoscript_ast_to_string(
    ast_Node node);

CORTO_SCRIPT_AST_PRINT_EXPORT
char* cortoscript_code_to_string(
    const char *code);

CORTO_SCRIPT_AST_PRINT_EXPORT
char* cortoscript_expr_to_string(
    const char *code);

#ifdef __cplusplus
}
#endif
/* $end */

#endif

