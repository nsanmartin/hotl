#ifndef __RAND_WORD_HOTL_H__
#define __RAND_WORD_HOTL_H__

#include <stdlib.h>

static inline void fill_int_word_rand(char* int_word) {
    int rand_int = rand();
    char* rand_int_str = (char*)&rand_int;
    for (size_t i = 0; i < sizeof(int); ++i) {
        unsigned char c = rand_int_str[i];
        int_word[i] = 'A' + (c % ('Z' - 'A'));
    }
}

static inline void fill_2_int_word_rand(char* w) {
    fill_int_word_rand(w);
    fill_int_word_rand(w + sizeof(int));
    w[2 * sizeof(int)] = '\0';
}

static inline int fill_2_int_or_less_word_rand(char* w, size_t max_wlen) {
    size_t wlen = 1 + rand() % max_wlen;
    fill_int_word_rand(w);
    if (wlen > sizeof(int)) { fill_int_word_rand(w + sizeof(int)); }
    w[wlen] = '\0';
    return wlen;
}

#endif
