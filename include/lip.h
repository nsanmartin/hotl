/*
 * Lip (Linear probing hash map)
 *
 * Lip: KT, VT, KHash, KTCmp, KTCpy -> LipOf(KT,VT)
 * ---
 *
 * Methods:
 * --------
 *
 *  + clean:      LipOf(KV,TV)* -> int 
 *  + del:        LipOf(KV,TV)*, KT* -> int 
 *  + get:        LipOf(KV,TV)*, KT* -> VT*
 *  + get_or_set: LipOf(KV,TV)*, KT*, VT* -> VT*
 *  + init:       LipOf(KV,TV)*, LipInitArgs -> int
 *  + set:        LipOf(KV,TV)*, KT*, VT* -> int
 *
 * */


#include <stdbool.h>

#include <hotl.h>

#ifndef KT
#error "Template type KT undefined"
#endif

#ifndef VT
#error "Template type VT undefined"
#endif


typedef struct { KT k; VT v; } LipEntryOf(KT, VT);
#define EntryT          LipEntryOf(KT, VT)
#define Fun(FName)  lipfn(KT,VT,FName)
#define LipType        LipOf(KT,VT)

//TODO: make KTClean : KT* -> ()
#if defined(KTClean) || defined(VTClean) 
static inline void
Fun(clean_entry)(EntryT* e) {
#ifdef KTClean
    KTClean(&e->k);
#endif //KTClean
#ifdef VTClean
    VTClean(&e->v);
#endif //VTClean
}
#define TClean Fun(clean_entry)
#endif // KTClean || VTClean

#define T EntryT
#include <buf.h>

#define lipsz(L) (L)->table.len
#define liptab(L) &(L)->table


#ifndef KTCmp
#define KTCmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(KT))
#endif // KTCmp

#ifndef KTCpy
#define KTCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(KT))
#endif // KTCpy

#ifndef VTCmp
#define VTCmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(VT))
#endif // VTCmp

#ifndef VTCpy
#define VTCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(VT))
#endif // VTCpy

typedef struct LipType {
    /* If a type's zero coincides with plain zero we nees to distinguish
     * it from empty slot */
    size_t attempts;
    size_t inserts;
    EntryT* zero;
    BufOf(EntryT) table;
} LipType;

static inline EntryT* Fun(__get_entry)(LipType* l, KT* k);
static inline EntryT* Fun(__insert)(LipType* l, KT* k, bool found[1]);

static inline int
Fun(init)(LipType* l, LipInitArgs p) {
    if (p.sz == 0) { return 0; }
    p.attempts = p.attempts ? p.attempts : LipMaximumAttempts;
    p.sz = p.sz ? p.sz : LipDefaultSize;
    l->attempts = p.sz < p.attempts ? p.sz : p.attempts;
    l->inserts = 0;
    l->zero = NULL;
    return !buffn(EntryT, calloc)(liptab(l), p.sz);
}

static inline int
Fun(is_zero)(KT* k) { return KTCmp(k, &(KT){0}) == 0; }

#ifndef KHash
#define KHash(S) hotl_djb2_k33((char*)S, sizeof(KT))
#endif // KHash

/*
 * return value:
 * + if value is found, `found` is set to true and a ptr returned
 * + if value is not found, `found` is set to false and a ptr to
 *   the entry were it should be returned.
 * + if nmovs >= attempts, found is set to false en NULL is returned
 * + if an unexpected error in at, found is set to true and NULL returned.
 */
static inline EntryT*
Fun(__find)(LipType* l, KT* k, bool found[1]) {
    size_t h = KHash(k) % l->table.len;
    size_t nmovs = 0;
    while (nmovs++ < l->attempts) {
        EntryT* e = buffn(EntryT, at)(&l->table, h);
        if (!e) { *found = true; return NULL; }                 /* AHRE_ERROR */
        if (KTCmp(k, &e->k) == 0) { *found = true; return e; }  /* Ok */
        if (l->zero != e && KTCmp(&e->k, &(KT){0}) == 0) {
            *found = false; return e;                           /* Ok */
        }
        h = (h + 1) % buflen(liptab(l));
    }
    *found = false;
    return NULL;                                                 /* Ok */
}

static inline int Fun(set)(LipType* l, KT* k, VT* v);
static inline VT* Fun(get)(LipType* l, KT* k);



static inline int
Fun(__dup)(LipType* l) {
    const size_t n = buflen(liptab(l));
    const size_t nX2 = n + n;
    if (nX2 < n) return -1;                                 /* size_t overflow */

    bool has_zero     = l->zero;
    EntryT zero_entry = (EntryT){0};
    if (has_zero) {
        EntryT* ent = Fun(__get_entry)(l, &(KT){0});
        if (!ent) return -1;                                /* __get_entry ERROR */
        zero_entry = *ent;
    }

    BufOf(EntryT) t0 = *liptab(l);
    *(liptab(l))     = (BufOf(EntryT)){0};
    size_t attempts  = nX2 < LipMaximumAttempts ? nX2 : LipMaximumAttempts;
    Fun(init)(l,(LipInitArgs){.sz=nX2,.attempts=attempts});

    if (has_zero) {
        bool found;
        l->zero = Fun(__insert)(l, &(KT){0}, &found);
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
Fun(__insert)(LipType* l, KT* k, bool found[1]) {
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
        if (KTCpy(&e->k, k)) return NULL;              /* KTCpy ERROR */
    }
    return e;                                          /* Ok */
} 


static inline VT* Fun(get_or_set)(LipType* l, KT* k, VT* v) {
    if (Fun(is_zero)(k)) {
        if (!l->zero) { return 0x0; }
        return &l->zero->v;
    }
    bool found = 0;
    EntryT* e = Fun(__find)(l, k, &found);
    while (!e && !found) {
        if (Fun(__dup)(l)) { return 0x0; }
        e = Fun(__find)(l, k, &found);
    }
    if (!e) { /*TODO ERROR or MAX TRIES?*/
        puts("Unxpected error.");
        return 0x0;
    }
    if (Fun(is_zero)(k)) { l->zero = e; }
    if (!found) {
        ++l->inserts;
        if (KTCpy(&e->k, k)) {
            puts("KTCpy failed");
            return 0x0;
        }
        if (VTCpy(&e->v, v)) {
            puts("VTCpy failed");
            return 0x0;
        }
    }
    return &e->v;
}


static inline EntryT*
Fun(__get_entry)(LipType* l, KT* k) {
    if (Fun(is_zero)(k)) {
        if (!l->zero) { return NULL; }
        return l->zero;
    }
    bool found = 0;
    EntryT* e = Fun(__find)(l, k, &found);
    if (!e) { /*TODO ERROR or MAX TRIES?*/ return NULL; }
    if (!found) {
        return 0x0;
    }
    return e;
}


//TODO0: Clean Entry
static inline int Fun(del)(LipType* l, KT* k) {
    if (Fun(is_zero)(k)) {
        if (!l->zero) { return -1; }
        l->zero = NULL;
        --l->inserts;
        return 0;
    }

    bool found = 0;
    EntryT* e = Fun(__find)(l, k, &found);
    if (!e || !found) { return -1; }
    --l->inserts;
    return KTCpy(&e->k, &(KT){0});
}


static inline void Fun(clean)(LipType*l) {
    buffn(EntryT, clean)(liptab(l));
}


/* Value Functions */


static inline VT*
Fun(get)(LipType* l, KT* k) {
    EntryT* res = Fun(__get_entry)(l,k);
    if (res) return &res->v;
    return NULL;
}


static inline int
Fun(set)(LipType* l, KT* k, VT* v) {
    bool found;
    EntryT* ent = Fun(__insert)(l, k, &found);
    if (!ent) return -1;
    if (VTCpy(&ent->v, v)) { 
        puts("VTCpy failed");
        return -1;
    }
    return 0;

}


#undef KT
#undef KHash 
#undef KTCmp
#undef KTCpy
#undef KTClean
#undef VT
#undef VTCmp
#undef VTCpy
#undef EntryT

