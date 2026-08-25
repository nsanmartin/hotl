/*
 * Arl (Array list)
 *
 * Arl: T, T_Cmp, T_Cpy, T_Clean -> ArlOf(T)
 * ---
 *  + T: element type
 *  + T_Cmp: element ptr comparator. Default: hotl_compare_bytes (memcmp sizeof T)
 *  + T_Cpy: element ptr copy. Default: hotl_copy_bytes (memmove sizeof T).
 *  + T_Clean: element ptr clean. Default: (void).
 *
 * Methods:
 * -------
 *
 * append : ArlOf(T)*, T*     -> T*
 * at     : ArlOf(T)*, size_t -> T*
 * back   : ArlOf(T)*         -> T*
 * begin  : ArlOf(T)*         -> T
 * clean  : ArlOf(T)*         -> void
 * end    : ArlOf(T)*         -> T
 * find   : ArlOf(T)*, T*     -> T*
 * pop    : ArlOf(T)*         -> T*
 *
 */

#include <hotl.h>

#ifndef Hotl_Arl_T
#error "Template type Hotl_Arl_T undefined"
#endif

#ifndef Hotl_Arl_T_Cmp
#define Hotl_Arl_T_Cmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(Hotl_Arl_T))
#endif // Hotl_Arl_T_Cmp


#ifndef Hotl_Arl_T_Cpy
#define Hotl_Arl_T_Cpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_Arl_T))
#endif // Hotl_Arl_T_Cpy


typedef struct ArlOf(Hotl_Arl_T) {
    Hotl_Arl_T* items;
    size_t len;
    size_t capacity;
} ArlOf(Hotl_Arl_T);


static inline int
arlfn(Hotl_Arl_T, __realloc)(ArlOf(Hotl_Arl_T)* a) {
    if (a->capacity * 2 < a->capacity) { /* overflow */ return -1; }

    const size_t new_capacity = a->capacity ? 2 * a->capacity : ArlDefaultInitialCapacity;
    Hotl_Arl_T* reallocated   = realloc((void*)a->items, new_capacity * sizeof(Hotl_Arl_T));
    if (!reallocated) return -1;

    a->capacity = new_capacity;
    a->items 	= reallocated;
    return a->items == 0;
} 


static inline int
arlfn(Hotl_Arl_T, init_reserve)(ArlOf(Hotl_Arl_T)* a, size_t size) {
    if (a->capacity || a->items) return -1; /* we expect an empty arl */
    a->len      = 0;
    a->capacity = size;
    a->items    = calloc(a->capacity, sizeof(Hotl_Arl_T));
    return a->items == 0;
}


static inline Hotl_Arl_T*
arlfn(Hotl_Arl_T, at)(ArlOf(Hotl_Arl_T)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

static inline Hotl_Arl_T*
arlfn(Hotl_Arl_T, back)(ArlOf(Hotl_Arl_T)* a) {
    return arl_len(a) ? arlfn(Hotl_Arl_T, at)(a, arl_len(a) - 1) : 0x0;
}

static inline bool
arlfn(Hotl_Arl_T, pop)(ArlOf(Hotl_Arl_T)* a) {
    Hotl_Arl_T* pp = arlfn(Hotl_Arl_T,back)(a);
    if (!pp) return false;
#ifdef Hotl_Arl_T_Clean
    Hotl_Arl_T_Clean(pp);
#endif
    --a->len;
    return true;
}


static inline Hotl_Arl_T*
arlfn(Hotl_Arl_T, append)(ArlOf(Hotl_Arl_T)* a, const Hotl_Arl_T* ptr) {
    if (a->len >= a->capacity) {
        if (arlfn(Hotl_Arl_T, __realloc)(a)) { /*error in realloc*/ return  NULL; }
    }
    if (Hotl_Arl_T_Cpy((void*)(a->items + a->len), ptr)) { /*error in T_Cpy */  return NULL; }
    return a->items + a->len++;
}


static inline Hotl_Arl_T* arlfn(Hotl_Arl_T, begin)(ArlOf(Hotl_Arl_T)*a) { return a->items; }
/*
 * We test whether items is NULL to avoid clang message:
 * runtime error: applying zero offset to null pointer
 *  SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior
 */
static inline Hotl_Arl_T*
arlfn(Hotl_Arl_T, end)(ArlOf(Hotl_Arl_T)*a) { return a->items ? a->items + a->len : a->items; }


static inline Hotl_Arl_T*
arlfn(Hotl_Arl_T, find) (ArlOf(Hotl_Arl_T)* a, Hotl_Arl_T* x) {
    for (size_t i = 0; i < a->len; ++i) {
        Hotl_Arl_T* addr = &a->items[i];
        if (Hotl_Arl_T_Cmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}


static inline void
arlfn(Hotl_Arl_T, reset)(ArlOf(Hotl_Arl_T)* a) {
#ifdef Hotl_Arl_T_Clean
    for (Hotl_Arl_T* it = arlfn(Hotl_Arl_T, begin)(a); it != arlfn(Hotl_Arl_T,end)(a); ++it) {
        Hotl_Arl_T_Clean(it);
    }
#endif
    a->len = 0;
}


static inline void
arlfn(Hotl_Arl_T, clean)(ArlOf(Hotl_Arl_T)*a) {
#ifdef Hotl_Arl_T_Clean
    for (Hotl_Arl_T* it = arlfn(Hotl_Arl_T, begin)(a); it != arlfn(Hotl_Arl_T,end)(a); ++it) {
        Hotl_Arl_T_Clean(it);
    }
#endif
    free((void*)a->items);
    *a = (ArlOf(Hotl_Arl_T)){0};
}


static inline int
arlfn(Hotl_Arl_T, remove_at)(ArlOf(Hotl_Arl_T)* a, size_t ix) {
    Hotl_Arl_T* elem = arlfn(Hotl_Arl_T,at)(a,ix);
    if (!elem) return -1;
#ifdef Hotl_Arl_T_Clean
    Hotl_Arl_T_Clean(elem);
#endif
    --a->len;
    if (ix < a->len)
        memmove(elem,  elem + 1, sizeof(Hotl_Arl_T) * (a->len - ix));
    return 0;
}

static inline void
arlfn(Hotl_Arl_T, remove)(ArlOf(Hotl_Arl_T)* a, Hotl_Arl_T* v) {
    size_t rm_count = 0;
    Hotl_Arl_T* dest = arlfn(Hotl_Arl_T, begin)(a);
    for (Hotl_Arl_T* it = arlfn(Hotl_Arl_T, begin)(a); it != arlfn(Hotl_Arl_T,end)(a); ++it) {
        if (Hotl_Arl_T_Cmp(it, v) == 0) {
#ifdef Hotl_Arl_T_Clean
            Hotl_Arl_T_Clean(it);
#endif
            ++rm_count;
        } else {
            if (dest < it) memmove(dest, it, sizeof(Hotl_Arl_T));
            ++dest;
        }
    }
    a->len -= rm_count;
}
#undef Hotl_Arl_T
#undef Hotl_Arl_T_Cmp
#undef Hotl_Arl_T_Cpy
#undef Hotl_Arl_T_Clean

