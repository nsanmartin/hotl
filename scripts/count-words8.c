#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <lip-cstr-size_t.h>

#include <rand-word.h>
#include <stats.h>

#define T size_t
#include <arl.h>


static int fill_table(LipOf(cstr,size_t)* ht, long n_words) {
#define WSZ 1 + 2 * sizeof(int)
    size_t zero = 0;
    char w[WSZ];
    char* wp = w;
    while (n_words --> 0) {
        fill_2_int_or_less_word_rand(w, WSZ - 1);
        size_t* v = lipfn(cstr,size_t,get_or_set)(
            ht,
            &wp,
            &zero
        );
        if (!v) { perror("Error aborting"); return -1; }
        *v = *v + 1;
    }
    return 0;
}

static int run(size_t initial_capacity, size_t n_words) {
    LipOf(cstr,size_t)* ht = &(LipOf(cstr,size_t)){0};
    int err = lipfn(cstr,size_t,init)(ht, (LipInitArgs){.sz=initial_capacity});
    if(err) { perror("lip init error"); return -1;}

    if (fill_table(ht, n_words)) {
        lipfn(cstr,size_t,clean)(ht);
        return EXIT_FAILURE;
    }
    
    typeof(lip_buf(ht))* buf = &lip_buf(ht);

    LipEntryOf(cstr,size_t)* it = buffn(LipEntryOf(cstr,size_t),begin)(buf);
    LipEntryOf(cstr,size_t)* end = buffn(LipEntryOf(cstr,size_t),end)(buf);

    for (; it != end; ++it) {
        if (it->k) printf("%s: %lu\n", it->k, it->v);
    }

    lipfn(cstr,size_t,clean)(ht);
    return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc == 4 || argc == 3) {
        const long N = strtol(argv[1], 0, 0);
        const long CAPACITY = strtol(argv[2], 0, 0);
        if (!CAPACITY) { perror("CAPACITY must not be zero, aborting."); return EXIT_FAILURE; }
        const long SEED = argc == 3 ? 8 : strtol(argv[3], 0, 0);
        srand(SEED);
        printf("N: %ld, [INITIAL_CAPACITY]: %ld, SEED: %ld\n", N, CAPACITY, SEED);
        return run(CAPACITY, N);
    } else {
        printf("usage %s N CAPACITY SEED\n", argv[0]);
    }
}
