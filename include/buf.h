/*
 * Buf (Buffer)
 *
 * Buf: T, T_Cmp, T_Cpy, T_Clean -> ArlOf(T)
 * ---
 *  + T       : element type
 *  + T_Cmp   : element ptr comparator. Default: hotl_compare_bytes (memcmp sizeof T)
 *  + T_Cpy   : element ptr copy. Default: hotl_copy_bytes (memmove sizeof T).
 *  + T_Clean : element ptr clean. Default: (void).
 *
 * Methods:
 * -------
 *
 * append  : BufOf(T)*, T*, size_t ->  T*
 * at      : BufOf(T)*, size_t     ->  T*
 * begin   : BufOf(T)*             ->  T
 * calloc  : BufOf(T)*             ->  T*
 * clean   : BufOf(T)*             ->  void
 * end     : BufOf(T)*             ->  T
 * find    : BufOf(T)*, T*         ->  T*
 * prepend : BufOf(T)*, T*, size_t ->  T*
 * reset   : BufOf(T)*             ->  void
 *
 */

#include <hotl.h>

#ifndef Hotl_Buf_T
#error "Template type Hotl_Buf_T undefined"
#endif

#define buflen(B) (B)->len
typedef struct BufOf(Hotl_Buf_T) {
    Hotl_Buf_T* items;
    size_t len;
    size_t capacity;
} BufOf(Hotl_Buf_T);


static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, calloc)(BufOf(Hotl_Buf_T)* a, size_t len) {
    if (!len || a->items) { return NULL; }
    a->items    = calloc(len, sizeof(Hotl_Buf_T)); 
    a->len      = len;
    a->capacity = len;
    return a->items ? a->items : NULL;
}

static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, __ensure_extra_capacity)(BufOf(Hotl_Buf_T)* b, size_t len) {
    if (b->len + len < b->len) { /* overflow */ return NULL; }
    size_t rest_offset = b->len;
    if (b->len + len > b->capacity) {
        const size_t new_capacity  = b->len + len;
        Hotl_Buf_T* reallocated    = realloc((void*)b->items, new_capacity * sizeof(Hotl_Buf_T));
        if (!reallocated) { /* realloc error */ return NULL; }

        b->capacity = new_capacity;
        b->items    = reallocated;
    }

    return b->items + rest_offset;
} 


static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, append)(BufOf(Hotl_Buf_T)* b, Hotl_Buf_T* data, size_t len) {
    if (!data || !len) { return NULL; }
    Hotl_Buf_T* rest = buffn(Hotl_Buf_T, __ensure_extra_capacity)(b, len);
    if (!rest) { return NULL; }
    memcpy((void*)rest, data, len * sizeof(Hotl_Buf_T));
    b->len += len;
    return rest;
}

static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, prepend)(BufOf(Hotl_Buf_T)* b, Hotl_Buf_T* data, size_t len) {
    if (!data || !len) { return NULL; }
    Hotl_Buf_T* rest = buffn(Hotl_Buf_T, __ensure_extra_capacity)(b, len);
    if (!rest) { return NULL; }
    if (b->len)
        memmove((void*)(b->items + len), b->items, b->len * sizeof(Hotl_Buf_T));
    memcpy((void*)b->items, data, len * sizeof(Hotl_Buf_T));
    b->len += len;
    return rest;
}

static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, at)(BufOf(Hotl_Buf_T)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

#ifndef Hotl_Buf_T_Cmp
#define Hotl_Buf_T_Cmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(Hotl_Buf_T))
#endif // Hotl_Buf_T_Cmp


#ifndef Hotl_Buf_T_Cpy
#define Hotl_Buf_T_Cpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_Buf_T))
#endif // Hotl_Buf_T_Cpy


static inline Hotl_Buf_T* buffn(Hotl_Buf_T, begin)(BufOf(Hotl_Buf_T)*a) { return a->items; }

static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, end)(BufOf(Hotl_Buf_T)*a) { return a->items ? a->items + a->len : a->items; }

#ifdef Hotl_Buf_T_Cmp
static inline Hotl_Buf_T*
buffn(Hotl_Buf_T, find) (BufOf(Hotl_Buf_T)* a, Hotl_Buf_T* x) {
    for (size_t i = 0; i < a->len; ++i) {
        Hotl_Buf_T* addr = &a->items[i];
        if (Hotl_Buf_T_Cmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}
#endif // Hotl_Buf_T_Cmp


static inline void
buffn(Hotl_Buf_T, reset)(BufOf(Hotl_Buf_T)*a) {
#ifdef Hotl_Buf_T_Clean
    for (Hotl_Buf_T* it = buffn(Hotl_Buf_T, begin)(a); it != buffn(Hotl_Buf_T,end)(a); ++it) {
        Hotl_Buf_T_Clean(it);
    }
#endif
    a->len = 0;
}


static inline void
buffn(Hotl_Buf_T, clean)(BufOf(Hotl_Buf_T)*a) {
    buffn(Hotl_Buf_T, reset)(a);
    free((void*)a->items);
    *a = (BufOf(Hotl_Buf_T)){0};
}

#undef Hotl_Buf_T
#undef Hotl_Buf_T_Cmp
#undef Hotl_Buf_T_Cpy
#undef Hotl_Buf_T_Clean

