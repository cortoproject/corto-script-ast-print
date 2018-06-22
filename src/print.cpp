/* This is a managed file. Do not delete this comment. */

#include <corto/script/ast/print/print.h>


extern "C"
int cortomain(int argc, char *argv[]) {

    if (argc > 1) {
        ast_Node node = NULL;
        if (corto_file_test(argv[1])) {
            node = cortoscript_ast_parse_file(argv[1], false);
        } else {
            node = cortoscript_ast_parse(argv[1], false);
        }

        if (node) {
            char *str = cortoscript_ast_to_string(node);
            if (str) {
                corto_log("%s\n", str);
                free(str);
            }
        } else {
            corto_throw("failed to parse '%s'", argv[1]);
            return -1;
        }
    }

    return 0;
}
