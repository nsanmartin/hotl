/*
 * Lip (Linear probing hash map)
 *
 * Lip: EntryT, VT, THash, T_Cmp, LipKtCpy -> LipOf(EntryT,VT)
 * ---
 *
 * Methods:
 * --------
 *
 *  + clean:      LipOf(KV,TV)* -> int 
 *  + del:        LipOf(KV,TV)*, EntryT* -> int 
 *  + get:        LipOf(KV,TV)*, EntryT* -> VT*
 *  + get_or_set: LipOf(KV,TV)*, EntryT*, VT* -> VT*
 *  + init:       LipOf(KV,TV)*, LipInitArgs -> int
 *  + set:        LipOf(KV,TV)*, EntryT*, VT* -> int
 *
 * */


#include <stdbool.h>

#include <hotl.h>

// #ifndef Hotl_LipKt
// #error "Template type EntryT undefined"
// #endif

#ifndef LipKt
#error "Template type LipKt undefined"
#endif

#ifndef EntryT
#error "Template type EntryT undefined"
#endif


//TODO: make TClean : EntryT* -> ()
// #if defined(TClean) || defined(VTClean) 
// static inline void
// Fun(clean_entry)(EntryT* e) {
// #ifdef TClean
//     TClean(&e->k);
// #endif //TClean
// #ifdef VTClean
//     VTClean(&e->v);
// #endif //VTClean
// }
// #define TClean Fun(clean_entry)
// #endif // TClean || VTClean

#define T EntryT
#include <buf.h>

#define lipsz(L) (L)->table.len
#define liptab(L) &(L)->table



typedef struct LipType {
    /* If a type's zero coincides with plain zero we nees to distinguish
     * it from empty slot */
    size_t attempts;
    size_t inserts;
    EntryT* zero;
    BufOf(EntryT) table;
} LipType;

static inline int Fun(__get_entry)(LipType* l, LipKt* k, EntryT* ent[1]);
static inline EntryT* Fun(__insert)(LipType* l, LipKt* k, bool found[1]);

static inline int
Fun(init)(LipType* l, LipInitArgs p) {
    if (p.sz == 0) return 0;
    p.attempts = p.attempts ? p.attempts : LipMaximumAttempts;
    p.sz = p.sz ? p.sz : LipDefaultSize;
    l->attempts = p.sz < p.attempts ? p.sz : p.attempts;
    l->inserts = 0;
    l->zero = NULL;
    return !buffn(EntryT, calloc)(liptab(l), p.sz);
}

static inline int
Fun(is_zero)(LipKt* k) { return LipKtCmp(k, &(LipKt){0}) == 0; }

#ifndef THash
#define THash(S) hotl_djb2_k33((char*)S, sizeof(EntryT))
#endif // THash

/*
 * found==true and NULL is an error
 * return value:
 * + if value is found, `found` is set to true and a ptr returned
 * + if value is not found, `found` is set to false and a ptr to
 *   the entry were it should be returned.
 * + if nmovs >= attempts, found is set to false en NULL is returned
 * + if an unexpected error in at, found is set to true and NULL returned.
 */
static inline EntryT*
Fun(__find)(LipType* l, LipKt* k, bool found[1]) {
    size_t h = THash(k) % l->table.len;
    size_t nmovs = 0;
    while (nmovs++ < l->attempts) {
        EntryT* e = buffn(EntryT, at)(&l->table, h);
        if (!e) { *found = true; return NULL; }                 /* AHRE_ERROR */
        if (LipKtCmp(k, &e->k) == 0) { *found = true; return e; }  /* Ok */
        if (l->zero != e && LipKtCmp(&e->k, &(EntryT){0}) == 0) {
            *found = false; return e;                           /* Ok */
        }
        h = (h + 1) % buflen(liptab(l));
    }
    *found = false;
    return NULL;                                                 /* Ok */
}



static inline int
Fun(__dup)(LipType* l) {
    const size_t n = buflen(liptab(l));
    const size_t nX2 = n + n;
    if (nX2 < n) return -1;                                 /* size_t overflow */

    bool has_zero     = l->zero;
    EntryT zero_entry = (EntryT){0};
    if (has_zero) {
        EntryT* ent;
        if (Fun(__get_entry)(l, &(LipKt){0}, &ent)) return -1; /* __get_entry ERROR */
        if (!ent) return -1;                                /* ERROR: expecing zero */
        zero_entry = *ent;
    }

    BufOf(EntryT) t0 = *liptab(l);
    *(liptab(l))     = (BufOf(EntryT)){0};
    size_t attempts  = nX2 < LipMaximumAttempts ? nX2 : LipMaximumAttempts;
    Fun(init)(l,(LipInitArgs){.sz=nX2,.attempts=attempts});

    if (has_zero) {
        bool found;
        l->zero = Fun(__insert)(l, &(LipKt){0}, &found);
        if (!l->zero) return -1;                            /* __insert ERROR */
        *l->zero = zero_entry;
        zero_entry = (EntryT){0};
    }

    EntryT* it  = buffn(EntryT,begin)(&t0);
    EntryT* end = buffn(EntryT,end)(&t0);
    for(; it != end; ++it) {
        if (!Fun(is_zero)(&it->k)) {
            bool found;
            EntryT* e = Fun(__find)(l,&it->k,&found);
            if (!e || found) return -1;                     /* __find ERROR or duplicated entry ERROR */
            memmove(e, it, sizeof(EntryT));
        }
    }

    /* We 'move' the pointers/bytes, so no cleanup. */
    free((void*)t0.items);
    return 0;                                               /* Ok */
}

/* inserts key and return the entry. On error return NULL */
static inline EntryT*
Fun(__insert)(LipType* l, LipKt* k, bool found[1]) {
    *found = false;
    EntryT* e = Fun(__find)(l, k, found);
    if (!e && *found) return NULL;                     /* __find ERROR */
    while (!e && !*found) {
        if (Fun(__dup)(l)) { return NULL; }            /* __dup ERROR */
        e = Fun(__find)(l, k, found);
        if (!e && *found) return NULL;                 /* __find ERROR */
    }
    if (!e) return NULL;                               /* __find ERROR (redundant check) */

    if (Fun(is_zero)(k)) {
        *found = l->zero != NULL;
        l->zero = e;
    }

    if (!*found) {
        ++l->inserts;
        if (LipKtCpy(&e->k, k)) return NULL;              /* LipKtCpy ERROR */
    }
    return e;                                          /* Ok */
} 


static inline int
Fun(__get_entry)(LipType* l, LipKt* k, EntryT* ent[1]) {
    if (Fun(is_zero)(k)) {
        if (!l->zero)  *ent = NULL;
        else           *ent = l->zero;
        return 0;                   /* Ok */
    }

    bool found = 0;
    EntryT* e = Fun(__find)(l, k, &found);
    if (!found && e) {
        *ent = NULL;
        return 0;                   /* Ok: not found */
    }
    if (found && e) {
        *ent = e;
        return 0;                   /* Ok: not found */
    }
    return -1;                      /* ERROR */
}


//TODO0: Clean Entry
static inline int Fun(del)(LipType* l, LipKt* k) {
    if (Fun(is_zero)(k)) {
        if (!l->zero) return -1;        /* CLIENT ERROR: cannot delete zero */
        l->zero = NULL;
        --l->inserts;
        return 0;                       /* Ok: delete zero */
    }

    bool found = 0;
    EntryT* e = Fun(__find)(l, k, &found);

    if (!e && found) return -1;      /* HOTL ERROR */
    if (!e || !found) return -1;     /* CLIENT ERROR: not fouind */ 
    --l->inserts;
    return LipKtCpy(&e->k, &(EntryT){0});
}

static inline int
Fun(get_entry)(LipType* l, LipKt* k, EntryT* ent[1]) {
    return Fun(__get_entry)(l,k, ent);
}


static inline void Fun(clean)(LipType*l) {
    buffn(EntryT, clean)(liptab(l));
}


///* Value Functions */
//
//#ifdef VT
//static inline
//VT* Fun(get_or_set)(LipType* l, EntryT* k, VT* v) {
//    if (Fun(is_zero)(k) && l->zero) return &l->zero->v; /* Ok: get zero */
//
//    bool found = 0;
//    EntryT* e = Fun(__insert)(l, k, &found);
//    if (!e) return NULL;                                /* ERROR: could not insert */
//    if (!found && VTCpy(&e->v, v)) return NULL;         /* ERROR: VTCpy failed */
//    return &e->v;
//}
//
//static inline int
//Fun(get)(LipType* l, EntryT* k, VT* v[1]) {
//    EntryT* ent;
//    if (Fun(__get_entry)(l,k, &ent)) return -1;     /* __get_entry ERROR */
//    if (ent) *v = &ent->v;                          /* Ok: found */
//    else *v = NULL;
//    return 0;                                       /* Ok: not found */
//}
//
//
//static inline int
//Fun(set)(LipType* l, EntryT* k, VT* v) {
//    bool found;
//    EntryT* ent = Fun(__insert)(l, k, &found);
//    if (!ent) return -1;                        /* __insert ERROR */
//    if (VTCpy(&ent->v, v)) return -1;           /* VTCpy ERROR */
//    return 0;                                   /* Ok */
//}
//
//#else
//
//static inline int
//Fun(get)(LipType* l, EntryT* k, EntryT* res[1]) {
//    EntryT* ent;
//    if (Fun(__get_entry)(l,k, &ent)) return -1;     /* __get_entry ERROR */
//    if (ent) *res = &ent->k;                          /* Ok: found */
//    else *res = NULL;
//    return 0;                                       /* Ok: not found */
//}
//
//
//static inline int
//Fun(insert)(LipType* l, EntryT* k) {
//    bool found;
//    EntryT* ent = Fun(__insert)(l, k, &found);
//    if (!ent) return -1;                        /* __insert ERROR */
//    return 0;                                   /* Ok */
//}
//
//
//#endif


#undef EntryT
#undef THash 
#undef LipKtCmp
#undef LipKtCpy
#undef TClean
#undef VT
#undef VTCmp
#undef VTCpy
#undef EntryT
#undef Fun
