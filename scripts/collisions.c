#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <che.h>
#include <stats.h>
#include <util.h>

typedef struct { size_t* s; size_t n; } Slots;

void process_line(char* line, Slots* slots) {
    line = skip_space(line);
    while (*line) {
        char* wend = next_space(line);
        if (wend > line) {
            size_t h = che_djb2_k33(line, wend-line) % slots->n;
            ++slots->s[h];
        }
        line = skip_space(wend);
    }
}


int fill_table(Slots* slots) {
    char* line = 0x0;
    size_t n = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &n, stdin)) != -1) {
        if (!line) { puts("Error aborting"); free(line); return -1; }
        process_line(line, slots);
    }
    free(line);
    return 0;
}

int size_t_cmp(const void* x, const void* y) { return *(size_t*)x - *(size_t*)y; }

int run(const long nslots) {
    Slots slots = {.s=malloc(sizeof(size_t)*nslots),.n=nslots};
    if (!slots.s) { return -1; }
    memset(slots.s, 0, slots.n);

    int err = fill_table(&slots);
    
    qsort(slots.s, slots.n, sizeof(*slots.s), size_t_cmp);

    Stats st;
    stats_init(&st, slots.s, slots.n);
    print_freqs(&st);

    free(slots.s);
    return err;
}

#define COLLISIONS_HELP\
    "usage: STREAM | %s NSLOTS\n\n"\
    "Reads and hashes words from stdin and print for each collision count the number of slot,\n"\
    "i.e., the number of slots for each quantity of coalitions.\n\n"\
    "Ex:\n"\
    "cat /dev/urandom | tr -cd \"a-z \\n\" | head -c 1000000 | %s 10000\n"
int main(int argc, char* argv[]) {
    if (argc == 2) {
        const long nslots = strtol(argv[1], 0, 0);
        run(nslots);
    } else {
        printf(COLLISIONS_HELP, argv[0], argv[0]);
    }
}
