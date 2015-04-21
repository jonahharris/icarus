#ifndef ICARUS_STRING_H
#define ICARUS_STRING_H

#include "carray.h"

struct ic_string {
    /* the number of characters slots used within the carray
     * this is useful if we start mutating the string
     *
     * this does NOT include the null terminator
     * so this 'used' is equiv to strlen(ic_string_contents(self));
     */
    unsigned int used;

    /* internally use a character array
     * ic_string must additional enforce string-characteristics
     * such as being null terminated
     */
    struct ic_carray backing;
};

/* build a new string from a char* and a length
 * this will allocate a new buffer and strncpy len
 * chars from source into the string
 * then it will add the null terminator
 *
 * returns new string on success
 * returns 0 on failure
 */
struct ic_string * ic_string_new(char *source, unsigned int len);

/* initialise an existing symbol from a char* and a length
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_string_init(struct ic_string *string, char *source, unsigned int len);

/* returns backing character array
 * the caller is NOT allowed to mutate this character array directly
 *
 * returns a char * on success
 * returns 0 on error
 */
char * ic_string_contents(struct ic_string *string);

/* get the strlen of the used slots in the string
 * this length does NOT include the null terminator
 *
 * returns numbers of characters in string on success
 * returns -1 on error
 */
int ic_string_length(struct ic_string *string);

/* get a character from the string
 *
 * you may get any character up to and including the '\0'
 *
 * bounds checked
 *
 * returns character on success
 * returns 0 on failure
 */
char ic_string_get(struct ic_string *string, unsigned int offset);

/* set character at [pos] to val
 *
 * pos must not be the null terminator
 * pos must be < used
 *
 * val must not be '\0'
 *
 * bounds checked
 *
 * returns 1 on success
 * returns 0 on failure
 */
char ic_string_set(struct ic_string *string, unsigned int pos, char val);

/* append the contents of `from` to `to`
 * this will resize `to` to guarantee there is enough space
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_string_append(struct ic_string *to, struct ic_string *from);

/* append the contents of `from` to `to`
 * copying over a maximum of from_len
 *
 * this will resize `to` to guarantee there is enough space
 *
 * this will also make sure the final character internally is a \0
 * even if the character at from[from_len - 1] != '\0'
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_string_append_char(struct ic_string *to, char *from, unsigned int from_len);

#endif
