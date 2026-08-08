#include <stdio.h>
#include <utests.h>

#define Hotl_LipSet_T int
#include <lipset.h>             /* LipOf(int) */


int test_0(void) {
    int status = 1;
    LipOf(int)* x = &(LipOf(int)){0};

    /* size_t initsz = 4; */
    /* int err = lipfn(int,init)(x, (LipInitArgs){.sz=initsz}); */
    /* utest_assert(!err, clean); */
    /* utest_assert(lipsz(x) == initsz, clean); */
    /* clean_and_ret(status, clean, lipfn(int,clean)(x)); */

    //TODO:DEL
    (void)status;(void)x;
    return 0;
}


/* int test_1(void) { */
/*     int status = 1; */
/*     LipOf(int)* x = &(LipOf(int)){0}; */
/*     size_t initsz = 4; */
/*     int err = lipfn(int,init)(x, (LipInitArgs){.sz=initsz}); */
/*     utest_assert(!err, clean); */

/*     int k = 10; */
/*     err = lipfn(int,insert)(x, &k); */
/*     utest_assert(!err, clean); */

/*     int* g; */
/*     err = lipfn(int,get)(x, &k, &g); */
/*     utest_assert(!err, clean); */
/*     utest_assert(g, clean); */
/*     utest_assert(*g == k, clean); */

/*     k = 13; */
/*     err = lipfn(int,get)(x, &k, &g); */
/*     utest_assert(!err, clean); */
/*     utest_assert(!g, clean); */


/*     clean_and_ret(status, clean, lipfn(int,clean)(x)); */
/* } */

int main(void) {
    int failures 
        = test_0()
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
