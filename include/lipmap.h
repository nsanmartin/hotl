/*
 * Lip (Linear probing hash map)
 *
 * Lip: LipKT, LipVT, KHash, KTCmp, KTCpy -> LipOf(LipKT,LipVT)
 * ---
 *
 * Methods:
 * --------
 *
 *  From lip.h:
 *  ----------
 *  + init:       LipOf(K,V)*, LipInitArgs -> int
 *  + clean_entry LipOf(K,V)*, EntryT*     -> void
 *  + del:        LipOf(K,V)*, K*          -> int 
 *  + clean:      LipOf(K,V)*              -> int 
 *
 *  From lipmap.h:
 *  -------------
 *  + get_or_set: LipOf(K,V)*, K*, V* -> V*
 *  + get:        LipOf(K,V)*, K*     -> V*
 *  + set:        LipOf(K,V)*, K*, V* -> int
 *
 * */


#include <stdbool.h>

#include <hotl.h>

/* Required params */
#ifndef Hotl_LipMap_KT
#error "Template type Hotl_LipMap_KT undefined"
#endif

#ifndef Hotl_LipMap_VT
#error "Template type Hotl_LipMap_VT undefined"
#endif

#define LipKT      Hotl_LipMap_KT
#define LipVT      Hotl_LipMap_VT
/* Optional Params */
#ifdef Hotl_LipMap_K_Hash
#define LipHash Hotl_LipMap_K_Hash
#endif 

#ifdef Hotl_LipMap_KT_Cmp
#define LipKTCmp Hotl_LipMap_KT_Cmp
#endif 

#ifdef Hotl_LipMap_KT_Cpy
#define LipKTCpy Hotl_LipMap_KT_Cpy
#endif 

#ifndef Hotl_LipMap_VT_Cpy 
#define LipVTCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(LipVT))
#else
#define LipVTCpy Hotl_LipMap_VT_Cpy
#endif // LipVTCpy
/* end of parameters */

#define EntryT     LipMapEntryOf(LipKT, LipVT)
#define Fun(FName) lipmapfn(LipKT,LipVT,FName)
#define LipType    LipMapOf(LipKT,LipVT)
typedef struct { LipKT k; LipVT v; } EntryT;
#include <lip.h>



/* Value Functions */

static inline LipVT*
Fun(get_or_set)(LipType* l, LipKT* k, LipVT* v) {
    if (Fun(is_zero)(k) && l->zero) return &l->zero->v; /* Ok: get zero */

    bool found = 0;
    EntryT* e = Fun(__insert)(l, k, &found);
    if (!e) return NULL;                                /* ERROR: could not insert */
    if (!found && LipVTCpy(&e->v, v)) return NULL;         /* ERROR: LipVTCpy failed */
    return &e->v;
}

static inline LipVT*
Fun(get)(LipType* l, LipKT* k) {
    EntryT* ent = Fun(__get_entry)(l,k );
    if (ent) return &ent->v;                          /* Ok: found */
    return NULL;                                      /* Ok: not found */
}


static inline int
Fun(set)(LipType* l, LipKT* k, LipVT* v) {
    bool found;
    EntryT* ent = Fun(__insert)(l, k, &found);
    if (!ent) return -1;                        /* __insert ERROR */
    if (LipVTCpy(&ent->v, v)) return -1;        /* LipVTCpy ERROR */
    return 0;                                   /* Ok */
}



/* END */
#undef LipKT
#undef EntryT
#undef Fun
#undef LipType
#undef LipVT
#undef LipVTCpy

/* Clear required parameters */
#undef Hotl_LipMap_KT
#undef Hotl_LipMap_VT

/* Clear optional parameters */
#ifdef Hotl_LipMap_K_Hash
#undef Hotl_LipMap_K_Hash
#endif

#ifdef Hotl_LipMap_KT_Cmp
#undef Hotl_LipMap_KT_Cmp
#endif

#ifdef Hotl_LipMap_KT_Cpy
#undef Hotl_LipMap_KT_Cpy
#endif

#ifdef Hotl_LipMap_VT_Cpy 
#undef Hotl_LipMap_VT_Cpy
#endif
