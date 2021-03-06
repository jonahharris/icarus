#ifndef ICARUS_TYPE_PARAM_H
#define ICARUS_TYPE_PARAM_H

#include "../../data/symbol.h"
#include "decl.h"

/* a type parameter for type polymorphism
 * this is given a name at parse-time
 * we then copy this for each instantiation of the fdecl/tdecl and bind it to
 * a concrete type
 */
struct ic_type_param {
    /* symbol set during parse at creation */
    struct ic_symbol name;
    /* tdecl set during analysis of instantiation */
    struct ic_decl_type *tdecl;
};

/* allocate and initialise a new type_param
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_param *ic_type_param_new(char *name_src, unsigned int name_len);

/* initialise an existing type_param
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_param_init(struct ic_type_param *tparam, char *name_src, unsigned int name_len);

/* destroy a type_param
 *
 * will only free tparam is `free_tparam` is truthy
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_param_destroy(struct ic_type_param *tparam, unsigned int free_tparam);

/* perform a deep copy of a type_param
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_type_param *ic_type_param_deep_copy(struct ic_type_param *tparam);

/* perform a deep copy of a type_param embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_param_deep_copy_embedded(struct ic_type_param *from, struct ic_type_param *to);

/* perform a deep copy of a pvector of type_param(s)
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_pvector *ic_type_param_pvector_deep_copy(struct ic_pvector *tparams);

/* perform a deep copy of a pvector of type_param(s) embedded within an object
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_param_pvector_deep_copy_embedded(struct ic_pvector *from, struct ic_pvector *to);

/* get pointer to internal name
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_type_param_get_name(struct ic_type_param *tparam);

/* set the tdecl on this type_param
 *
 * error if already set
 *
 * returns 1 on success
 * returns 0 on failure
 */
unsigned int ic_type_param_set(struct ic_type_param *tparam, struct ic_decl_type *tdecl);

/* get the tdecl on this type param
 *
 * error if not set
 *
 * returns 1 on success
 * returns 0 on failure
 */
struct ic_decl_type *ic_type_param_get(struct ic_type_param *tparam);

/* check if this type param is already resolved to a type
 * (if tdecl is set)
 *
 * returns 1 if set
 * returns 0 if not set
 */
unsigned int ic_type_param_check_set(struct ic_type_param *tparam);

/* print this type_param */
void ic_type_param_print(FILE *fd, struct ic_type_param *tparam);

/* find a type_param by string
 *
 * this could probably go somewhere else better.
 *
 * returns * on successful find
 * returns 0 on failure to find
 * returns 0 an failure
 */
struct ic_type_param *ic_type_param_search(struct ic_pvector *type_params, char *tname);

/* get mangled_name
 * this is owned by the underlying decl_type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_type_param_mangled_name(struct ic_type_param *tparam);

/* get full_name
 * this is owned by the underlying decl_type
 *
 * returns * on success
 * returns 0 on failure
 */
struct ic_symbol *ic_type_param_full_name(struct ic_type_param *tparam);

#endif
