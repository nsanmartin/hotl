/*
 * Lip (Linear probing hash map)
 *
 * Lip: EntryT, VT, LipHash, T_Cmp, LipKTCpy -> LipOf(EntryT,VT)
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


#ifndef LipKT
#error "Template type LipKT undefined"
#endif

#ifndef EntryT
#error "Template type EntryT undefined"
#endif

#ifndef LipKTCmp
#define LipKTCmp(S1, S2) hotl_compare_bytes(S1, S2, sizeof(LipKT))
#endif // LipKTCmp

#ifndef LipKTCpy
#define LipKTCpy(Dst, Src) hotl_copy_bytes(Dst, Src, sizeof(LipKT))
#endif // LipKTCpy


#if defined(LipKTClean) || defined(LipVTClean) 
static inline void
Fun(clean_entry)(EntryT* e) {
#ifdef LipKTClean
    LipKTClean(&e->k);
#endif //LipKTClean
#ifdef LipVTClean
    LipVTClean(&e->v);
#endif //LipVTClean
}
#endif // LipKTClean || LipVTClean

#define T EntryT
#if defined(LipKTClean) || defined(LipVTClean) 
#define TClean Fun(clean_entry)
#endif
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

static inline EntryT* Fun(__get_entry)(LipType* l, LipKT* k);
static inline EntryT* Fun(__insert)(LipType* l, LipKT* k, bool found[1]);

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
Fun(is_zero)(LipKT* k) { return LipKTCmp(k, &(LipKT){0}) == 0; }

#ifndef LipHash
#define LipHash(S) hotl_djb2_k33((char*)S, sizeof(LipKT))
#endif // LipHash

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
Fun(__find)(LipType* l, LipKT* k, bool found[1]) {
    size_t h = LipHash(k);
    size_t nmovs = 0;
    while (nmovs++ < l->attempts) {
        EntryT* e = &l->table.items[h % l->table.len];
        if (LipKTCmp(k, &e->k) == 0) { *found = true; return e; }   /* Ok */
        if (l->zero != e && LipKTCmp(&e->k, &(LipKT){0}) == 0) {
            *found = false; return e;                               /* Ok */
        }
        h = (h + 1);
    }
    *found = false;
    return NULL;                                                    /* Ok */
}



static inline int
Fun(__dup)(LipType* l) {
    const size_t n = buflen(liptab(l));
    const size_t nX2 = n + n;
    if (nX2 < n) return -1;                                 /* size_t overflow */

    bool has_zero     = l->zero;
    EntryT zero_entry = (EntryT){0};
    if (has_zero) {
        EntryT* ent = Fun(__get_entry)(l, &(LipKT){0});
        if (!ent) return -1;                                /* ERROR: expecing zero */
        zero_entry = *ent;
    }

    BufOf(EntryT) t0 = *liptab(l);
    *(liptab(l))     = (BufOf(EntryT)){0};
    size_t attempts  = nX2 < LipMaximumAttempts ? nX2 : LipMaximumAttempts;
    Fun(init)(l,(LipInitArgs){.sz=nX2,.attempts=attempts});

    if (has_zero) {
        bool found;
        l->zero = Fun(__insert)(l, &(LipKT){0}, &found);
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
Fun(__insert)(LipType* l, LipKT* k, bool found[1]) {
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
        if (LipKTCpy(&e->k, k)) return NULL;              /* LipKTCpy ERROR */
    }
    return e;                                          /* Ok */
} 


static inline EntryT*
Fun(__get_entry)(LipType* l, LipKT* k) {
    if (Fun(is_zero)(k)) return l->zero;

    bool found = 0;
    EntryT* e = Fun(__find)(l, k, &found);
    if (found && e) return e;
    return NULL;
}


//TODO0: Clean Entry
static inline int Fun(del)(LipType* l, LipKT* k) {
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
    return LipKTCpy(&e->k, &(LipKT){0});
}

static inline EntryT*
Fun(get_entry)(LipType* l, LipKT* k) {
    return Fun(__get_entry)(l,k);
}


static inline void Fun(clean)(LipType*l) {
    buffn(EntryT, clean)(liptab(l));
}



#undef LipHash 
#undef LipKTCmp
#undef LipKTCpy
#undef LipKTClean
