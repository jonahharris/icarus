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

    /* boolean representing if this func_decl is a non-instantiated generic function
     * fn id[T](t::T) -> T return t end
     * is a non-instantiated generic function
     *
     * if we then call this `id[Signed](6s)` we will trigger instantiation
     *
     * for a non-generic function this will be true (1)
     * for a generic instantiated function this will be true (1)
     * otherwise this will be false (0)
     */
    unsigned int is_instantiated;

    /* a pointer vector of field(s) */
    struct ic_pvector args;

    /* return type */
    struct ic_type_ref ret_type;

    /* body of parse time stmt */
    struct ic_body body;

    /* TIR body
     * body constructed at transform time for consumption
     * by backend
     */
    struct ic_transform_body *tbody;

    /* mangled_name
     * generated by ic_parse_helper_mangled_name
     */
    struct ic_symbol *mangled_name;

    /* full_name (and call signature)
     * generated by ic_parse_helper_full_name
     */
    struct ic_symbol *full_name;

    /* full signature
     * foo(Int,Int) -> Int
     */
    struct ic_symbol *full_signature;

    /* string representation of function decl with param and arg masking
     * e.g. Foo[A,B](a::A, b::B, i::Signed) becomes Foo[_,_](_,_,_)
     *
     * can be generated without arg analyis, generated by
     * ic_parse_helper_generic_name
     */
    struct ic_symbol *generic_name;

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

/* deep-copy this fdecl
 *
 * returns pointer to new copy
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_func *ic_decl_func_deep_copy(struct ic_decl_func *fdecl);

/* deep-copy this fdecl embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_deep_copy_embedded(struct ic_decl_func *from, struct ic_decl_func *to);

/* get is_instantiated
 *
 * for a non-generic function this will be true (1)
 * for a generic instantiated function this will be true (1)
 * otherwise this will be false (0)
 */
unsigned int ic_decl_func_is_instantiated(struct ic_decl_func *fdecl);

/* set is_instantiated to true (1)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_mark_instantiated(struct ic_decl_func *fdecl);

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

/* get return type_ref
 *
 * returns 1 on success
 * returns 0 on failure
 */
struct ic_type_ref * ic_decl_func_get_return(struct ic_decl_func *fdecl);

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
 * for a function signature (instantiated generic)
 *      fn bar[A::Foo,B::bar](a::A, b::B) -> A
 *
 * this function will return
 *      bar[Foo,Bar](Foo,Bar)
 *
 * the returned symbol is owned by fdecl,
 * this means the caller must not free or mutate this string
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_func_full_name(struct ic_decl_func *fdecl);

/* return a mangled representation of this function full signature
 *
 * for a function signature
 *      fn foo(a::Int,b::Int) -> Int
 *
 * this function will return
 *      foo_a_Int_Int_b
 *
 * the returned symbol is owned by this fdecl,
 * this means the caller must not free or mutate this string
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_func_mangled_name(struct ic_decl_func *fdecl);

/* return a string representation of this function full signature
 *
 * for a function signature
 *      fn foo(a::Int,b::Int) -> Int
 *
 * this function will return
 *      foo(Int,Int) -> Int
 *
 * the returned symbol is owned by this fdecl
 * this means the caller must not free or mutate this string
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_func_full_signature(struct ic_decl_func *fdecl);

/* return a masked representation of this function's call signature
 *
 * string representation of function decl with param and arg masking
 * e.g. Foo[A,B](a::A, b::B, i::Signed) becomes Foo[_,_](_,_,_)
 *
 * Must match ic_analyse_fcall_str_generic
 *
 * can be generated without arg analyis
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_func_generic_name(struct ic_decl_func *fdecl);

/* check if this function returns Unit
 *
 * returns boolean on success
 * returns 0 on failure
 */
unsigned int ic_decl_func_is_unit(struct ic_decl_func *fdecl);

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

    /* boolean representing if this func_decl is a non-instantiated generic type
     *
     * for a non-generic type this will be true (1)
     * for a generic instantiated type this will be true (1)
     * otherwise this will be false (0)
     */
    unsigned int is_instantiated;

    /* a pointer vector of field(s) */
    struct ic_pvector fields;

    /* a map from (char *) -> decl_type *
     * used for looking up the type of a field by name
     * this is filled in during analyse time by
     *  ic_analyse_decl_type
     */
    struct ic_dict field_dict;

    /* name generated by ic_parse_helper_mangled_name
     * owned by this struct
     */
    struct ic_symbol *mangled_name;

    /* name generated by ic_parse_helper_full_name
     * owned by this struct
     */
    struct ic_symbol *full_name;

    /* name generated by ic_parse_helper_generic_name
     * owned by this struct
     */
    struct ic_symbol *generic_name;

    /* FIXME TODO these bool fields are gross
     * should at least be an enum
     * maybe move up to decl_type level
     */

    /* 1 if this is the unit type
     * 0 if this is not the unit type
     */
    unsigned int isunit;

    /* 1 if this is the bool type
     * 0 if this is not the bool type
     */
    unsigned int is_bool;

    /* 1 if this is the string type
     * 0 if this is not the string type
     */
    unsigned int is_string;

    /* 1 if this is the uint type
     * 0 if this is not the uint type
     */
    unsigned int is_unsigned_integer;

    /* 1 if this is the sint type
     * 0 if this is not the sint type
     */
    unsigned int is_signed_integer;
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

/* deep-copy this tdecl
 *
 * returns pointer to new copy
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type_struct *ic_decl_type_struct_deep_copy(struct ic_decl_type_struct *tdecl);

/* deep-copy this tdecl embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_deep_copy_embedded(struct ic_decl_type_struct *from, struct ic_decl_type_struct *to);

/* get is_instantiated
 *
 * for a non-generic type this will be true (1)
 * for a generic instantiated type this will be true (1)
 * otherwise this will be false (0)
 */
unsigned int ic_decl_type_struct_is_instantiated(struct ic_decl_type_struct *tdecl);

/* set is_instantiated to true (1)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_instantiated(struct ic_decl_type_struct *tdecl);

/* get symbol of name for this type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_struct_get_name(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the unit type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_unit(struct ic_decl_type_struct *tdecl);

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
unsigned int ic_decl_type_struct_mark_unsigned(struct ic_decl_type_struct *tdecl);

/* mark the supplied decl as the sint type
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_struct_mark_signed(struct ic_decl_type_struct *tdecl);

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
unsigned int ic_decl_type_struct_is_bool(struct ic_decl_type_struct *tdecl);

/* test if string
 *
 * returns 1 if string
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_is_string(struct ic_decl_type_struct *tdecl);

/* test if uint
 *
 * returns 1 if uint
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_is_unsigned_integer(struct ic_decl_type_struct *tdecl);

/* test if sint
 *
 * returns 1 if sint
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_is_signed_integer(struct ic_decl_type_struct *tdecl);

/* test if unit
 *
 * returns 1 if unit
 * returns 0 otherwise
 */
unsigned int ic_decl_type_struct_isunit(struct ic_decl_type_struct *tdecl);

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

/* return mangled_name of this type
 * this symbol remains owned by this struct
 *
 * generated by ic_parse_helper_mangled_name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_struct_mangled_name(struct ic_decl_type_struct *tstruct);

/* return full_name of this type
 * this symbol remains owned by this struct
 *
 * generated by ic_parse_helper_full_name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_struct_full_name(struct ic_decl_type_struct *tstruct);

/* return a masked representation of this type
 *
 * string representation of type decl with param masking
 * e.g. Foo[A,B] becomes Foo[_,_]
 *
 * can be generated without arg analyis
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_struct_generic_name(struct ic_decl_type_struct *tstruct);

struct ic_decl_type_union {
    struct ic_symbol name;

    /* a pointer vector of type_param(s) */
    struct ic_pvector type_params;

    /* boolean representing if this func_decl is a non-instantiated generic type
     *
     * for a non-generic type this will be true (1)
     * for a generic instantiated type this will be true (1)
     * otherwise this will be false (0)
     */
    unsigned int is_instantiated;

    /* a pointer vector of field(s) */
    struct ic_pvector fields;

    /* a map from (char *) -> type *
     * used for looking up the type of a field by name
     * this is filled in during analyse time by
     *  ic_analyse_decl_union
     */
    struct ic_dict field_dict;

    /* name generated by ic_parse_helper_mangled_name
     * owned by this struct
     */
    struct ic_symbol *mangled_name;

    /* name generated by ic_parse_helper_full_name
     * owned by this struct
     */
    struct ic_symbol *full_name;

    /* name generated by ic_parse_helper_generic_name
     * owned by this struct
     */
    struct ic_symbol *generic_name;
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

/* deep-copy this tdecl
 *
 * returns pointer to new copy
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type_union *ic_decl_type_union_deep_copy(struct ic_decl_type_union *tdecl);

/* deep-copy this tdecl embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_deep_copy_embedded(struct ic_decl_type_union *from, struct ic_decl_type_union *to);

/* get is_instantiated
 *
 * for a non-generic type this will be true (1)
 * for a generic instantiated type this will be true (1)
 * otherwise this will be false (0)
 */
unsigned int ic_decl_type_union_is_instantiated(struct ic_decl_type_union *udecl);

/* set is_instantiated to true (1)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_union_mark_instantiated(struct ic_decl_type_union *udecl);

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

/* return mangled_name of this type
 * this symbol remains owned by this struct
 *
 * generated by ic_parse_helper_mangled_name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_union_mangled_name(struct ic_decl_type_union *tunion);

/* return full_name of this type
 * this symbol remains owned by this struct
 *
 * generated by ic_parse_helper_full_name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_union_full_name(struct ic_decl_type_union *tunion);

/* return a masked representation of this type
 *
 * string representation of type decl with param masking
 * e.g. Foo[A,B] becomes Foo[_,_]
 *
 * can be generated without arg analyis
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_union_generic_name(struct ic_decl_type_union *tunion);


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

/* deep-copy this tdecl
 *
 * returns pointer to new copy
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_decl_type_deep_copy(struct ic_decl_type *tdecl);

/* deep-copy this tdecl embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_deep_copy_embedded(struct ic_decl_type *from, struct ic_decl_type *to);

/* return a new type_ref that refers to this type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_ref *ic_decl_type_make_type_ref(struct ic_decl_type *tdecl);

/* get is_instantiated
 *
 * for a non-generic type this will be true (1)
 * for a generic instantiated type this will be true (1)
 * otherwise this will be false (0)
 */
unsigned int ic_decl_type_is_instantiated(struct ic_decl_type *tdecl);

/* set is_instantiated to true (1)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_type_mark_instantiated(struct ic_decl_type *tdecl);

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

/* test if bool
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_is_bool(struct ic_decl_type *tdecl);

/* test if string
 *
 * returns 1 if string
 * returns 0 otherwise
 */
unsigned int ic_decl_type_is_string(struct ic_decl_type *tdecl);

/* test if uint
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_is_unsigned_integer(struct ic_decl_type *tdecl);

/* test if sint
 *
 * returns 1 if bool
 * returns 0 otherwise
 */
unsigned int ic_decl_type_is_signed_integer(struct ic_decl_type *tdecl);

/* test if unit
 *
 * returns 1 if unit
 * returns 0 otherwise
 */
unsigned int ic_decl_type_isunit(struct ic_decl_type *tdecl);

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

/* return mangled_name of this type
 * this symbol remains owned by this struct
 *
 * generated by ic_parse_helper_mangled_name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_mangled_name(struct ic_decl_type *tdecl);

/* return full_name of this type
 * this symbol remains owned by this struct
 *
 * generated by ic_parse_helper_full_name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_full_name(struct ic_decl_type *tdecl);

/* return a masked representation of this type
 *
 * string representation of type decl with param masking
 * e.g. Foo[A,B] becomes Foo[_,_]
 *
 * can be generated without arg analyis
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_decl_type_generic_name(struct ic_decl_type *tdecl);


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

/* deep-copy this op
 *
 * returns pointer to new copy
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl_op *ic_decl_op_deep_copy(struct ic_decl_op *op);

/* deep-copy this op embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_op_deep_copy_embedded(struct ic_decl_op *from, struct ic_decl_op *to);

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

/* deep-copy this decl
 *
 * returns pointer to new copy
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_decl *ic_decl_deep_copy(struct ic_decl *decl);

/* deep-copy this tdecl embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_deep_copy_embedded(struct ic_decl *from, struct ic_decl *to);

/* get is_instantiated
 *
 * for a non-generic function/type this will be true (1)
 * for a generic instantiated function/type this will be true (1)
 * otherwise this will be false (0)
 */
unsigned int ic_decl_is_instantiated(struct ic_decl *decl);

/* set is_instantiated to true (1)
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_decl_mark_instantiated(struct ic_decl *decl);

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
