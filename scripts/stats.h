#ifndef __STATS_CHE__H_
#define __STATS_CHE__H_

typedef struct {
    size_t n;
    size_t* xs;
    size_t min;
    size_t max;
    double sum;
    double ss;
    double mean;
    double stdev;
} Stats;

void stats_init(Stats st[static 1], size_t* xs, size_t n);
void print_freqs(Stats st[static 1]);

static inline void stats_print(Stats st[static 1]) {
    printf("size_t n      : %lu\n",    st->n);
    printf("size_t min    : %lu\n",    st->min);
    printf("size_t max    : %lu\n",    st->max);
    printf("double sum    : %lf\n",    st->sum);
    printf("double mean   : %lf\n",    st->mean);
    printf("double stdev  : %lf\n",    st->stdev);
}

#endif
