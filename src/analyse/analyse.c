#include <stdlib.h> /* FIMXE exit */
#include <stdio.h>
#include <string.h> /* strcmp */

#include "../parse/data/field.h"
#include "../data/set.h"
#include "analyse.h"

/* ignored unused parameter */
#pragma GCC diagnostic ignored "-Wunused-parameter"
/* ignored unused variable */
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"


/* takes an ast and performs analysis on it
 * this returns a kludge
 *
 * see kludge->errors for errors
 *
 * returns kludge on success
 * returns 0 on failure
 */
struct ic_kludge * ic_analyse(struct ic_ast *ast){
    /* our offset into various lists */
    unsigned int i = 0;
    /* cached len of various lists */
    unsigned int len = 0;

    /* the current type we are analysing */
    struct ic_type_decl *tdecl = 0;
    /* the current func we are analysing */
    struct ic_func_decl *fdecl = 0;

    /* our mighty kludge */
    struct ic_kludge *kludge = 0;

    /* steps:
     *      create kludge from ast
     *      for each type call ic_analyse_type_decl
     *      for each func call ic_analyse_func_decl
     */

    if( ! ast ){
        puts("ic_analyse: ast null");
        return 0;
    }

    /* create kludge from ast */
    kludge = ic_kludge_new(ast);
    if( ! kludge ){
        puts("ic_analyse: call to ic_kludge_new failed");
        return 0;
    }

    /* for each type call ic_analyse_type_decl */
    len = ic_pvector_length(&(kludge->tdecls));
    for( i=0; i<len; ++i ){
        tdecl = ic_pvector_get(&(kludge->tdecls), i);
        if( ! tdecl ){
            puts("ic_analyse: call to ic_pvector_get failed for tdecl");
            goto ERROR;
        }

        if( ic_analyse_type_decl(kludge, tdecl) ){
            puts("ic_analyse: call to ic_analyse_type_decl failed");
            goto ERROR;
        }
    }

    /* for each func call analyst_func_decl */
    len = ic_pvector_length(&(kludge->fdecls));
    for( i=0; i<len; ++i ){
        fdecl = ic_pvector_get(&(kludge->fdecls), i);
        if( ! fdecl ){
            puts("ic_analyse: call to ic_pvector_get failed for fdecl");
            goto ERROR;
        }

        if( ic_analyse_func_decl(kludge, fdecl) ){
            puts("ic_analyse: call to ic_analyse_func_decl failed");
            goto ERROR;
        }
    }

    return kludge;

ERROR:
    /* destroy kludge
     * free_kludge as allocated with new
     */
    if( ic_kludge_destroy(kludge, 1) ){
        puts("ic_analyse: call to ic_kludge_destroy failed in error case");
    }

    return 0;
}

/* takes a type_decl and performs analysis
 *
 * FIXME need a way of signalling and passing errors
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_analyse_type_decl(struct ic_kludge *kludge, struct ic_type_decl *tdecl){
    /* name of current type we are trying to declare */
    char *this_type = 0;
    /* index into fields */
    unsigned int i = 0;
    /* len of fields */
    unsigned int len = 0;
    /* current field */
    struct ic_field *field = 0;
    /* current name from field */
    char *name = 0;
    /* current type from field */
    struct ic_symbol *type = 0;
    char *type_str = 0;
    /* set of all field names used */
    struct ic_set *set = 0;
    /* for each field this captures the type we resolve it to */
    struct ic_type_decl *field_tdecl = 0;

    if( ! kludge ){
        puts("ic_analyse_type_decl: kludge was null");
        return 1;
    }

    if( ! tdecl ){
        puts("ic_analyse_type_decl: tdecl was null");
        return 1;
    }

    this_type = ic_type_decl_str(tdecl);
    if( ! this_type ){
        puts("ic_analyse_type_decl: for this_type: call to ic_type_decl_str failed");
        return 1;
    }

    set = ic_set_new();
    if( ! set ){
        puts("ic_analyse_type_decl: call to ic_set_new failed");
        return 1;
    }

    /* iterate through each field
     * for each field we check:
     *      the field name is unique within this type decl
     *      the field type exists
     */
    len = ic_pvector_length(&(tdecl->fields));
    for( i=0; i<len; ++i ){
        field = ic_pvector_get(&(tdecl->fields), i);
        if( ! field ){
            puts("ic_analyse_type_decl: call to ic_pvector_get failed");
            goto ERROR;
        }

        /* this is a char* to the inside of symbol
         * so we do not need to worry about free-ing this
         */
        name = ic_symbol_contents(&(field->name));
        if( ! name ){
            puts("ic_analyse_type_decl: call to ic_symbol_contents failed for name");
            goto ERROR;
        }

        /* check name is unique within this type decl
         * ic_set_insert makes a strdup copy
         */
        if( ! ic_set_insert(set, name) ){
            printf("ic_analyse_type_decl: field name '%s' it not unique within type declaration for '%s'\n",
                    name,
                    this_type);
            goto ERROR;
        }

        /* what we are really doing here is:
         *  a) convert type to string representation
         *  b) checking that this is not a self-recursive type
         *  c) check that this field's type exists
         *
         * note that field->type may be in a few states,
         * if field->type.type is one of
         *  ic_type_ref_tdecl
         *  ic_type_ref_builtin
         * then checking the type exists (c) is wasted effort
         * as we already know the type exists
         *
         * at this point however every type should still be
         *  ic_type_ref_symbol
         *
         *  FIXME check / consider this
         */
        type = ic_type_ref_get_symbol(&(field->type));
        if( ! type ){
            puts("ic_analyse_type_decl: call to ic_type_get_symbol failed for type");
            goto ERROR;
        }

        type_str = ic_symbol_contents(type);
        if( ! type_str ){
            puts("ic_analyse_type_decl: call to ic_symbol_contents failed for type");
            goto ERROR;
        }

        /* check that the type used is not the same we are currently trying
         * to declare as this would be an infinitely recursive type
         */
        if( ! strcmp(type_str, this_type) ){
            printf("ic_analyse_type_decl: recursive type detected; '%s' used within declaration of same type '%s'\n",
                    type_str,
                    this_type);
            goto ERROR;
        }

        /* check that type exists */
        field_tdecl = ic_kludge_get_tdecl(kludge, type_str);
        if( ! field_tdecl ){
            printf("ic_analyse_type_decl: type '%s' mentioned in type declaration for '%s' does not exist within this kludge\n",
                    type_str,
                    this_type);
            goto ERROR;
        }

        /* store that type decl on the field (to save lookup costs again later)
         * FIXME are we sure this is safe?
         * if field->type is already a tdecl this will blow up
         */
        if( ic_type_ref_set_tdecl( &(field->type), field_tdecl ) ){
            printf("ic_analyse_type_decl: trying to store tdecl for '%s' on field '%s' of type '%s' failed\n",
                    type_str,
                    name,
                    this_type);
            goto ERROR;
        }
    }

    /* call set destroy
     * free_set as we allocated above with new
     */
    if( ! ic_set_destroy(set, 1) ){
        puts("ic_analyse_type_decl: call to ic_set_destroy failed");
        return 1;
    }

    return 0;

ERROR:
    /* call set destroy
     * free_set as we allocated above with new
     */
    if( ! ic_set_destroy(set, 1) ){
        puts("ic_analyse_type_decl: call to ic_set_destroy failed in error case");
    }

    return 1;
}

/* takes a func_decl and performs analysis
 *
 * FIXME need a way of signalling and passing errors
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_analyse_func_decl(struct ic_kludge *kludge, struct ic_func_decl *fdecl){
    puts("ic_analyse_func_decl: unimplemented");
    return 1;
}

/* takes an expr and returns the inferred type as a symbol
 *
 * FIXME need a way of signalling error and passing errors
 * possible suggestions:
 *  0 -> unrecoverable / internal error
 *  special ic_type_ref * -> syntax error -> ignore this statement, record error, keep going
 *  ic_type_ref * -> actual type inferred
 *
 * examples:
 *  infer 1 -> Int
 *  infer "helo" -> String
 *  infer addone(1) -> addone(Int)->Int -> Int
 *  infer Foo(1 "hello") -> Foo(Int String) -> Foo
 *
 * returns ic_type_ref on success
 * returns 0 on error
 */
struct ic_type_ref * ic_analyse_infer(struct ic_kludge *kludge, struct ic_expr *expr){
    /*
     *  infer 1 -> Int
     *  expr->type == constant
     *  cons = expr->u.cons
     *  cons->type == integer
     *  return integer
     */

    /*
     *  infer "hello" -> String
     *  expr->type == constant
     *  cons = expr->u.cons
     *  cons->type == string
     *  return string
     */

    /*
     *  infer f -> typeof contents of f
     *  expr->type = identifier
     *  id = expr->u.id
     *  name = str(id)
     *  variable = get variable within scope name // FIXME no mechanism
     *  type = get type of variable // FIXME no mechanism
     *  return type
     */

    /*
     *  infer addone(1) -> addone(Int)->Int -> Int
     *  expr->type == func_call
     *  fc = expr->u.fcall
     *  fstr = str(fc) // FIXME ic_func_call_str doesn't exist
     *  fdecl = kludge get fdecl from fstr
     *  tstr = fdecl->ret_type
     *  type = kludge get tdecl from tstr
     *  return type
     */

    /*
     *  infer Foo(1 "hello") -> Foo(Int String) -> Foo
     *  expr->type == func_call
     *  fc = expr->u.fcall
     *  fstr = str(fc) // FIXME ic_func_call_str doesn't exist
     *  fdecl = kludge get fdecl from fstr
     *  tstr = fdecl->ret_type
     *  type = kludge get tdecl from tstr
     *  return type
     */

    puts("ic_analyse_infer: unimplemented");
    exit(1);
}

/* check a statement for validity
 *
 * FIXME need a way of signalling and passing errors
 *
 * examples:
 *  check let f::Foo = Foo(addone(1) "hello")
 *  check d(f)
 *  check print(s)
 *
 * returns 0 for success
 * returns 1 for error
 */
unsigned int ic_analyse_check(struct ic_kludge *kludge, struct ic_stmt *stmt){
    puts("ic_analyse_check: unimplemented");
    exit(1);
}


