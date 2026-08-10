#include <hotl.h>

#ifndef Hotl_Pair_T1
#error "Template type Hotl_Pair_T1 undefined"
#endif
#ifndef Hotl_Pair_T2
#error "Template type Hotl_Pair_T1 undefined"
#endif


typedef struct PairOf(Hotl_Pair_T1,Hotl_Pair_T2) {
    Hotl_Pair_T1 fst;
    Hotl_Pair_T2 snd;
} PairOf(Hotl_Pair_T1,Hotl_Pair_T2);

static inline void
pairfn(Hotl_Pair_T1,Hotl_Pair_T2,clean)(PairOf(Hotl_Pair_T1,Hotl_Pair_T2)* p) {
#ifdef Hotl_Pair_T1_Clean
    Hotl_Pair_T1_Clean(&p->fst);
#endif
#ifdef Hotl_Pair_T2_Clean
    Hotl_Pair_T2_Clean(&p->snd);
#endif
}

#ifndef Hotl_Pair_T1_Cpy
#define Hotl_Pair_T1_Cpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_Pair_T1))
#endif // Hotl_Pair_T1_Cpy
#ifndef Hotl_Pair_T2_Cpy
#define Hotl_Pair_T2_Cpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_Pair_T2))
#endif // Hotl_Pair_T2_Cpy
#ifndef Hotl_Pair_T1_Move
#define Hotl_Pair_T1_Move(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_Pair_T1))
#endif // Hotl_Pair_T1_Move
#ifndef Hotl_Pair_T2_Move
#define Hotl_Pair_T2_Move(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_Pair_T2))
#endif // Hotl_Pair_T2_Move

static inline int
pairfn(Hotl_Pair_T1,Hotl_Pair_T2,copy)(
    PairOf(Hotl_Pair_T1,Hotl_Pair_T2)* p,
    const Hotl_Pair_T1* t1,
    const Hotl_Pair_T2* t2
) {
    if (Hotl_Pair_T1_Cpy(&p->fst, t1)) return -1;
    if (Hotl_Pair_T2_Cpy(&p->snd, t2)) return -1;
    return 0;
}

static inline int
pairfn(Hotl_Pair_T1,Hotl_Pair_T2,move)(
    PairOf(Hotl_Pair_T1,Hotl_Pair_T2)* p,
    const Hotl_Pair_T1* t1,
    const Hotl_Pair_T2* t2
) {
    if (Hotl_Pair_T1_Move(&p->fst, t1)) return -1;
    if (Hotl_Pair_T2_Move(&p->snd, t2)) return -1;
    return 0;
}
#undef Hotl_Pair_T1
#undef Hotl_Pair_T1_Cpy
#undef Hotl_Pair_T1_Move
#undef Hotl_Pair_T1_Clean
#undef Hotl_Pair_T2
#undef Hotl_Pair_T2_Cpy
#undef Hotl_Pair_T2_Move
#undef Hotl_Pair_T2_Clean

