#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <fenv.h>
#include <limits.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include <stats.h>

static inline bool
is_sum_overflow(unsigned long long x, unsigned long long y) { return x > ULLONG_MAX - y; }

void stats_init(Stats st[static 1], size_t* xs, size_t n) {
    *st = (Stats) {
       .n = n,
       .xs = xs,
       .min = SIZE_MAX,
       .max = 0,
       .sum = 0,
       .ss = 0,
       .mean = 0
    };

    for (size_t i = 0; i < st->n; ++i) {
        st->sum += xs[i];
        st->ss += xs[i] * xs[i];
        st->min = st->min > xs[i] ? xs[i] : st->min;
        st->max = st->max < xs[i] ? xs[i] : st->max;
    }

    st->mean = (double)st->sum / (double)st->n;
    errno = 0; feclearexcept(FE_ALL_EXCEPT);
    st->stdev = sqrt((st->ss / st->n) - st->mean*st->mean);
    if (errno) {
        perror("error: sqrt failure");
        exit(-1);
    }
}

void print_freqs(Stats st[static 1]) {
    if (st->max < st->min) {
        perror("error max < min :(");
        return;
    }
    if (st->max == st->min) {
        puts("TODO: all eq :)");
        return;
    }
    stats_print(st);

    size_t range = 1 + st->max - st->min;
    size_t frqs[range];
    memset(frqs, 0, sizeof(frqs));

    for (size_t i = 0; i < st->n; ++i) {
        ++frqs[st->xs[i] - st->min];
    }
    
    for (size_t i = 0; i < range; ++i) {
        if (frqs[i])
            printf("%ld: %ld\n", i + st->min, frqs[i]);
    }

    puts("===");
    printf("#slots: %ld\n", st->n);
}
