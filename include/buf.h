/*
 * Buf (Buffer)
 *
 * Buf: T, TCmp, TCpy, TClean -> ArlOf(T)
 * ---
 *  + T: element type
 *  + TCmp: element ptr comparator. Default: che_compare_bytes
 *    (memcmp sizeof T)
 *  + TCpy: element ptr copy. Default: che_copy_bytes
 *           (memmove sizeof T).
 *  + TClean: element ptr clean. Default: (void).
 *
 * Methods:
 * -------
 *
 * append:            BufOf(T)*, T*, size_t  ->  T*
 * at:                BufOf(T)*, size_t  ->  T*
 * begin:             BufOf(T)*  ->  T
 * calloc:            BufOf(T)*  ->  T*
 * clean:             BufOf(T)*  ->  void
 * end:               BufOf(T)*  ->  T
 * find:              BufOf(T)*, T*  ->  T*
 * prepend:           BufOf(T)*, T*, size_t  ->  T*
 * reset:             BufOf(T)*  ->  void
 *
 */

#include <che.h>

#ifndef T
#error "Template type T undefined"
#endif

#define buflen(B) (B)->len
typedef struct BufOf(T) {
    T* items;
    size_t len;
    size_t capacity;
} BufOf(T);


static inline T*
buffn(T, calloc)(BufOf(T)* a, size_t len) {
    if (!len || a->items) { return NULL; }
    a->items    = calloc(len, sizeof(T)); 
    a->len      = len;
    a->capacity = len;
    return a->items ? a->items : NULL;
}

static inline T*
buffn(T, __ensure_extra_capacity)(BufOf(T)* b, size_t len) {
    if (b->len + len < b->len) { /* overflow */ return NULL; }
    size_t rest_offset = b->len;
    if (b->len + len > b->capacity) {
        b->capacity  = b->len + len;
        b->items = realloc((void*)b->items, b->capacity * sizeof(T)); 
        if (!b->items) { return NULL; };
    }

    return b->items + rest_offset;
} 


static inline T*
buffn(T, append)(BufOf(T)* b, T* data, size_t len) {
    if (!data || !len) { return NULL; }
    T* rest = buffn(T, __ensure_extra_capacity)(b, len);
    if (!rest) { return NULL; }
    memcpy((void*)rest, data, len * sizeof(T));
    b->len += len;
    return rest;
}

static inline T*
buffn(T, prepend)(BufOf(T)* b, T* data, size_t len) {
    if (!data || !len) { return NULL; }
    T* rest = buffn(T, __ensure_extra_capacity)(b, len);
    if (!rest) { return NULL; }
    if (b->len)
        memmove((void*)(b->items + len), b->items, b->len * sizeof(T));
    memcpy((void*)b->items, data, len * sizeof(T));
    b->len += len;
    return rest;
}

static inline T*
buffn(T, at)(BufOf(T)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

#ifndef TCmp
#define TCmp(S1, S2) che_compare_bytes(S1, S2, sizeof(T))
#endif // TCmp


#ifndef TCpy
#define TCpy(Dst, Src) che_copy_bytes(Dst, Src, sizeof(T))
#endif // TCpy


static inline T* buffn(T, begin)(BufOf(T)*a) { return a->items; }

static inline T* buffn(T, end)(BufOf(T)*a) { return a->items ? a->items + a->len : a->items; }

#ifdef TCmp
static inline T*
buffn(T, find) (BufOf(T)* a, T* x) {
    for (size_t i = 0; i < a->len; ++i) {
        T* addr = &a->items[i];
        if (TCmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}
#endif // TCmp


static inline void
buffn(T, reset)(BufOf(T)*a) {
#ifdef TClean
    for (T* it = buffn(T, begin)(a); it != buffn(T,end)(a); ++it) {
        TClean(it);
    }
#endif
    a->len = 0;
}


static inline void
buffn(T, clean)(BufOf(T)*a) {
    buffn(T, reset)(a);
    free((void*)a->items);
    *a = (BufOf(T)){0};
}

#undef T
#undef TCmp
#undef TCpy
#undef TClean

