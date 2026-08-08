/*
 * Lip (Linear probing hash set)
 *
 * Lip: T, THash, LipKTCmp, LipKTCpy -> LipOf(T)
 * ---
 *
 * Methods:
 * --------
 *  + clean:      LipOf(T)*              ->  int 
 *  + del:        LipOf(T)*, T*          ->  int 
 *  + get:        LipOf(T)*, T*          ->  T*
 *  + get_or_set: LipOf(T)*, T*          ->  T*
 *  + init:       LipOf(T)*, LipInitArgs ->  int
 *  + insert:     LipOf(T)*, T*          ->  int
 * */

#include <hotl.h>

#ifndef Hotl_LipSet_T
#error "Template type Hotl_LipSet_T undefined"
#endif

#define LipKT      Hotl_LipSet_T
#define EntryT     LipSetEntryOf(LipKT)
#define Fun(FName) lipsetfn(LipKT,FName)
#define LipType    LipOf(LipKT)
typedef struct { LipKT k; } EntryT;
#include <lip.h>

static inline int
Fun(get)(LipType* l, LipKT* k, LipKT* res[1]) {
    EntryT* ent = Fun(__get_entry)(l,k);
    if (ent) *res = &ent->k;                          /* Ok: found */
    else *res = NULL;
    return 0;                                        /* Ok: not found */
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
#undef Hotl_LipMap_KT
#undef Hotl_LipSet_T
