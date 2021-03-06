#include <stdio.h>  /* puts */
#include <stdlib.h> /* calloc, free */

#include "token.h"

/* alloc and init a new token
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_token *ic_token_new(enum ic_token_id id, char *line, unsigned int offset, char *file, unsigned int line_num) {
    struct ic_token *token = 0;

    token = calloc(1, sizeof(struct ic_token));
    if (!token) {
        puts("ic_token_new: call to calloc failed");
        return 0;
    }

    if (!ic_token_init(token, id, line, offset, file, line_num)) {
        puts("ic_token_new: call to ic_token_init failed");
        free(token);
        return 0;
    }

    return token;
}

/* init an existing token
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_token_init(struct ic_token *token, enum ic_token_id id, char *line, unsigned int offset, char *file, unsigned int line_num) {
    if (!token) {
        puts("ic_token_init: token was null");
        return 0;
    }

    if (!line) {
        puts("ic_token_init: line was null");
        return 0;
    }

    if (!file) {
        puts("ic_token_init: file was null");
        return 0;
    }

    token->id = id;
    token->line = line;
    token->offset = offset;
    token->file = file;
    token->line_num = line_num;

    return 1;
}

/* set string data on token
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_token_set_string(struct ic_token *token, char *string, unsigned int len) {
    if (!token) {
        puts("ic_token_set_string: token was null");
        return 0;
    }

    /* check if token->id allows for a string */
    switch (token->id) {
        case IC_IDENTIFIER:
        case IC_LITERAL_STRING:
        case IC_COMMENT:
            /* string allowed */
            break;

        default:
            fputs("ic_token_set_string: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 1;
            break;
    }

    token->u.str.string = string;
    token->u.str.len = len;
    return 1;
}

/* get string representation of token id
 *
 * returns * on success
 * returns 0 on failure
 */
char *ic_token_id_get_representation(enum ic_token_id id) {
    switch (id) {
        case IC_IDENTIFIER:
        case IC_LITERAL_UNSIGNED_INTEGER:
        case IC_LITERAL_SIGNED_INTEGER:
        case IC_LITERAL_STRING:
        case IC_COMMENT:
            puts("ic_token_id_get_representation: payload types are not supported");
            return 0;
            break;

        case IC_TRUE:
            return "True";
            break;
        case IC_FALSE:;
            return "False";
            break;

        case IC_NEWLINE:
            return "\n";
            break;
        case IC_WHITESPACE:
            return " ";
            break;

        case IC_END:
            return "end";
            break;

        case IC_BEGIN:
            return "begin";
            break;
        case IC_IF:
            return "if";
            break;
        case IC_ELSIF:
            return "elsif";
            break;
        case IC_ELSE:
            return "else";
            break;
        case IC_RETURN:
            return "return";
            break;
        case IC_LET:
            return "let";
            break;
        case IC_FOR:
            return "for";
            break;
        case IC_IN:
            return "in";
            break;
        case IC_WHILE:
            return "while";
            break;
        case IC_MATCH:
            return "match";
            break;
        case IC_CASE:
            return "case";
            break;

        case IC_IMPORT:
            return "import";
            break;
        case IC_BUILTIN:
            return "builtin";
            break;
        case IC_OP:
            return "op";
            break;
        case IC_FUNC:
            return "fn";
            break;
        case IC_TYPE:
            return "type";
            break;
        case IC_ENUM:
            return "enum";
            break;
        case IC_UNION:
            return "union";
            break;

        case IC_ARROW:
            return "->";
            break;
        case IC_EQUAL:
            return "==";
            break;
        case IC_ASSIGN:
            return "=";
            break;
        case IC_DOUBLECOLON:
            return "::";
            break;
        case IC_PERIOD:
            return ".";
            break;
        case IC_COMMA:
            return ",";
            break;
        case IC_AND:
            return "and";
            break;
        case IC_OR:
            return "or";
            break;

        case IC_DIVIDE:
            return "/";
            break;

        case IC_DOLLAR:
            return "$";
            break;
        case IC_PERCENT:
            return "%";
            break;
        case IC_AMPERSAND:
            return "&";
            break;
        case IC_AT:
            return "@";
            break;
        case IC_ASTERISK:
            return "*";
            break;
        case IC_CARET:
            return "^";
            break;
        case IC_PLUS:
            return "+";
            break;
        case IC_MINUS:
            return "-";
            break;

        case IC_LRBRACKET:
            return "(";
            break;
        case IC_RRBRACKET:
            return ")";
            break;

        case IC_LSBRACKET:
            return "[";
            break;
        case IC_RSBRACKET:
            return "]";
            break;

        case IC_LESSTHAN_EQUAL:
            return "<=";
            break;
        case IC_GREATERTHAN_EQUAL:
            return ">=";
            break;
        case IC_LESSTHAN:
            return "<";
            break;
        case IC_GREATERTHAN:
            return ">";
            break;

        default:
            printf("ic_token_id_get_representation: ERROR, unknown token id '%d'\n", id);
            return 0;
            break;
    }
}
/* get string representation of token
 *
 * returns * on success
 * returns 0 on failure
 */
char *ic_token_get_representation(struct ic_token *token) {
    if (!token) {
        puts("ic_token_get_representation: token was null");
        return 0;
    }

    return ic_token_id_get_representation(token->id);
}

/* get string data on token
 *
 * returns * on success
 * returns 0 on failure
 */
char *ic_token_get_string(struct ic_token *token) {
    if (!token) {
        puts("ic_token_get_string: token was null");
        return 0;
    }

    /* check if token->id allows for a string */
    switch (token->id) {
        case IC_IDENTIFIER:
        case IC_LITERAL_STRING:
        case IC_COMMENT:
            /* string allowed */
            break;

        default:
            fputs("ic_token_get_string: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 0;
            break;
    }

    return token->u.str.string;
}

/* get string length on token
 *
 * returns length on success
 * returns 0 on failure
 */
unsigned int ic_token_get_string_length(struct ic_token *token) {
    if (!token) {
        puts("ic_token_get_string_length: token was null");
        return 0;
    }

    /* check if token->id allows for a string */
    switch (token->id) {
        case IC_IDENTIFIER:
        case IC_LITERAL_STRING:
        case IC_COMMENT:
            /* string allowed */
            break;

        default:
            fputs("ic_token_get_string_length: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 0;
            break;
    }

    return token->u.str.len;
}

/* set unsigned integer data on token
 *
 * returns 1 on success
 * returns 0 on failure
 */
int ic_token_set_unsigned_integer(struct ic_token *token, unsigned long int unsigned_integer) {
    if (!token) {
        puts("ic_token_set_unsigned_integer: token was null");
        return 0;
    }

    /* check if token->id allows for an integer */
    switch (token->id) {
        case IC_LITERAL_UNSIGNED_INTEGER:
            break;

        default:
            fputs("ic_token_set_unsigned_integer: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 0;
            break;
    }

    token->u.unsigned_integer = unsigned_integer;

    return 1;
}

/* get unsigned integer data on token
 *
 * returns * on success
 * returns 0 on failure
 */
unsigned long int ic_token_get_unsigned_integer(struct ic_token *token) {
    if (!token) {
        puts("ic_token_get_unsigned_integer: token was null");
        return 0;
    }

    /* check if token->id allows for an integer */
    switch (token->id) {
        case IC_LITERAL_UNSIGNED_INTEGER:
            break;

        default:
            fputs("ic_token_get_unsigned_integer: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 0;
            break;
    }

    return token->u.unsigned_integer;
}

/* set signed integer data on token
 *
 * returns 1 on success
 * returns 0 on failure
 */
int ic_token_set_signed_integer(struct ic_token *token, long int signed_integer) {
    if (!token) {
        puts("ic_token_set_signed_integer: token was null");
        return 0;
    }

    /* check if token->id allows for an integer */
    switch (token->id) {
        case IC_LITERAL_SIGNED_INTEGER:
            break;

        default:
            fputs("ic_token_set_signed_integer: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 0;
            break;
    }

    token->u.signed_integer = signed_integer;

    return 1;
}

/* get signed integer data on token
 *
 * returns * on success
 * returns 0 on failure
 */
long int ic_token_get_signed_integer(struct ic_token *token) {
    if (!token) {
        puts("ic_token_get_signed_integer: token was null");
        return 0;
    }

    /* check if token->id allows for an integer */
    switch (token->id) {
        case IC_LITERAL_SIGNED_INTEGER:
            break;

        default:
            fputs("ic_token_get_signed_integer: called on token not allowing a payload, token->id:", stdout);
            ic_token_id_print_debug(stdout, token->id);
            puts("");
            return 0;
            break;
    }

    return token->u.signed_integer;
}

/* destroy
 *
 * will only free token if free_token is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_token_destroy(struct ic_token *token, unsigned int free_token) {
    if (!token) {
        puts("ic_token_destroy: token was null");
        return 0;
    }

    if (free_token) {
        free(token);
    }

    return 1;
}

/* check if token is an operator
 *
 * returns 1 if it is an operator
 * returns 0 if not
 */
unsigned int ic_token_isoperator(struct ic_token *token) {
    if (!token) {
        puts("ic_token_isoperator: token was null");
        return 0;
    }

    switch (token->id) {
        case IC_PLUS:
        case IC_MINUS:
        case IC_DIVIDE:
        case IC_ASTERISK:
        case IC_PERCENT:
            return 1;

        case IC_EQUAL:
        case IC_AND:
        case IC_OR:
            return 1;

        case IC_LESSTHAN_EQUAL:
        case IC_GREATERTHAN_EQUAL:
        case IC_LESSTHAN:
        case IC_GREATERTHAN:
            return 1;

        case IC_PERIOD:
            /* a period is not an operator
             * it is a field access */
            return 0;

        default:
            break;
    }

    return 0;
}

/* check if token is a boolean
 *
 * returns 1 if it is a boolean
 * returns 0 if not
 */
unsigned int ic_token_is_boolean(struct ic_token *token) {
    if (!token) {
        puts("ic_token_is_boolean: token was null");
        return 0;
    }

    switch (token->id) {
        case IC_TRUE:
        case IC_FALSE:
            return 1;

        default:
            break;
    }

    return 0;
}

/* check if token is a permission
 *
 * returns 1 if it is a permission
 * returns 0 if not
 */
unsigned int ic_token_ispermission(struct ic_token *token) {
    if (!token) {
        puts("ic_token_ispermission: token was null");
        return 0;
    }

    switch (token->id) {
        case IC_DOLLAR:
        case IC_PERCENT:
        case IC_AMPERSAND:
        case IC_AT:
        case IC_ASTERISK:
        case IC_CARET:
        case IC_PLUS:
        case IC_MINUS:
            return 1;

        default:
            break;
    }

    return 0;
}

void ic_token_print(FILE *fd, struct ic_token *token) {
    char *str = 0;
    if (!token) {
        puts("ic_token_print: token was null");
        return;
    }

    switch (token->id) {
        case IC_IDENTIFIER:
            fprintf(fd, "%.*s", token->u.str.len, token->u.str.string);
            break;
        case IC_LITERAL_UNSIGNED_INTEGER:
            fprintf(fd, "%ldu", token->u.unsigned_integer);
            break;
        case IC_LITERAL_SIGNED_INTEGER:
            fprintf(fd, "%lds", token->u.unsigned_integer);
            break;
        case IC_LITERAL_STRING:
            fprintf(fd, "\"%.*s\"", token->u.str.len, token->u.str.string);
            break;
        case IC_COMMENT:
            fprintf(fd, "#%.*s", token->u.str.len, token->u.str.string);
            break;

        default:
            str = ic_token_get_representation(token);
            if (!str) {
                puts("ic_token_print: call to ic_token_get_representation failed");
                return;
            }

            fputs(str, fd);
            break;
    }
}

void ic_token_id_print_debug(FILE *fd, enum ic_token_id id) {
    switch (id) {
        case IC_IDENTIFIER:
            /* FIXME add payload */
            fputs("IC_IDENTIFIER", fd);
            break;
        case IC_LITERAL_UNSIGNED_INTEGER:
            /* FIXME add payload */
            fputs("IC_LITERAL_UNSIGNED_INTEGER", fd);
            break;
        case IC_LITERAL_SIGNED_INTEGER:
            /* FIXME add payload */
            fputs("IC_LITERAL_SIGNED_INTEGER", fd);
            break;
        case IC_LITERAL_STRING:
            /* FIXME add payload */
            fputs("IC_LITERAL_STRING", fd);
            break;
        case IC_COMMENT:
            /* FIXME add payload */
            fputs("IC_COMMENT", fd);
            break;

        case IC_TRUE:
            fputs("IC_TRUE", fd);
            break;
        case IC_FALSE:
            fputs("IC_TRUE", fd);
            break;

        case IC_NEWLINE:
            fputs("IC_NEWLINE", fd);
            break;
        case IC_WHITESPACE:
            fputs("IC_WHITSPACE", fd);
            break;

        case IC_END:
            fputs("IC_END", fd);
            break;

        case IC_BEGIN:
            fputs("IC_BEGIN", fd);
            break;
        case IC_IF:
            fputs("IC_IF", fd);
            break;
        case IC_ELSIF:
            fputs("IC_ELSIF", fd);
            break;
        case IC_ELSE:
            fputs("IC_ELSE", fd);
            break;
        case IC_RETURN:
            fputs("IC_RETURN", fd);
            break;
        case IC_LET:
            fputs("IC_LET", fd);
            break;
        case IC_FOR:
            fputs("IC_FOR", fd);
            break;
        case IC_IN:
            fputs("IC_IN", fd);
            break;
        case IC_WHILE:
            fputs("IC_WHILE", fd);
            break;
        case IC_MATCH:
            fputs("IC_MATCH", fd);
            break;
        case IC_CASE:
            fputs("IC_CASE", fd);
            break;

        case IC_IMPORT:
            fputs("IC_IMPORT", fd);
            break;
        case IC_BUILTIN:
            fputs("IC_BUILTIN", fd);
            break;
        case IC_OP:
            fputs("IC_OP", fd);
            break;
        case IC_FUNC:
            fputs("IC_FUNC", fd);
            break;
        case IC_TYPE:
            fputs("IC_TYPE", fd);
            break;
        case IC_ENUM:
            fputs("IC_ENUM", fd);
            break;
        case IC_UNION:
            fputs("IC_UNION", fd);
            break;

        case IC_ARROW:
            fputs("IC_ARROW", fd);
            break;
        case IC_EQUAL:
            fputs("IC_EQUAL", fd);
            break;
        case IC_ASSIGN:
            fputs("IC_ASSIGN", fd);
            break;
        case IC_DOUBLECOLON:
            fputs("IC_DOUBLECOLON", fd);
            break;
        case IC_PERIOD:
            fputs("IC_PERIOD", fd);
            break;
        case IC_COMMA:
            fputs("IC_COMMA", fd);
            break;
        case IC_AND:
            fputs("IC_AND", fd);
            break;
        case IC_OR:
            fputs("IC_OR", fd);
            break;

        case IC_DIVIDE:
            fputs("IC_DIVIDE", fd);
            break;

        case IC_DOLLAR:
            fputs("IC_DOLLAR", fd);
            break;
        case IC_PERCENT:
            fputs("IC_PERCENT", fd);
            break;
        case IC_AMPERSAND:
            fputs("IC_AMPERSAND", fd);
            break;
        case IC_AT:
            fputs("IC_AT", fd);
            break;
        case IC_ASTERISK:
            fputs("IC_ASTERISK", fd);
            break;
        case IC_CARET:
            fputs("IC_CARET", fd);
            break;
        case IC_PLUS:
            fputs("IC_PLUS", fd);
            break;
        case IC_MINUS:
            fputs("IC_MINUS", fd);
            break;

        case IC_LRBRACKET:
            fputs("IC_LRBRACKET", fd);
            break;
        case IC_RRBRACKET:
            fputs("IC_RRBRACKET", fd);
            break;

        case IC_LSBRACKET:
            fputs("IC_LSBRACKET", fd);
            break;
        case IC_RSBRACKET:
            fputs("IC_RSBRACKET", fd);
            break;

        case IC_LESSTHAN_EQUAL:
            fputs("IC_LESSTHAN_EQUAL", fd);
            break;
        case IC_GREATERTHAN_EQUAL:
            fputs("IC_GREATERTHAN_EQUAL", fd);
            break;
        case IC_LESSTHAN:
            fputs("IC_LESSTHAN", fd);
            break;
        case IC_GREATERTHAN:
            fputs("IC_GREATERTHAN", fd);
            break;

        default:
            fprintf(fd, "ic_token_id_print_debug: ERROR, unknown token id '%d'\n", id);
            break;
    }
}

void ic_token_print_debug(FILE *fd, struct ic_token *token) {
    if (!token) {
        puts("ic_token_print_debug: token was null");
        return;
    }

    ic_token_id_print_debug(fd, token->id);
}

void ic_token_print_line(FILE *fd, struct ic_token *token) {
    /* number of chars until line ends */
    unsigned int len = 0;

    if (!token) {
        puts("ic_token_print_line: token was null");
        return;
    }

    for (len = 0;; ++len) {
        switch (token->line[len]) {
            case '\0':
            case '\n':
            case '\r':
                goto END;
                break;

            default:
                break;
        }
    }

END:

    fprintf(fd, "%.*s\n", len, token->line);
}

void ic_token_debug(FILE *fd, struct ic_token *token) {
    if (!token) {
        puts("ic_token_debug: token was null");
        return;
    }

    fputs("ic_token_debug:", fd);
    ic_token_print_debug(fd, token);
    fprintf(fd, " found in context: '%.*s'\nline:\n", 10, &(token->line[token->offset]));
    ic_token_print_line(fd, token);
}
