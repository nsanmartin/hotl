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

#define LipKt                       Hotl_LipSet_T
#define EntryT                      LipSetEntryOf(Hotl_LipSet_T)
typedef struct { Hotl_LipSet_T k; } EntryT;
#define Fun(FName)                  lipsetfn(Hotl_LipSet_T,FName)
#define LipType                     LipOf(Hotl_LipSet_T)

#ifndef LipKtCmp
#define LipKtCmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(Hotl_LipSet_T))
#endif // LipKtCmp

#ifndef LipKtCpy
#define LipKtCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(Hotl_LipSet_T))
#endif // LipKtCpy
#include <lip.h>

/* END */
#undef LipKt
#undef EntryT
#undef Hotl_LipSet_T
