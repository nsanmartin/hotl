/* Copyright (C) 2026 Nicolas San Martin

Copying and distribution of this file, with or without modification,
are permitted in any medium without royalty provided the copyright
notice and this notice are preserved.  This file is offered as-is,
without any warranty.*/

#ifndef __HOTL_SCRIPTS_UTIL__
#define __HOTL_SCRIPTS_UTIL__

static inline char* skip_space(char* l) {
    while (*l && isspace(*l)) { ++l; }
    return l;
}

static inline char* next_space(char* l) {
    while (*l && !isspace(*l)) { ++l; }
    return l;
}

#endif
