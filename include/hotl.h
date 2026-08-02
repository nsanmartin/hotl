/* Global hotl functions, ie, with larger scope than #define T .. #undef T
 * */
#ifndef __HOTL__H__
#define __HOTL__H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define HOTL_VERSION "0.1.3"

#define HOTL_CAT_LITERAL(A, B) A##_##B
#define HOTL_CAT_IND(A, B) HOTL_CAT_LITERAL(A,B)


#define BufOf(T) HOTL_CAT_IND(buf, T)
#define buffn(T, Name) HOTL_CAT_IND(HOTL_CAT_IND(buf, T), Name)

enum { ArlDefaultInitialCapacity = 2 };
#define ArlOf(T) HOTL_CAT_IND(arl, T)
#define ArlFn(T, Name) HOTL_CAT_IND(HOTL_CAT_IND(arl, T), Name)
#define arlfn ArlFn
#define arl_len(A) (A)->len


#define LipOf(KT, VT) HOTL_CAT_IND(HOTL_CAT_IND(lip, KT), VT)
#define lipfn(KT, VT, Name) HOTL_CAT_IND(LipOf(KT, VT), Name)
#define LipEntryOf(KT, VT) HOTL_CAT_IND(LipOf(KT, VT), entry)
#define LipZeroOf(KT, VT) HOTL_CAT_IND(LipOf(KT, VT), zero)
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
