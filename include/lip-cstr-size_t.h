/* Copyright (C) 2026 Nicolas San Martin

This file is part of hotl.

hotl is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

hotl is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with hotl; see the file LICENSE or see
<https://www.gnu.org/licenses/>.  */

/*
 * The cstr type is the c string char* type.
 *
 * This name is consistent with standard lib functions
 * such as strcmp, strcpy, strdup, ...
 *
 */

#ifndef _LIP_CSTR_SIZE_T__
#define _LIP_CSTR_SIZE_T__

#include <hotl.h>
typedef char* cstr;

#define Hotl_LipMap_KT cstr
#define Hotl_LipMap_VT size_t
#define Hotl_LipMap_KTCmp cstrp_compare
#define Hotl_LipMap_KTCpy cstrp_copy
#define Hotl_LipMap_KTClean cstrp_clean
#ifndef Hotl_LipMap_Hash
#define Hotl_LipMap_Hash cstrp_hash
#endif


static inline int cstrp_compare(cstr* s1, cstr* s2) {
    if (*s1 == *s2) { return 0; }
    int null = (-(*s1 == 0) | (*s2 == 0));
    if (null) return null;
    return strcmp(*s1, *s2);
}

static inline int cstrp_copy(char** dest, char** src) {
    return (*dest = strdup(*src)) == NULL;
}
static inline int cstrp_hash(char** s) {
    return hotl_djb2_k33(*s, (size_t)strlen(*s));
}

static inline void cstrp_clean(char** s) { free(*s); }
#include <lipmap.h>

#endif // _LIP_STR_SIZE_T__
