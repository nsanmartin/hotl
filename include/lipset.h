/*
 * Lip (Linear probing hash set)
 *
 * Lip: T, THash, LipKtCmp, LipKtCpy -> LipOf(T)
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

#define LipKt      Hotl_LipSet_T
#define EntryT     LipSetEntryOf(LipKt)
#define Fun(FName) lipsetfn(LipKt,FName)
#define LipType    LipOf(LipKt)
typedef struct { LipKt k; } EntryT;
#include <lip.h>

static inline int
Fun(get)(LipType* l, LipKt* k, LipKt* res[1]) {
    EntryT* ent;
    if (Fun(__get_entry)(l,k, &ent)) return -1;     /* __get_entry ERROR */
    if (ent) *res = &ent->k;                          /* Ok: found */
    else *res = NULL;
    return 0;                                       /* Ok: not found */
}


static inline int
Fun(insert)(LipType* l, LipKt* k) {
    bool found;
    EntryT* ent = Fun(__insert)(l, k, &found);
    if (!ent) return -1;                        /* __insert ERROR */
    return 0;                                   /* Ok */
}



/* END */
#undef LipKt
#undef LipType
#undef EntryT
#undef Fun
#undef Hotl_LipMap_Kt
#undef Hotl_LipSet_T
