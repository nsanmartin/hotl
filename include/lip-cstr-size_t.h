/*
 * The cstr type is the c string char* type.
 *
 * This name is consistent with standard lib functions
 * such as strcmp, strcpy, strdup, ...
 *
 */

#ifndef _LIP_CSTR_SIZE_T__
#define _LIP_CSTR_SIZE_T__

#include <hotl.h>
typedef char* cstr;

#define Hotl_LipMap_Kt cstr
#define Hotl_LipMap_Vt size_t
#define LipKtCmp cstrp_compare
#define LipKtCpy cstrp_copy
#define LipKtClean cstrp_clean
#ifndef LipHash
#define LipHash cstrp_hash
#endif


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
    return hotl_djb2_k33(*s, (size_t)strlen(*s));
}

static inline void cstrp_clean(char** s) { free(*s); }
#include <lipmap.h>

#endif // _LIP_STR_SIZE_T__
