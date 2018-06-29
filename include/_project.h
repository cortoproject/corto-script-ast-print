/* _project.h
 * This file is generated. Do not modify the contents of this file.
 */

#if CORTO_SCRIPT_AST_PRINT_IMPL && defined _MSC_VER
#define CORTO_SCRIPT_AST_PRINT_EXPORT __declspec(dllexport)
#elif CORTO_SCRIPT_AST_PRINT_IMPL
#define CORTO_SCRIPT_AST_PRINT_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define CORTO_SCRIPT_AST_PRINT_EXPORT __declspec(dllimport)
#else
#define CORTO_SCRIPT_AST_PRINT_EXPORT
#endif

