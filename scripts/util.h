#ifndef __CHE_SCRIPTS_UTIL__
#define __CHE_SCRIPTS_UTIL__

static inline char* skip_space(char* l) {
    while (*l && isspace(*l)) { ++l; }
    return l;
}

static inline char* next_space(char* l) {
    while (*l && !isspace(*l)) { ++l; }
    return l;
}

#endif
