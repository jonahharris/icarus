#ifndef ICARUS_STATEMENT_H
#define ICARUS_STATEMENT_H

#include "body.h"
#include "expression.h"
#include "symbol.h"
#include "pvector.h"

struct ic_stmt;

/* a let statement
 *  let identifier::type = init
 */
struct ic_stmt_let {
    struct ic_symbol identifier;
    struct ic_symbol type;
    struct ic_expr init;
};

/* an if statement
 *  if expr
 *      body
 *  end
 */
struct ic_stmt_if {
    struct ic_expr expr;
    struct ic_body body;
};

enum ic_stmt_type {
    ic_stmt_type_let,
    ic_stmt_type_if,
    ic_stmt_type_expr
};

struct ic_stmt {
    enum ic_stmt_type type;
    union {
        struct ic_stmt_let let;
        struct ic_stmt_if sif;
        /* a statement can just be an expression in
         * void context
         *  foo(bar)
         */
        struct ic_expr expr;
    } u;
};

/* allocate and initialise anew ic_stmt
 * will not initialise union members
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt * ic_stmt_new(enum ic_stmt_type type);

/* initialise an existing ic_stmt
 *
 * returns 0 on success
 * returns 1 on failure
 */
int ic_stmt_init(struct ic_stmt *stmt, enum ic_stmt_type type);

/* get a pointer to the let within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_let * ic_stmt_get_let(struct ic_stmt *stmt);

/* get a pointer to the sif within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_stmt_if * ic_stmt_get_sif(struct ic_stmt *stmt);

/* get a pointer to the expr within
 * will only succeed if ic_stmt is of the correct type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_expr * ic_stmt_get_expr(struct ic_stmt *stmt);

#endif