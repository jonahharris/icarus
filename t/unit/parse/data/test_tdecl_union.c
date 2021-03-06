#include <assert.h> /* assert */
#include <stdio.h>  /* printf */
#include <string.h> /* strncmp */

#include "../../../../src/parse/data/decl.h"
#include "../../../../src/parse/permissions.h"

int main(void) {
    struct ic_field *field = 0;
    struct ic_decl_type *tdecl = 0;
    struct ic_type_ref *type_ref1 = 0;
    struct ic_type_ref *type_ref2 = 0;
    /* fake indent level */
    unsigned int fake_indent = 0;

    /* test decl_union */
    tdecl = ic_decl_type_new_union("Foo", 3);
    assert(tdecl);

    /* check type name is correct */
    assert(!strncmp(ic_symbol_contents(ic_decl_type_get_name(tdecl)), "Foo", 3));

    assert(0 == ic_decl_type_field_length(tdecl));

    /* add some fields */
    type_ref1 = ic_type_ref_symbol_new("Signed", 6);
    assert(type_ref1);
    field = ic_field_new("a", 1, type_ref1, ic_parse_perm_default());
    assert(field);
    assert(1 == ic_decl_type_add_field(tdecl, field));

    assert(1 == ic_decl_type_field_length(tdecl));

    type_ref2 = ic_type_ref_symbol_new("String", 6);
    assert(type_ref2);
    field = ic_field_new("b", 1, type_ref2, ic_parse_perm_default());
    assert(field);
    assert(1 == ic_decl_type_add_field(tdecl, field));

    assert(2 == ic_decl_type_field_length(tdecl));

    assert(0 == ic_decl_type_get_field_offset(tdecl, "a"));
    assert(1 == ic_decl_type_get_field_offset(tdecl, "b"));

    printf("Should see:\ntype Foo\n    a::Signed\n    b::String\nend\n");

    /* output type */
    puts("Output:");
    ic_decl_type_print(stdout, tdecl, &fake_indent);

    assert(ic_decl_type_destroy(tdecl, 1));

    return 0;
}
