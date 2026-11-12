/* Copyright (C) 2026 Nicolas San Martin

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.*/

//#include <limits.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include "util.h"

static int process_line(std::unordered_map<std::string,size_t>& umap, char* line) {
    line = skip_space(line);
    while (*line) {
        char* wend = next_space(line);
        if (wend > line) {
            std::string s{line, wend};
            ++umap[s];
            line = skip_space(wend);
        }
    }
    return 0;
}

static int fill_table(std::unordered_map<std::string,size_t>& lip) {
    char* line = 0x0;
    size_t n = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &n, stdin)) != -1) {
        if (line && process_line(lip, line)) { puts("Error aborting"); free(line); return -1; }
    }
    free(line);
    return 0;
}


int run() {
    std::unordered_map<std::string,size_t> umap{};
    int err = fill_table(umap);
    if (err) { return -1; }


    for(auto it = umap.begin(); it != umap.end(); ++it) {
        std::cout << it->first << " -> " << it->second << '\n';
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        printf(
            "usage: STREAM | %s\n"
            "Prints the words and their frequency. Uses unordered map.",
            argv[0]
        );
        return 0;
    }

    run();
}

