/*
 * Lip (Linear probing hash map)
 *
 * Lip: LipKt, LipVt, KHash, KTCmp, KTCpy -> LipOf(LipKt,LipVt)
 * ---
 *
 * Methods:
 * --------
 *
 *  + clean:      LipOf(KV,TV)* -> int 
 *  + del:        LipOf(KV,TV)*, LipKt* -> int 
 *  + get:        LipOf(KV,TV)*, LipKt* -> LipVt*
 *  + get_or_set: LipOf(KV,TV)*, LipKt*, LipVt* -> LipVt*
 *  + init:       LipOf(KV,TV)*, LipInitArgs -> int
 *  + set:        LipOf(KV,TV)*, LipKt*, LipVt* -> int
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


#define LipKt      Hotl_LipMap_Kt
#define LipVt      Hotl_LipMap_Vt
#define EntryT     LipMapEntryOf(LipKt, LipVt)
#define Fun(FName) lipmapfn(LipKt,LipVt,FName)
#define LipType    LipMapOf(LipKt,LipVt)
typedef struct { LipKt k; LipVt v; } EntryT;
#include <lip.h>

// #ifndef LipVtCmp
// #define LipVtCmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(LipVt))
// #endif // LipVtCmp

#ifndef LipVtCpy
#define LipVtCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(LipVt))
#endif // LipVtCpy


/* Value Functions */

static inline
LipVt* Fun(get_or_set)(LipType* l, LipKt* k, LipVt* v) {
    if (Fun(is_zero)(k) && l->zero) return &l->zero->v; /* Ok: get zero */

    bool found = 0;
    EntryT* e = Fun(__insert)(l, k, &found);
    if (!e) return NULL;                                /* ERROR: could not insert */
    if (!found && LipVtCpy(&e->v, v)) return NULL;         /* ERROR: LipVtCpy failed */
    return &e->v;
}

static inline int
Fun(get)(LipType* l, LipKt* k, LipVt* v[1]) {
    EntryT* ent;
    if (Fun(__get_entry)(l,k, &ent)) return -1;     /* __get_entry ERROR */
    if (ent) *v = &ent->v;                          /* Ok: found */
    else *v = NULL;
    return 0;                                       /* Ok: not found */
}


static inline int
Fun(set)(LipType* l, LipKt* k, LipVt* v) {
    bool found;
    EntryT* ent = Fun(__insert)(l, k, &found);
    if (!ent) return -1;                        /* __insert ERROR */
    if (LipVtCpy(&ent->v, v)) return -1;           /* LipVtCpy ERROR */
    return 0;                                   /* Ok */
}



/* END */
#undef LipKt
#undef EntryT
#undef Fun
#undef LipType
#undef LipVt
#undef LipVtCpy
// #undef LipVtCmp

#undef Hotl_LipMap_Kt
#undef Hotl_LipMap_Vt
