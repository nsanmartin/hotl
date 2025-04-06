#include <stdio.h>
#include <utests.h>

int value_cmp(void* x, void* y) {
    return strcmp(x, y);
}

#define T int
#define TCmp value_cmp
#include <arl.h>

typedef struct { int x, y; float f; } Foo;
#define T Foo
#include <arl.h>

typedef char* cstr;
int str_copy(cstr* dst, const cstr* src) {
    *dst = strdup(*src);
    return *dst == 0x0;
}

int str_compare(cstr* s, cstr* t) { return strcmp(*s, *t); }
void strp_free(cstr* p) { free(*p); }
#define T cstr
#define TCpy str_copy
#define TCmp str_compare
#define TClean strp_free
#include <arl.h>

typedef Foo* Fooptr;
#define T Fooptr
#include <arl.h>

int arl_not_found(void) {
    int status = 1;
    ArlOf(int)* x = &(ArlOf(int)){0};

    int v = 1;
    int* ptr = arlfn(int, append)(x, &v);
    utest_assert(ptr, clean);

    int* value = arlfn(int, at)(x, 1);
    utest_assert(!value, clean);

    value = arlfn(int, at)(x, 0);
    utest_assert(value, clean);
    utest_assert(*value == v, clean);

    for (int i = 0; i < 100; ++i) {
        ptr = arlfn(int, append)(x, &i);
        utest_assert(ptr, clean);

        value = arlfn(int, at)(x, i + 2);
        utest_assert(!value, clean);
    }

    clean_and_ret(status, clean, arlfn(int, clean)(x));
}

int test_arl_1(void) {
    int status = 1;
    const size_t test_len = 10000;

    ArlOf(int)* x = &(ArlOf(int)){0};

    for (size_t i = 0; i < test_len; ++i) {
        int v = 3*i;
        int* ptr = arlfn(int, append)(x, &v);
        utest_assert(ptr, clean);
    }

    utest_assert(arl_len(x) == test_len, clean);

    for (size_t i = 0; i < test_len; ++i) {
        int* value = arlfn(int, at)(x, i);
        utest_assert(value, clean);
        utest_assert(*value == 3*(int)i, clean);
    }
    
    clean_and_ret(status, clean, arlfn(int, clean)(x));
}


int test_arl_2(void) {
    int status = 1;

    ArlOf(Foo)* x = &(ArlOf(Foo)){0};

    utest_assert(x->len == 0, clean);

    Foo* ptr = arlfn(Foo, append)(x, &(Foo){.x=4,.y=7,.f=0});
    utest_assert(ptr, clean);
    Foo* f = arlfn(Foo, at)(x, 0);
    utest_assert(f, clean);
    utest_assert(f->x == 4 && f->y == 7 && f->f == 0, clean);

    clean_and_ret(status, clean, arlfn(Foo, clean)(x));
}

int test_arl_string(void) {
    int status = 1;

    ArlOf(cstr)* x = &(ArlOf(cstr)){0};
    cstr s = "foo";
    cstr* ptr = arlfn(cstr, append)(x, &s);
    utest_assert(ptr, clean);

    s = "bar";
    ptr = arlfn(cstr, append)(x, &s);
    utest_assert(ptr, clean);

    cstr* v = arlfn(cstr, at)(x, 0);
    utest_assert(v, clean);
    utest_assert(strcmp("foo", *v) == 0, clean);

    v = arlfn(cstr, at)(x, 1);
    utest_assert(v, clean);
    utest_assert(strcmp("bar", *v) == 0, clean);


    clean_and_ret(status, clean, arlfn(cstr, clean)(x));
}


int test_arl_find_int(void) {
    int status = 1;

    ArlOf(int)* x = &(ArlOf(int)){0};
    //arl_int* x = &arl_empty(int);
    int v = 13;
    int* ptr = arlfn(int, append)(x, &v);
    //arl_append(x, 13);
    utest_assert(ptr, clean);
    v = 9;
    ptr = arlfn(int, append)(x, &v);
    utest_assert(ptr, clean);

    
    v = 13;
    int* intptr = arlfn(int, find)(x, &v);
    utest_assert(intptr, clean);

    v = 9;
    intptr = arlfn(int, find)(x, &v);
    utest_assert(intptr, clean);

    v = 1;
    intptr = arlfn(int, find)(x, &v);
    utest_assert(!intptr, clean);
    
    v = 0;
    intptr = arlfn(int, find)(x, &v);
    utest_assert(!intptr, clean);

    clean_and_ret(status, clean, arlfn(int, clean)(x));
}

int test_arl_find_string(void) {
    int status = 1;
    ArlOf(cstr)* x = &(ArlOf(cstr)){0};

    cstr s = "foo";
    cstr* ptr = arlfn(cstr, append)(x, &s);
    utest_assert(ptr, clean);

    s = "bar";
    ptr = arlfn(cstr, append)(x, &s);
    utest_assert(ptr, clean);

    s = "foo";
    cstr* strptr = arlfn(cstr, find)(x, &s);
    utest_assert(strptr, clean);

    s = "bar";
    strptr = arlfn(cstr, find)(x, &s);
    utest_assert(strptr, clean);

    s = "1";
    strptr = arlfn(cstr, find)(x, &s);
    utest_assert(!strptr, clean);

    s = "";
    strptr = arlfn(cstr, find)(x, &s);
    utest_assert(!strptr, clean);

    clean_and_ret(status, clean, arlfn(cstr, clean)(x));
}

int test_arl_it(void) {
    int status = 1;
    const int min = -999;
    const int max = 999;

    ArlOf(int)* x = &(ArlOf(int)){0};
    int* ptr = arlfn(int, append)(x, &min);
    utest_assert(ptr, clean);

    for (int i = min+ 1; i < max + 1; ++i) {
        ptr = arlfn(int, append)(x, &i);
        utest_assert(ptr, clean);
    }

    int* it = arlfn(int, begin)(x); utest_assert(it, clean);
    utest_assert(*it == min, clean);

    for (int i = min; i < max+1; ++i, ++it) {
        utest_assert(it, clean);
        utest_assert(*it == i, clean);
    }


    int i = min;
    int* end = arlfn(int, end)(x);
    for(it = arlfn(int, begin(x)) ; it != end; ++i, ++it) {
        utest_assert(*it == i, clean);
    }

    clean_and_ret(status, clean, arlfn(int, clean)(x));
}



int test_arl_find_str(void) {
    int status = 1;

    ArlOf(cstr)* x = &(ArlOf(cstr)){0};

    char* foo0 = strdup("foo");
    cstr* ptr = arlfn(cstr, append)(x, &foo0);
    free(foo0);
    utest_assert(ptr, clean);

    char* bar0 = strdup("bar");
    ptr = arlfn(cstr, append)(x, &bar0);
    free(bar0);
    utest_assert(ptr, clean);


    char* foo1 = strdup("foo");
    cstr* strptr = arlfn(cstr, find)(x, &foo1);
    free(foo1);
    utest_assert(strptr, clean);

    cstr lit = "foo";
    ptr = arlfn(cstr, find)(x, &lit);
    utest_assert(ptr, clean);

    lit = "foobar";
    ptr = arlfn(cstr, find)(x, &lit);
    utest_assert(!ptr, clean);

    char* bar1 = strdup("bar");
    ptr = arlfn(cstr, find)(x, &bar1);
    free(bar1);
    utest_assert(ptr, clean);

    char* one = strdup("1");
    ptr = arlfn(cstr, find)(x, &one);
    free(one);
    utest_assert(!ptr, clean);

    char* empty = strdup("");
    ptr = arlfn(cstr, find)(x, &empty);
    free(empty);
    utest_assert(!ptr, clean);


    clean_and_ret(status, clean, arlfn(cstr, clean)(x));
}

int main(void) {
    int failures = arl_not_found()
        + test_arl_1()
        + test_arl_2()
        + test_arl_string()
        + test_arl_find_int()
        + test_arl_find_string()
        + test_arl_it()
        + test_arl_find_str()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		printf("%s: Tests Ok!\n", __FILE__);

	}
	return failures;
}
