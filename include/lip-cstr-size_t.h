/*
 * The cstr type is the c string char* type.
 *
 * This name is consistent with standard lib functions
 * such as strcmp, strcpy, strdup, ...
 *
 */

#ifndef _LIP_CSTR_SIZE_T__
#define _LIP_CSTR_SIZE_T__

#include <che.h>
typedef char* cstr;

#define KT cstr
#define KTCmp cstrp_compare
#define KTCpy cstrp_copy
#define KTClean cstrp_clean
#ifndef KHash
#define KHash cstrp_hash
#endif

#define VT size_t

static inline int cstrp_compare(cstr* s1, cstr* s2) {
    if (*s1 == *s2) { return 0; }
    int null = (-(*s1 == 0) | (*s2 == 0));
    if (null) return null;
    return strcmp(*s1, *s2);
}

static inline int cstrp_copy(char** dest, char** src) {
    return (*dest = strdup(*src)) == NULL;
}
static inline int cstrp_hash(char** s) {
    return che_djb2_k33(*s, (size_t)strlen(*s));
}

static inline void cstrp_clean(char** s) { free(*s); }
#include <lip.h>

#endif // _LIP_STR_SIZE_T__
