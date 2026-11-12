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

#include <stdio.h>
#include <utests.h>

#define Hotl_LipSet_T int
#include <lipset.h>             /* LipOf(int) */

#define Hotl_LipSet_T char
int cmp_char(const char* s, const char* t) { return memcmp(s, t, sizeof(char)); }
#define Hotl_LipSet_T_Cmp cmp_char
int cpy_char(char* dest, const char* src) { memmove(dest, src, sizeof(char)); return 0; }
#define Hotl_LipSet_T_Cpy cpy_char
size_t hash_char(char* s) { return hotl_djb2_k33_with_seed(s, sizeof(char), 5381); }
#define Hotl_LipSet_Hash  hash_char
#include <lipset.h>             /* LipOf(char) */

int test_0(void) {
    int status = 1;
    LipOf(int)* x = &(LipOf(int)){0};

    size_t initsz = 4;
    int err = lipfn(int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);
    utest_assert(lipsz(x) == initsz, clean);
    clean_and_ret(status, clean, lipfn(int,clean)(x));

}


int test_1(void) {
    int status = 1;
    LipOf(int)* x = &(LipOf(int)){0};
    size_t initsz = 4;
    int err = lipfn(int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    int k = 10;
    err = lipfn(int,insert)(x, &k);
    utest_assert(!err, clean);

    int* g = lipfn(int,find)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == k, clean);

    k = 13;
    g = lipfn(int,find)(x, &k);
    utest_assert(!g, clean);


    clean_and_ret(status, clean, lipfn(int,clean)(x));
}

int test_2(void) {
    int status = 1;
    LipOf(int)* x = &(LipOf(int)){0};
    size_t initsz = 1000;
    int err = lipfn(int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    for (int i = -10; i < 10; ++i) {
        err = lipfn(int,insert)(x, &i);
        utest_assert(!err, clean);
    }

    for (int i = -10; i < 10; ++i) {
        int* p = lipfn(int,find)(x, &i);
        utest_assert(p, clean);
        utest_assert(*p == i, clean);
    }


    clean_and_ret(status, clean, lipfn(int,clean)(x));
}

int test_3(void) {
    int status = 1;
    LipOf(int)* x = &(LipOf(int)){0};
    size_t initsz = 10;
    int err = lipfn(int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

#define TEST_3_N 19231
    //insert TEST_3_N
    err = lipfn(int,insert)(x, &(int){TEST_3_N});
    utest_assert(!err, clean);

    //find TEST_3_N
    int* g = lipfn(int,find)(x, &(int){TEST_3_N});
    utest_assert(g, clean);
    utest_assert(*g == TEST_3_N, clean);

    //del TEST_3_N
    err = lipfn(int,del)(x, &(int){TEST_3_N});
    utest_assert(!err, clean);

    //find TEST_3_N (not found)
    g = lipfn(int,find)(x, &(int){TEST_3_N});
    utest_assert(!g, clean);


    //del TEST_3_N (error, undefined)
    err = lipfn(int,del)(x, &(int){TEST_3_N});
    utest_assert(err, clean);

    //clean and return
    clean_and_ret(status, clean, lipfn(int,clean)(x));
}


int test_4(void) {
    int status = 1;
    LipOf(char)* x = &(LipOf(char)){0};
    size_t initsz = 10;
    int err = lipfn(char,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    for (char c = 0; c < 200; ++c) {
        err = lipfn(char,insert)(x, &c);
        utest_assert(!err, clean);
    }

    for (char c = 0; c < 250; ++c) {
        char* elem = lipfn(char,find)(x, &c);
        if (c < 200) utest_assert(elem, clean);
        else  utest_assert(!elem, clean);
    }

    utest_assert(x->inserts == 200, clean);
    //clean and return
    clean_and_ret(status, clean, lipfn(char,clean)(x));
}


int main(void) {
    int failures 
        = test_0()
        + test_1()
        + test_2()
        + test_3()
        + test_4()
        ;

	if (failures) {
		fprintf(
            stderr,
            "%d test%s failed\n",
            failures,
            (failures == 1 ? "" : "s")
        );
	} else {
		printf("%s: Tests Ok!\n", __FILE__);
	}
	return failures;
}
