/*
 * Lip (Linear probing hash map)
 *
 * Lip: LipKT, LipVT, KHash, KTCmp, KTCpy -> LipOf(LipKT,LipVT)
 * ---
 *
 * Methods:
 * --------
 *
 *  + clean:      LipOf(KV,TV)* -> int 
 *  + del:        LipOf(KV,TV)*, LipKT* -> int 
 *  + get:        LipOf(KV,TV)*, LipKT* -> LipVT*
 *  + get_or_set: LipOf(KV,TV)*, LipKT*, LipVT* -> LipVT*
 *  + init:       LipOf(KV,TV)*, LipInitArgs -> int
 *  + set:        LipOf(KV,TV)*, LipKT*, LipVT* -> int
 *
 * */


#include <stdbool.h>

#include <hotl.h>

#ifndef Hotl_LipMap_Kt
#error "Template type Hotl_LipMap_Kt undefined"
#endif

#ifndef Hotl_LipMap_Vt
#error "Template type Hotl_LipMap_Vt undefined"
#endif


#define LipKT      Hotl_LipMap_Kt
#define LipVT      Hotl_LipMap_Vt
#define EntryT     LipMapEntryOf(LipKT, LipVT)
#define Fun(FName) lipmapfn(LipKT,LipVT,FName)
#define LipType    LipMapOf(LipKT,LipVT)
typedef struct { LipKT k; LipVT v; } EntryT;
#include <lip.h>

// #ifndef LipVTCmp
// #define LipVTCmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(LipVT))
// #endif // LipVTCmp

#ifndef LipVTCpy
#define LipVTCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(LipVT))
#endif // LipVTCpy


/* Value Functions */

static inline
LipVT* Fun(get_or_set)(LipType* l, LipKT* k, LipVT* v) {
    if (Fun(is_zero)(k) && l->zero) return &l->zero->v; /* Ok: get zero */

    bool found = 0;
    EntryT* e = Fun(__insert)(l, k, &found);
    if (!e) return NULL;                                /* ERROR: could not insert */
    if (!found && LipVTCpy(&e->v, v)) return NULL;         /* ERROR: LipVTCpy failed */
    return &e->v;
}

static inline int
Fun(get)(LipType* l, LipKT* k, LipVT* v[1]) {
    EntryT* ent = Fun(__get_entry)(l,k );
    if (ent) *v = &ent->v;                          /* Ok: found */
    else *v = NULL;
    return 0;                                       /* Ok: not found */
}


static inline int
Fun(set)(LipType* l, LipKT* k, LipVT* v) {
    bool found;
    EntryT* ent = Fun(__insert)(l, k, &found);
    if (!ent) return -1;                        /* __insert ERROR */
    if (LipVTCpy(&ent->v, v)) return -1;           /* LipVTCpy ERROR */
    return 0;                                   /* Ok */
}



/* END */
#undef LipKT
#undef EntryT
#undef Fun
#undef LipType
#undef LipVT
#undef LipVTCpy
// #undef LipVTCmp

#undef Hotl_LipMap_Kt
#undef Hotl_LipMap_Vt
