#include <hotl.h>

#ifndef T1
#error "Template type T1 undefined"
#endif
#ifndef T2
#error "Template type T1 undefined"
#endif


typedef struct PairOf(T1,T2) {
    T1 fst;
    T2 snd;
} PairOf(T1,T2);

static inline void
pairfn(T1,T2,clean)(PairOf(T1,T2)* p) {
#ifdef T1Clean
    T1Clean(&p->fst);
#endif
#ifdef T2Clean
    T2Clean(&p->snd);
#endif
}

#ifndef T1Cpy
#define T1Cpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(T1))
#endif // T1Cpy
#ifndef T2Cpy
#define T2Cpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(T2))
#endif // T2Cpy
#ifndef T1Move
#define T1Move(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(T1))
#endif // T1Move
#ifndef T2Move
#define T2Move(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(T2))
#endif // T2Move

static inline int
pairfn(T1,T2,copy)(PairOf(T1,T2)* p, const T1* t1, const T2* t2) {
    if (T1Cpy(&p->fst, t1)) return -1;
    if (T2Cpy(&p->snd, t2)) return -1;
    return 0;
}

static inline int
pairfn(T1,T2,move)(PairOf(T1,T2)* p, const T1* t1, const T2* t2) {
    if (T1Move(&p->fst, t1)) return -1;
    if (T2Move(&p->snd, t2)) return -1;
    return 0;
}
#undef T1
#undef T1Cpy
#undef T1Move
#undef T1Clean
#undef T2
#undef T2Cpy
#undef T2Move
#undef T2Clean

