#include <stdio.h> /* puts, fprintf */
#include <stdlib.h> /* calloc */
#include <string.h> /* strncpy */

#include "string.h"

/* FIXME crutch for unused param */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* build a new string from a char* and a length
 * this will allocate a new buffer and strncpy n
 * chars from source into the string
 * then it will add the null terminator
 * returns new string on success
 * returns 0 on failure
 */
struct ic_string * ic_string_new(char *source, unsigned int len){
    struct ic_string *str = calloc(1, sizeof(struct ic_string));
    if( ! str ){
        puts("ic_string_new: call to calloc failed");
        return 0;
    }

    if( ic_string_init(str, source, len) ){
        puts("ic_string_new: error in call to ic_string_init");
        free(str);
        return 0;
    }

    return str;
}

/* initalise an existing string from a char* and len
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_string_init(struct ic_string *str, char *source, unsigned int len){
    if( ! str ){
        puts("ic_string_init: pass in string was null");
        return 1;
    }

    /* our passed in len does not include the null terminator
     * but carray must take this into account
     */
    if( ic_carray_init( &(str->backing), (len + 1) ) ){
        puts("ic_string_init: call to ic_carray_init failed");
        return 1;
    }

    if( ! str->backing.contents ){
        puts("ic_string_init: results of ic_carray_init were suspicious");
        return 1;
    }

    /* strncpy source into string */
    strncpy(str->backing.contents, source, len);

    /* insert null terminator */
    str->backing.contents[len+1] = '\0';

    /* set the number of used chars
     * NB: this does not include the null terminator
     */
    str->used = len;

    return 0;

}



/* returns backing character array
 * the caller is NOT allowed to mutate this character array directly
 *
 * returns a char * on success
 * returns 0 on error
 */
char * ic_string_contents(struct ic_string *string){
    if( ! string ){
        puts("ic_string_contents: null string passed in");
        return 0;
    }

    return string->backing.contents;
}

/* get the strlen of the stores string
 * this length does NOT include the null terminator
 *
 * returns numbers of characters in string on success
 * returns -1 on error
 */
int ic_string_length(struct ic_string *string){
    if( ! string ){
        puts("ic_string_contents: null string passed in");
        return 0;
    }

    /* string->used is the strlen of the underlying string
     * this is the number of character slots used in the array
     * and does NOT include the null terminator
     */
    return string->used;
}

/* get a character from the string
 *
 * returns character on success
 * returns 0 on failure
 */
char ic_string_get(struct ic_string *string, unsigned int offset){
    if( ! string ){
        puts("ic_string_get: null string passed in");
        return 0;
    }

    /* backing.len includes the null terminator
     * so we must subtract one before using it to bounds check
     */
    if( offset >= (string->backing.len - 1) ){
        printf("ic_string_get: offset '%d' out of range '%d'\n", offset, ( string->backing.len - 1) );
        return 0;
    }

    return string->backing.contents[offset];
}

/* append the contents of `from` to `to`
 * this will resize `to` to guarantee there is enough space
 *
 * returns 0 on success
 * returns 1 on error
 */
unsigned int ic_string_append(struct ic_string *to, struct ic_string *from){
    /* FIXME */
    puts("ic_string_append: unimplemented");
    return 1;
}

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
unsigned int ic_string_append_char(struct ic_string *to, char *from, unsigned int from_len){
    /* FIXME */
    puts("ic_string_append_char: unimplemented");
    return 1;
}


