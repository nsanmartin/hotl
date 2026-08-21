/* Global hotl functions, ie, with larger scope than #define T .. #undef T
 * */
#ifndef __HOTL__H__
#define __HOTL__H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HOTL_VERSION "0.1.5"

#define HOTL_CAT_LITERAL(A, B) A##_##B
#define HOTL_CAT_IND(A, B) HOTL_CAT_LITERAL(A,B)

#define HOTL_GET_MACRO_1_OR_2_PARAMS(_1,_2,_3,NAME,...) NAME
#define HOTL_GET_MACRO_2_OR_3_PARAMS(_1,_2,_3,_4, NAME,...) NAME
#define HOTL_SKIP(X)
 

#define BufOf(T) HOTL_CAT_IND(buf, T)
#define buffn(T, Name) HOTL_CAT_IND(HOTL_CAT_IND(buf, T), Name)

enum { ArlDefaultInitialCapacity = 2 };
#define ArlOf(T) HOTL_CAT_IND(arl, T)
#define ArlFn(T, Name) HOTL_CAT_IND(HOTL_CAT_IND(arl, T), Name)
#define arlfn ArlFn
#define arl_len(A) (A)->len


#define LipSetOf(T) HOTL_CAT_IND(lipset, T)
#define LipMapOf(KT, VT) HOTL_CAT_IND(HOTL_CAT_IND(lipmap, KT), VT)
#define LipSetEntryOf(T) HOTL_CAT_IND(LipOf(T), entry)
#define LipMapEntryOf(KT, VT) HOTL_CAT_IND(LipOf(KT, VT), entry)
#define LipSetZeroOf(T) HOTL_CAT_IND(LipOf(T), zero)
#define LipMapZeroOf(KT) HOTL_CAT_IND(LipOf(KT, VT), zero)
#define lipsetfn(T, Name) HOTL_CAT_IND(LipOf(T), Name)
#define lipmapfn(KT, VT, Name) HOTL_CAT_IND(LipOf(KT, VT), Name)

#define LipOf(...) \
    HOTL_GET_MACRO_1_OR_2_PARAMS(NULL,__VA_ARGS__,LipMapOf,LipSetOf,HOTL_SKIP)(__VA_ARGS__)
#define LipEntryOf(...) \
    HOTL_GET_MACRO_1_OR_2_PARAMS(NULL,__VA_ARGS__,LipMapEntryOf,LipSetEntryOf,HOTL_SKIP)(__VA_ARGS__)
#define LipZeroOf(...) \
    HOTL_GET_MACRO_1_OR_2_PARAMS(NULL,__VA_ARGS__,LipMapZeroOf,LipSetZeroOf,HOTL_SKIP)(__VA_ARGS__)
#define lipfn(...) \
    HOTL_GET_MACRO_2_OR_3_PARAMS(NULL,__VA_ARGS__,lipmapfn,lipsetfn,HOTL_SKIP)(__VA_ARGS__)

// #define lipfn(KT, VT, Name) HOTL_CAT_IND(LipOf(KT, VT), Name)
// #define LipEntryOf(KT, VT) HOTL_CAT_IND(LipOf(KT, VT), entry)
// #define LipZeroOf(KT, VT) HOTL_CAT_IND(LipOf(KT, VT), zero)
#define lip_buf(L) (L)->table

enum { LipMaximumAttempts = 256, LipDefaultSize = 1024 };

typedef struct { size_t sz; size_t attempts; } LipInitArgs;

#define PairOf(T1, T2) HOTL_CAT_IND(HOTL_CAT_IND(pair, T1), T2)
#define pairfn(T1, T2, Name) HOTL_CAT_IND(PairOf(T1, T2), Name)

static inline int
hotl_compare_bytes(const void* s1, const void* s2, size_t n) {
    return memcmp(s1, s2, n);
}

static inline int
hotl_copy_bytes(void* dest, const void* src, size_t n) {
    memmove(dest, src, n); return 0;
}

static inline size_t
hotl_djb2_k33_with_seed(char* s, size_t n, size_t seed) {
    unsigned long hash = seed; //5381;

    for (const char* end = s + n; s < end; ++s) {
        hash = ((hash << 5) + hash) + *s; /* hash * 33 + c */
    }

    return hash;
}

static inline size_t
hotl_djb2_k33(char* s, size_t n) {
    return hotl_djb2_k33_with_seed(s, n, 5381);
}

#endif // __HOTL__H__
