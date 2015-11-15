#ifndef ICARUS_TOKEN_H
#define ICARUS_TOKEN_H

#include "../../data/pvector.h"

enum ic_token_id {
    /* the top tokens are given explicit numbers
     * as these are used as indicies into the 'permissions'
     * table
     * DO NOT modify these values without also checking parse/permissions.h
     */
    IC_DOLLAR    = 0,
    IC_PERCENT   = 1,
    IC_AMPERSAND = 2,
    IC_AT        = 3,
    IC_ASTERISK  = 4,
    IC_CARET     = 5,
    IC_PLUS      = 6,
    IC_MINUS     = 7,

    IC_IDENTIFIER, /* no entry in table[], payload */
    IC_LITERAL_INTEGER, /* no entry in table[], payload */
    IC_LITERAL_STRING, /* no entry in table[], payload */
    IC_COMMENT, /* no entry in table[], payload */

    IC_NEWLINE,
    IC_WHITESPACE,

    IC_END,

    IC_IF,
    IC_ELSE,
    IC_RETURN,
    IC_LET,

    IC_IMPORT,
    IC_BUILTIN,
    IC_OP,
    IC_FUNC,
    IC_TYPE,
    IC_ENUM,
    IC_UNION,

    IC_ARROW,
    IC_EQUAL, /* == */
    IC_ASSIGN, /* = */
    IC_DOUBLECOLON, /* :: double colon */
    IC_PERIOD,
    IC_COMMA,

    IC_DIVIDE,

    IC_LRBRACKET, /* ( left round bracket */
    IC_RRBRACKET, /* ) right round backet */

    IC_TOKEN_LEN /* final token */
};

struct ic_token {
    /* this token type */
    enum ic_token_id id;

    /* location of token in source */
    /* source line */
    char *line;
    /* offset into line */
    unsigned int offset;
    /* filename */
    char *file;
    /* line number in file */
    unsigned int line_num;

    /* token data for literals */
    union {
        struct {
            char *string;
            unsigned int len;
        } str;
        long int integer;
    } u;
};

/* alloc and init a new token
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_token * ic_token_new(enum ic_token_id id, char *line, unsigned int offset, char *file, unsigned int line_num);

/* init an existing token
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_token_init(struct ic_token *token, enum ic_token_id id, char *line, unsigned int offset, char *file, unsigned int line_num);

/* set string data on token
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_token_set_string(struct ic_token *token, char *string, unsigned int len);

/* get string representation of token
 *
 * returns * on success
 * returns 0 on failure
 */
char * ic_token_get_representation(struct ic_token *token);

/* get string data on token
 *
 * returns * on success
 * returns 0 on failure
 */
char * ic_token_get_string(struct ic_token *token);

/* get string length on token
 *
 * returns length on success
 * returns 0 on failure
 */
unsigned int ic_token_get_string_length(struct ic_token *token);

/* set integer data on token
 *
 * returns 1 on success
 * returns 0 on failure
 */
int ic_token_set_integer(struct ic_token *token, int integer);

/* get integer data on token
 *
 * returns integer on success
 * returns 0 on failure
 */
int ic_token_get_integer(struct ic_token *token);

/* destroy
 *
 * will only free token if free_token is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_token_destroy(struct ic_token *token, unsigned int free_token);

/* check if token is an operator
 *
 * returns 1 if it is an operator
 * returns 0 if not
 */
unsigned int ic_token_isoperator(struct ic_token *token);

/* check if token is a permission
 *
 * returns 1 if it is a permission
 * returns 0 if not
 */
unsigned int ic_token_ispermission(struct ic_token *token);

void ic_token_print(struct ic_token *token);

void ic_token_id_print_debug(enum ic_token_id id);

void ic_token_print_debug(struct ic_token *token);

void ic_token_print_line(struct ic_token *token);

void ic_token_debug(struct ic_token *token);

#endif
