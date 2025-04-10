#include <stdio.h>
#include <rand-word.h>


int main(int argc, char* argv[]) {
    if (argc == 3) {
        const long N = strtol(argv[1], 0, 0);
        const long SEED = strtol(argv[2], 0, 0);
        srand(SEED);

        char w[1 + 2 * sizeof(int)];
        for (int i = 0; i < N; ++i) {
            size_t l = fill_2_int_or_less_word_rand(w);
            fwrite(w, 1, l + 1, stdout);
            fwrite(" ", 1, 1, stdout);
        }
        fwrite("\n", 1, 1, stdout);
    } else {
        printf("Generate random words.\nusage %s N SEED\n", argv[0]);
    }
}
