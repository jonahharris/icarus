#ifndef ICARUS_DECL_H
#define ICARUS_DECL_H

#include "../../data/dict.h"
#include "../../data/pvector.h"
#include "../../data/symbol.h"
#include "body.h"
#include "field.h"
#include "type_param.h"

/* predeclare to allow usage of *ic_transform_body
 * within decl_func
 */
struct ic_transform_body;

struct ic_decl_func {
    /* fn name(args...) -> ret_type
     *      body ...
     * end
     *
     * fn name(args...)
     *      body ...
     * end
     *
     */
    struct ic_symbol name;

    /* a pointer vector of type_param(s) */
    struct ic_pvector type_params;

    /* a pointer vector of field(s) */
    struct ic_pvector args;

    /* return type is optional
     *  0    -> void
     *  else -> symbol for type
     */
    struct ic_symbol *ret_type;

    /* body of parse time stmt */
    struct ic_body body;

    /* TIR body
     * body constructed at transform time for consumption
     * by backend
     */
    struct ic_transform_body *tbody;

    /* call signature
     * foo(Int,Int)
     */
    struct ic_string sig_call;

    /* full signature
     * foo(Int,Int) -> Int
     */
    struct ic_string sig_full;

    /* mangled signature
     * foo(Int,Int) -> Int => i_foo_a_Int_Int
     */
    struct ic_string sig_mangled;

    /* 1 for builtin, 0 for not */
    unsigned int builtin;
};

/* allocate and initialise a new decl_func
 *
 * returns new decl_func on success
 * returns 0 on failure
 */
struct ic_decl_func *ic_decl_func_new(char *name, unsigned int name_len);

/* initialise an existing decl_func
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_init(struct ic_decl_func *fdecl, char *name, unsigned int name_len);

/* calls destroy on every element within
 *
 * this will only free the fdecl if `free_fdecl` is truthy
 *
 * the caller must determine if it is appropriate
 * or not to call free(decl)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_destroy(struct ic_decl_func *fdecl, unsigned int free_fdecl);

/* add new type_param to decl_func
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_type_params_add(struct ic_decl_func *fdecl, struct ic_type_param *tparam);

/* get length of type_params
 *
 * returns len on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_type_params_length(struct ic_decl_func *fdecl);

/* get type_param at i
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_param *ic_decl_func_type_params_get(struct ic_decl_func *fdecl, unsigned int i);

/* add new arg field to decl_func
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_args_add(struct ic_decl_func *fdecl, struct ic_field *field);

/* get length of args
 *
 * returns len on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_args_length(struct ic_decl_func *fdecl);

/* get arg at i
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_field *ic_decl_func_args_get(struct ic_decl_func *fdecl, unsigned int i);

/* set return type
 *
 * this function will fail if the return type is already set
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_set_return(struct ic_decl_func *fdecl, char *type, unsigned int type_len);

/* add new stmt to the body
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_add_stmt(struct ic_decl_func *fdecl, struct ic_stmt *stmt);

/* mark this fdecl as being a builtin
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_mark_builtin(struct ic_decl_func *fdecl);

/* test if builtin
 *
 * returns 1 if builtin
 * returns 0 otherwise
 */
unsigned int ic_decl_func_isbuiltin(struct ic_decl_func *fdecl);

/* print decl_func
 * this will print a reproduction of the function from the ast
 */
void ic_decl_func_print(FILE *fd, struct ic_decl_func *fdecl, unsigned int *indent_level);
/* print decl_func header
 * this will print a reproduction of the function header from the ast
 */
void ic_decl_func_print_header(FILE *fd, struct ic_decl_func *fdecl, unsigned int *indent_level);
/* print decl_func body
 * this will print a reproduction of the function body from the ast
 */
void ic_decl_func_print_body(FILE *fd, struct ic_decl_func *fdecl, unsigned int *indent_level);

/* return a string representation of this function's call signature
 *
 * for a function signature
 *      fn foo(a::Int,b::Int) -> Int
 *
 * this function will return
 *      foo(Int,Int)
 *
 * the char* returned is a string stored within fdecl,
 * this means the caller must not free or mutate this string
 *
 * returns char* on success
 * returns 0 on failure
 */
char *ic_decl_func_sig_call(struct ic_decl_func *fdecl);

/* return a string representation of this function full signature
 *
 * for a function signature
 *      fn foo(a::Int,b::Int) -> Int
 *
 * this function will return
 *      foo(Int,Int) -> Int
 *
 * the char* returned is a string stored within fdecl,
 * this means the caller must not free or mutate this string
 *
 * returns char* on success
 * returns 0 on failure
 */
char *ic_decl_func_sig_full(struct ic_decl_func *fdecl);

/* return a mangled representation of this function full signature
 *
 * for a function signature
 *      fn foo(a::Int,b::Int) -> Int
 *
 * this function will return
 *      i_foo_a_Int_Int
 *
 * the char* returned is a string stored within fdecl,
 * this means the caller must not free or mutate this string
 *
 * returns char* on success
 * returns 0 on failure
 */
char *ic_decl_func_sig_mangled(struct ic_decl_func *fdecl);

/* check if this function returns void
 *
 * returns boolean on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_is_void(struct ic_decl_func *fdecl);

/* a func declaration is a symbol and then a collection of fields
 *  type Foo
 *   a::Int
 *   b::String
 *  end
 *
 * gives the symbol 'Foo' and the
 * fields (a, Int) and (b, String)
 */
struct ic_decl_type_struct {
    struct ic_symbol name;

    /* a pointer vector of type_param(s) */
    struct ic_pvector type_params;

    /* a pointer vector of field(s) */
    struct ic_pvector fields;

    /* a map from (char *) -> decl_type *
     * used for looking up the type of a field by name
     * this is filled in during analyse time by
     *  ic_analyse_decl_type
     */
    struct ic_dict field_dict;

    /* full mangled signature
     *
     * for the type
     *  type Foo
     *    i::Sint
     *    u::Uint
     *    b::Bar
     *  end
     *
     * this should generate the function
     *
     *  struct Foo * ic_Foo_a_Sint_Uint_Bar(Sint *i, Uint *u, Bar *b);
     */
    struct ic_string sig_mangled_full;

    /* FIXME TODO these bool fields are gross
     * should at least be an enum
     * maybe move up to decl_type level
     */

    /* 1 if this is the void type
     * 0 if this is not the void type
     */
    unsigned int isvoid;

    /* 1 if this is the bool type
     * 0 if this is not the bool type
     */
    unsigned int isbool;

    /* 1 if this is the string type
     * 0 if this is not the string type
     */
    unsigned int isstring;

    /* 1 if this is the uint type
     * 0 if this is not the uint type
     */
    unsigned int isuint;

    /* 1 if this is the sint type
     * 0 if this is not the sint type
     */
    unsigned int issint;
};

/* allocate and return a new decl_type
 * only needs name and len
 * will also allocate an empty pvector for fields
 *
 * returns new ic_field * on success
 * returns 0 on failure
 */
struct ic_decl_type_struct *ic_decl_type_struct_new(char *name_src, unsigned int name_len);

/* initialise an existing decl_type
 * only needs name and len
 * will also allocate an empty pvector for fields
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_init(struct ic_decl_type_struct *tdecl, char *name_src, unsigned int name_len);

/* calls destroy on every element within
 *
 * this will only free the tdecl if `free_tdecl` is truthy
 *
 * the caller must determine if it is appropriate
 * or not to call free(decl)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_destroy(struct ic_decl_type_struct *tdecl, unsigned int free_tdecl);

/* get symbol of name for this type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_struct_get_name(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the void type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_void(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the bool type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_bool(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the string type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_string(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the uint type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_uint(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the sint type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_sint(struct ic_decl_type_struct *tdecl);

/* add a new field to types list of fields
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_add_field(struct ic_decl_type_struct *tdecl, struct ic_field *field);

/* test if bool
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_isbool(struct ic_decl_type_struct *tdecl);

/* test if string
 *
 * returns 1 if string
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_isstring(struct ic_decl_type_struct *tdecl);

/* test if uint
 *
 * returns 1 if uint
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_isuint(struct ic_decl_type_struct *tdecl);

/* test if sint
 *
 * returns 1 if sint
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_issint(struct ic_decl_type_struct *tdecl);

/* test if void
 *
 * returns 1 if void
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_isvoid(struct ic_decl_type_struct *tdecl);

/* print the decl_type to provided fd */
void ic_decl_type_struct_print(FILE *fd, struct ic_decl_type_struct *tdecl, unsigned int *indent_level);
void ic_decl_type_struct_print_header(FILE *fd, struct ic_decl_type_struct *tdecl, unsigned int *indent_level);
void ic_decl_type_struct_print_body(FILE *fd, struct ic_decl_type_struct *tdecl, unsigned int *indent_level);

/* print debug information for this type */
void ic_decl_type_struct_print_debug(FILE *fd, struct ic_decl_type_struct *tdecl);

/* return the symbol name of this types
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_struct_name(struct ic_decl_type_struct *tdecl);

/* get the char * contents of the name
 *
 * returns char * on success
 * returns 0 on failure
 */
char *ic_decl_type_struct_str(struct ic_decl_type_struct *tdecl);

/* return a mangled representation of this function full signature
 *
 * the char* returned is a string stored within tdecl,
 * this means the caller must not free or mutate this string
 *
 * for the type
 *  type Foo
 *    i::Sint
 *    u::Uint
 *    b::Bar
 *  end
 *
 * this should generate the function
 *
 *  struct Foo * ic_Foo_a_Sint_Uint_Bar(Sint *i, Uint *u, Bar *b);
 *
 * returns char* on success
 * returns 0 on failure
 */
char *ic_decl_type_struct_sig_mangled_full(struct ic_decl_type_struct *tdecl);

/* add new type_param to decl_struct
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_type_params_add(struct ic_decl_type_struct *tdecl, struct ic_type_param *tparam);

/* get length of type_params
 *
 * returns len on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_type_params_length(struct ic_decl_type_struct *tdecl);

/* get type_param at i
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_param *ic_decl_type_struct_type_params_get(struct ic_decl_type_struct *tdecl, unsigned int i);

/* get number of fields
 *
 * returns number on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_field_length(struct ic_decl_type_struct *tdecl);

/* get field by number
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_field *ic_decl_type_struct_field_get(struct ic_decl_type_struct *tdecl, unsigned int field_number);

/* get the type of a field by name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_type_struct_get_field_type(struct ic_decl_type_struct *tdecl, char *field_name);

/* get the offset of a field by name
 *
 * TODO FIXME no way to indicate failure
 *
 * returns n on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_get_field_offset(struct ic_decl_type_struct *tdecl, char *field_name);

/* add field to field_dict
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_add_field_type(struct ic_decl_type_struct *tdecl, char *field_name, struct ic_decl_type *type);

/* compare 2 types for equality
 *
 * returns 1 if equal
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_equal(struct ic_decl_type_struct *a, struct ic_decl_type_struct *b);

struct ic_decl_type_union {
    struct ic_symbol name;

    /* a pointer vector of type_param(s) */
    struct ic_pvector type_params;

    /* a pointer vector of field(s) */
    struct ic_pvector fields;

    /* a map from (char *) -> type *
     * used for looking up the type of a field by name
     * this is filled in during analyse time by
     *  ic_analyse_decl_union
     */
    struct ic_dict field_dict;
};

/* allocate and return a new decl_union
 * only needs name and len
 * will also allocate an empty pvector for fields
 *
 * returns new ic_field * on success
 * returns 0 on failure
 */
struct ic_decl_type_union *ic_decl_type_union_new(char *name_src, unsigned int name_len);

/* initialise an existing decl_union
 * only needs name and len
 * will also allocate an empty pvector for fields
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_init(struct ic_decl_type_union *udecl, char *name_src, unsigned int name_len);

/* calls destroy on every element within
 *
 * this will only free the udecl if `free_udecl` is truthy
 *
 * the caller must determine if it is appropriate
 * or not to call free(decl)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_destroy(struct ic_decl_type_union *udecl, unsigned int free_udecl);

/* get symbol of name for this type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_union_get_name(struct ic_decl_type_union *udecl);

/* add a new field to types list of fields
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_add_field(struct ic_decl_type_union *udecl, struct ic_field *field);

/* print the decl_union to provided fd */
void ic_decl_type_union_print(FILE *fd, struct ic_decl_type_union *udecl, unsigned int *indent_level);
void ic_decl_type_union_print_header(FILE *fd, struct ic_decl_type_union *udecl, unsigned int *indent_level);
void ic_decl_type_union_print_body(FILE *fd, struct ic_decl_type_union *udecl, unsigned int *indent_level);

/* print debug information for this type */
void ic_decl_type_union_print_debug(FILE *fd, struct ic_decl_type_union *udecl);

/* return the symbol name of this types
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_union_name(struct ic_decl_type_union *tdecl);

/* get the char * contents of the name
 *
 * returns char * on success
 * returns 0 on failure
 */
char *ic_decl_type_union_str(struct ic_decl_type_union *tdecl);

/* add new type_param to decl_union
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_type_params_add(struct ic_decl_type_union *udecl, struct ic_type_param *tparam);

/* get length of type_params
 *
 * returns len on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_type_params_length(struct ic_decl_type_union *udecl);

/* get type_param at i
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_param *ic_decl_type_union_type_params_get(struct ic_decl_type_union *udecl, unsigned int i);

/* get number of fields
 *
 * returns number on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_field_length(struct ic_decl_type_union *udecl);

/* get field by number
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_field *ic_decl_type_union_field_get(struct ic_decl_type_union *udecl, unsigned int field_number);

/* get the type of a field by name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_type_union_get_field_type(struct ic_decl_type_union *udecl, char *field_name);

/* get the offset of a field by name
 *
 * TODO FIXME no way to indicate failure
 *
 * returns n on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_get_field_offset(struct ic_decl_type_union *tdecl, char *field_name);

/* add field to field_dict
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_add_field_type(struct ic_decl_type_union *udecl, char *field_name, struct ic_decl_type *type);

/* compare types for equality
 *
 * returns 1 if equals
 * returns 0 otherwise
 */
unsigned int ic_decl_type_union_equal(struct ic_decl_type_union *a, struct ic_decl_type_union *b);

enum ic_decl_type_tag {
    ic_decl_type_tag_struct,
    ic_decl_type_tag_union,
};

/* Note: decl_type lacks a _new and _init as it only makes sense
 * in the context of one of its subtypes
 */
struct ic_decl_type {
    enum ic_decl_type_tag tag;
    union {
        struct ic_decl_type_struct tstruct;
        struct ic_decl_type_union tunion;
    } u;
    /* 1 for builtin, 0 for not */
    unsigned int builtin;
};

/* allocate and initialise a new ic_decl_type as a struct
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_type_new_struct(char *name_src, unsigned int name_len);

/* initialise an existing ic_decl_type as a struct
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_init_struct(struct ic_decl_type *tdecl, char *name_src, unsigned int name_len);

/* allocate and initialise a new ic_decl_type as a union
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_type_new_union(char *name_src, unsigned int name_len);

/* initialise an existing ic_decl_type as a union
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_init_union(struct ic_decl_type *tdecl, char *name_src, unsigned int name_len);

/* destroy an ic_decl_type
 *
 * will only free tdecl of free_tdectl is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_destroy(struct ic_decl_type *tdecl, unsigned int free_tdecl);

/* mark this tdecl as being a builtin
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_mark_builtin(struct ic_decl_type *tdecl);

/* test if builtin
 *
 * returns 1 if builtin
 * returns 0 otherwise
 */
unsigned int ic_decl_type_isbuiltin(struct ic_decl_type *tdecl);

/* get symbol of name for this type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_get_name(struct ic_decl_type *tdecl);

/* fetch a pointer to the internal decl_type_struct
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type_struct *ic_decl_type_get_struct(struct ic_decl_type *tdecl);

/* fetch a pointer to the internal decl_type_union
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type_union *ic_decl_type_get_union(struct ic_decl_type *tdecl);

/* return the symbol name of this type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_name(struct ic_decl_type *tdecl);

/* get the char * contents of the name
 *
 * returns char * on success
 * returns 0 on failure
 */
char *ic_decl_type_str(struct ic_decl_type *tdecl);

/* add new type_param to decl
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_type_params_add(struct ic_decl_type *tdecl, struct ic_type_param *tparam);

/* get length of type_params
 *
 * returns len on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_type_params_length(struct ic_decl_type *tdecl);

/* get type_param at i
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_param *ic_decl_type_type_params_get(struct ic_decl_type *tdecl, unsigned int i);

/* add a new field to types list of fields
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_add_field(struct ic_decl_type *tdecl, struct ic_field *field);

/* get number of fields
 *
 * returns number on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_field_length(struct ic_decl_type *tdecl);

/* get field by number
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_field *ic_decl_type_field_get(struct ic_decl_type *tdecl, unsigned int field_number);

/* get the type of a field by name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_type_get_field_type(struct ic_decl_type *tdecl, char *field_name);

/* get the offset of a field by name
 *
 * TODO FIXME no way to indicate failure
 *
 * returns n on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_get_field_offset(struct ic_decl_type *tdecl, char *field_name);

/* add field to field_dict
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_add_field_type(struct ic_decl_type *tdecl, char *field_name, struct ic_decl_type *type);

/* the full signature for the constructor of this type
 *
 * returns * on success
 * returns 0 on failure
 */
char *ic_decl_type_sig_mangled_full(struct ic_decl_type *tdecl);

/* test if bool
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_isbool(struct ic_decl_type *tdecl);

/* test if string
 *
 * returns 1 if string
 * returns 0 otherwise
 */
unsigned int ic_decl_type_isstring(struct ic_decl_type *tdecl);

/* test if uint
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_isuint(struct ic_decl_type *tdecl);

/* test if sint
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_issint(struct ic_decl_type *tdecl);

/* test if void
 *
 * returns 1 if void
 * returns 0 otherwise
 */
unsigned int ic_decl_type_isvoid(struct ic_decl_type *tdecl);

/* compare types for equality
 *
 * returns 1 if equals
 * returns 0 otherwise
 */
unsigned int ic_decl_type_equal(struct ic_decl_type *a, struct ic_decl_type *b);

/* print the decl_type to provided fd */
void ic_decl_type_print(FILE *fd, struct ic_decl_type *tdecl, unsigned int *indent_level);
void ic_decl_type_print_header(FILE *fd, struct ic_decl_type *tdecl, unsigned int *indent_level);
void ic_decl_type_print_body(FILE *fd, struct ic_decl_type *tdecl, unsigned int *indent_level);

/* print debug information for this type */
void ic_decl_type_print_debug(FILE *fd, struct ic_decl_type *tdecl);

/* an op decl is only a mapping of a symbol (say '+')
 * to another (say 'plus')
 */
struct ic_decl_op {
    struct ic_symbol from;
    struct ic_symbol to;
};

/* allocate and return a new decl_op
 *
 * returns new ic_field * on success
 * returns 0 on failure
 */
struct ic_decl_op *ic_decl_op_new(char *from_src, unsigned int from_len, char *to_src, unsigned int to_len);

/* initialise an existing decl_op
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_op_init(struct ic_decl_op *op, char *from_src, unsigned int from_len, char *to_src, unsigned int to_len);

/* calls destroy on every element within
 *
 * this will only free the op if `free_op` is truthy
 *
 * the caller must determine if it is appropriate
 * or not to call free(decl)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_op_destroy(struct ic_decl_op *op, unsigned int free_op);

/* print the decl_op to provided fd */
void ic_decl_op_print(FILE *fd, struct ic_decl_op *op, unsigned int *indent_level);

/* FIXME TODO should this level be aware of builtin? */
enum ic_decl_tag {
    ic_decl_tag_func,
    ic_decl_tag_type,
    ic_decl_tag_builtin_func,
    ic_decl_tag_builtin_type,
    ic_decl_tag_builtin_op
};

struct ic_decl {
    enum ic_decl_tag tag;
    union {
        struct ic_decl_func fdecl;
        struct ic_decl_type tdecl;
        struct ic_decl_op op;
    } u;
};

/* allocate and initialise a new ic_decl
 *
 * NOTE: this call will NOT initialise the contents of the union
 * this is left up to the caller
 *
 * returns new ic_decl on success
 * returns 0 on failure
 */
struct ic_decl *ic_decl_new(enum ic_decl_tag tag);

/* initialise an existing ic_decl
 *
 * NOTE: this call will NOT initialise the contents of the union
 * this is left up to the caller
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_init(struct ic_decl *decl, enum ic_decl_tag tag);

/* calls destroy on every element within
 *
 * this will only free the decl if `free_decl` is truthy
 *
 * the caller must determine if it is appropriate
 * or not to call free(decl)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_destroy(struct ic_decl *decl, unsigned int free_decl);

/* returns pointer to ic_decl_func element
 * this function will only success if the decl is of type decl_func
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_decl_func *ic_decl_get_fdecl(struct ic_decl *decl);

/* returns pointer to ic_decl_type element
 * this function will only success if the decl is of type decl_type
 *
 * returns pointer on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_get_tdecl(struct ic_decl *decl);

/* returns pointer to ic_decl_op element
* this function will only success if the decl is of type decl_op
*
* returns pointer on success
* returns 0 on failure
*/
struct ic_decl_op *ic_decl_get_op(struct ic_decl *decl);

/* mark this decl as being a builtin
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_mark_builtin(struct ic_decl *decl);

/* print contents of ic_decl */
void ic_decl_print(FILE *fd, struct ic_decl *decl, unsigned int *indent_level);

#endif
