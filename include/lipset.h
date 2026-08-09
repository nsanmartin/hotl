/*
 * Lip (Linear probing hash set)
 *
 * Lip: T, THash, LipKTCmp, LipKTCpy -> LipOf(T)
 * ---
 *
 *
 *  From lip.h:
 *  ----------
 *  + init:       LipOf(K)*, LipInitArgs -> int
 *  + clean_entry LipOf(K)*, EntryT*     -> void
 *  + del:        LipOf(K)*, K*          -> int 
 *  + clean:      LipOf(K)*              -> int 
 *
 *  From lipset.h:
 *  -------------
 *  + find:       LipOf(K)*, K*          -> K*
 *  + insert:     LipOf(K)*, K*          -> int
 * */

#include <hotl.h>

/* Required param */
#ifndef Hotl_LipSet_T
#error "Template type Hotl_LipSet_T undefined"
#endif
#define LipKT      Hotl_LipSet_T
/* Optional Params */
#ifdef Hotl_LipSet_T_Cmp
#define LipKTCmp Hotl_LipSet_T_Cmp
#endif 

#ifdef Hotl_LipSet_T_Cpy
#define LipKTCpy Hotl_LipSet_T_Cpy
#endif 

#ifdef Hotl_LipSet_Hash
#define LipHash Hotl_LipSet_Hash
#endif 
/* end of parameters */

#define EntryT     LipSetEntryOf(LipKT)
#define Fun(FName) lipsetfn(LipKT,FName)
#define LipType    LipOf(LipKT)
typedef struct { LipKT k; } EntryT;
#include <lip.h>

static inline LipKT*
Fun(find)(LipType* l, LipKT* k) {
    EntryT* ent = Fun(__get_entry)(l,k);
    if (ent) return &ent->k;                          /* Ok: found */
    return NULL;                                      /* Ok: not found */
}


static inline int
Fun(insert)(LipType* l, LipKT* k) {
    bool found;
    EntryT* ent = Fun(__insert)(l, k, &found);
    if (!ent) return -1;                        /* __insert ERROR */
    return 0;                                   /* Ok */
}



/* END */
#undef LipKT
#undef LipType
#undef EntryT
#undef Fun

/* Clear required parameter */
#undef Hotl_LipSet_T
/* Clear optional parameters */
#ifdef Hotl_LipSet_Hash
#undef Hotl_LipSet_Hash
#endif

#ifdef Hotl_LipSet_T_Cmp
#undef Hotl_LipSet_T_Cmp
#endif

#ifdef Hotl_LipSet_T_Cpy
#undef Hotl_LipSet_T_Cpy
#endif

