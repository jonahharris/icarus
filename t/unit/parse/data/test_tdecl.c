#include <string.h> /* strncmp */
#include <assert.h> /* assert */
#include <stdio.h> /* printf */

#include "../../../../src/parse/data/decl.h"

int main(void){
    struct ic_field *field = 0;
    struct ic_type_decl *tdecl = 0;
    /* fake indent level */
    unsigned int fake_indent = 0;


    /* test type_decl */
    tdecl = ic_type_decl_new("Foo", 3);
    assert(tdecl);

    /* check type name is correct */
    assert( ! strncmp( ic_symbol_contents(&(tdecl->name)), "Foo", 3) );

    /* check vector is zero used
     * FIXME expose lenght methods
     */
    assert( tdecl->fields.used == 0 );


    /* add some fields */
    field = ic_field_new("a", 1, "Int", 3);
    assert(field);
    assert( 1 == ic_type_decl_add_field(tdecl, field) );

    field = ic_field_new("b", 1, "String", 6);
    assert(field);
    assert( 1 == ic_type_decl_add_field(tdecl, field) );

    printf("Should see:\ntype Foo\n    a::Int\n    b::String\nend\n");

    /* output type */
    puts("Output:");
    ic_type_decl_print(tdecl, &fake_indent);

    return 0;
}