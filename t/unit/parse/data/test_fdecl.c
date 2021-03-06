#include <assert.h> /* assert */
#include <stdio.h>  /* printf */
#include <string.h> /* strncmp */

#include "../../../../src/parse/data/decl.h"
#include "../../../../src/parse/permissions.h"

int main(void) {
    struct ic_field *field = 0;
    struct ic_type_param *tparam = 0;
    struct ic_decl_func *fdecl = 0;
    struct ic_type_ref *type_ref1 = 0;
    struct ic_type_ref *type_ref2 = 0;
    /* fake indent level */
    unsigned int fake_indent = 0;

    /* test decl_func */
    fdecl = ic_decl_func_new("Foo", 3);
    assert(fdecl);

    /* check type name is correct */
    assert(!strncmp(ic_symbol_contents(&(fdecl->name)), "Foo", 3));

    /* check vector is zero used */
    assert(0 == fdecl->args.used);
    assert(0 == ic_decl_func_args_length(fdecl));
    assert(0 == fdecl->type_params.used);
    assert(0 == ic_decl_func_type_params_length(fdecl));

    /* add some fields */
    type_ref1 = ic_type_ref_symbol_new("Signed", 6);
    assert(type_ref1);
    field = ic_field_new("a", 1, type_ref1, ic_parse_perm_default());
    assert(field);
    assert(1 == ic_decl_func_args_add(fdecl, field));
    assert(1 == fdecl->args.used);
    assert(1 == ic_decl_func_args_length(fdecl));

    type_ref2 = ic_type_ref_symbol_new("String", 6);
    assert(type_ref2);
    field = ic_field_new("b", 1, type_ref2, ic_parse_perm_default());
    assert(field);
    assert(1 == ic_decl_func_args_add(fdecl, field));
    assert(2 == fdecl->args.used);
    assert(2 == ic_decl_func_args_length(fdecl));

    /* add some type params */
    tparam = ic_type_param_new("A", 1);
    assert(tparam);
    assert(1 == ic_decl_func_type_params_add(fdecl, tparam));
    assert(1 == fdecl->type_params.used);
    assert(1 == ic_decl_func_type_params_length(fdecl));

    tparam = ic_type_param_new("B", 1);
    assert(tparam);
    assert(1 == ic_decl_func_type_params_add(fdecl, tparam));
    assert(2 == fdecl->type_params.used);
    assert(2 == ic_decl_func_type_params_length(fdecl));

    printf("Should see:\n# Foo[A,B](Signed,String)\nfn Foo[A, B](a::Signed, b::String) -> Unit\nend\n");

    /* output type */
    puts("Output:");
    ic_decl_func_print(stdout, fdecl, &fake_indent);

    assert(ic_decl_func_destroy(fdecl, 1));

    return 0;
}
