#include "expr.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

unsigned int ic_b2c_compile_expr_fcall(struct ic_kludge *input_kludge, struct ic_expr_func_call *fcall, FILE *out);
unsigned int ic_b2c_compile_expr_identifier(struct ic_kludge *input_kludge, struct ic_expr_identifier *identifier, FILE *out);
unsigned int ic_b2c_compile_expr_constant(struct ic_kludge *input_kludge, struct ic_expr_constant *constant, FILE *out);
unsigned int ic_b2c_compile_expr_operator(struct ic_kludge *input_kludge, struct ic_expr_operator *operator, FILE *out);
unsigned int ic_b2c_compile_expr_faccess(struct ic_kludge *input_kludge, struct ic_expr_faccess *faccess, FILE *out);

/* compile a given expr to specified file
 *
 * generate a c program
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_b2c_compile_expr(struct ic_kludge *input_kludge, struct ic_expr *expr, FILE *out){
  unsigned int indent_level = 1;

  if( ! input_kludge ){
    puts("ic_b2c_compile_expr: input_kludge was null");
    return 0;
  }

  if( ! expr ){
    puts("ic_b2c_compile_expr: expr was null");
    return 0;
  }

  if( ! out ){
    puts("ic_b2c_compile_expr: out was null");
    return 0;
  }

  switch( expr->tag ){
    case ic_expr_type_func_call:
      return ic_b2c_compile_expr_fcall(input_kludge, &(expr->u.fcall), out);
      break;

    case ic_expr_type_identifier:
      return ic_b2c_compile_expr_identifier(input_kludge, &(expr->u.id), out);
      break;

    case ic_expr_type_constant:
      return ic_b2c_compile_expr_constant(input_kludge, &(expr->u.cons), out);
      break;

    case ic_expr_type_operator:
      return ic_b2c_compile_expr_operator(input_kludge, &(expr->u.op), out);
      break;

    case ic_expr_type_field_access:
      return ic_b2c_compile_expr_faccess(input_kludge, &(expr->u.faccess), out);
      break;

    default:
      puts("ic_b2c_compile_expr: impossible tag");

      puts("ic_b2c_compile_expr: called for");
      ic_expr_print(expr, &indent_level);
      /* caller of ic_expr_print must add \n */
      puts("");
      return 0;

      break;
  }

  puts("ic_b2c_compile_expr: impossible");
  return 0;
}

unsigned int ic_b2c_compile_expr_fcall(struct ic_kludge *input_kludge, struct ic_expr_func_call *fcall, FILE *out){
  /* indent level used for _print calls for debug
   * FIXME gross
   */
  unsigned int indent_level = 1;
  /* function decl */
  struct ic_decl_func *fdecl = 0;
  /* mangled function signature, suitable for output */
  char *fdecl_sig_mangled = 0;
  /* offset into fcall args pvector */
  unsigned int i = 0;
  /* length of fcall args pvector */
  unsigned int length = 0;
  /* current arg */
  struct ic_expr *expr = 0;

  if( ! input_kludge ){
    puts("ic_b2c_compile_expr_fcall: input_kludge was null");
    return 0;
  }

  if( ! fcall ){
    puts("ic_b2c_compile_expr_fcall: fcall was null");
    return 0;
  }

  if( ! out ){
    puts("ic_b2c_compile_expr_fcall: out was null");
    return 0;
  }

  puts("ic_b2c_compile_expr_fcall: call for");
  ic_expr_func_call_print(fcall, &indent_level);
  puts("");

  fdecl = ic_expr_func_call_get_fdecl(fcall);
  if( ! fdecl ){
    puts("ic_b2c_compile_expr_fcall: call to ic_expr_func_call_get_fdecl failed");
    return 0;
  }

  /* find function name to call */
  fdecl_sig_mangled = ic_decl_func_sig_mangled(fdecl);
  if( ! fdecl_sig_mangled ){
    puts("ic_b2c_compile_expr_fcall: call to ic_decl_func_sig_mangled failed");
    return 0;
  }

  /* omit function name */
  fputs(fdecl_sig_mangled, out);

  /* omit opening ( */
  fputs("(", out);

  /* omit arguments */
  length = ic_expr_func_call_length(fcall);

  for( i=0; i<length; ++i ){
    expr = ic_expr_func_call_get_arg(fcall, i);
    if( ! expr ){
      puts("ic_b2c_compile_expr_fcall: call to ic_expr_func_call_get_arg");
      return 0;
    }

    if( ! ic_b2c_compile_expr(input_kludge, expr, out) ){
      puts("ic_b2c_compile_expr_fcall: call to ic_b2c_compile_expr failed");
      return 0;
    }
  }

  /* omit closing ) */
  fputs(")", out);

  /* FIXME communicate to caller the need to end (e.g. optionally append `;`) */

  puts("ic_b2c_compile_expr_fcall: implementation pending");
  return 1;
}

unsigned int ic_b2c_compile_expr_identifier(struct ic_kludge *input_kludge, struct ic_expr_identifier *identifier, FILE *out){
  unsigned int indent_level = 1;

  if( ! input_kludge ){
    puts("ic_b2c_compile_expr_identifier: input_kludge was null");
    return 0;
  }

  if( ! identifier ){
    puts("ic_b2c_compile_expr_identifier: identifier was null");
    return 0;
  }

  if( ! out ){
    puts("ic_b2c_compile_expr_identifier: out was null");
    return 0;
  }

  puts("ic_b2c_compile_expr_identifier: call for");
  ic_expr_identifier_print(identifier, &indent_level);
  puts("");

  puts("ic_b2c_compile_expr_identifier: unimplemented");
  return 0;
}

unsigned int ic_b2c_compile_expr_constant(struct ic_kludge *input_kludge, struct ic_expr_constant *constant, FILE *out){
  unsigned int indent_level = 1;

  if( ! input_kludge ){
    puts("ic_b2c_compile_expr_constant: input_kludge was null");
    return 0;
  }

  if( ! constant ){
    puts("ic_b2c_compile_expr_constant: constant was null");
    return 0;
  }

  if( ! out ){
    puts("ic_b2c_compile_expr_constant: out was null");
    return 0;
  }

  puts("ic_b2c_compile_expr_constant: call for");
  ic_expr_constant_print(constant, &indent_level);
  puts("");

  puts("ic_b2c_compile_expr_constant: unimplemented");
  return 0;
}

unsigned int ic_b2c_compile_expr_operator(struct ic_kludge *input_kludge, struct ic_expr_operator *operator, FILE *out){
  unsigned int indent_level = 1;

  if( ! input_kludge ){
    puts("ic_b2c_compile_expr_operator: input_kludge was null");
    return 0;
  }

  if( ! operator ){
    puts("ic_b2c_compile_expr_operator: operator was null");
    return 0;
  }

  if( ! out ){
    puts("ic_b2c_compile_expr_operator: out was null");
    return 0;
  }

  puts("ic_b2c_compile_expr_operator: call for");
  ic_expr_operator_print(operator, &indent_level);
  puts("");

  puts("ic_b2c_compile_expr_operator: unimplemented");
  return 0;
}

unsigned int ic_b2c_compile_expr_faccess(struct ic_kludge *input_kludge, struct ic_expr_faccess *faccess, FILE *out){
  unsigned int indent_level = 1;

  if( ! input_kludge ){
    puts("ic_b2c_compile_expr_faccess: input_kludge was null");
    return 0;
  }

  if( ! faccess ){
    puts("ic_b2c_compile_expr_faccess: faccess was null");
    return 0;
  }

  if( ! out ){
    puts("ic_b2c_compile_expr_faccess: out was null");
    return 0;
  }

  puts("ic_b2c_compile_expr_faccess: call for");
  ic_expr_faccess_print(faccess, &indent_level);
  puts("");

  puts("ic_b2c_compile_expr_faccess: unimplemented");
  return 0;
}

