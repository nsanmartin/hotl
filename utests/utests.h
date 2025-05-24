#ifndef __HOTL_UTESTS_H__
#define __HOTL_UTESTS_H__

#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

enum { TestOk = 0, TestFail = 1 };

#define utest_assert_true(Expr, Tag) do{ \
    if(!(Expr)) { fprintf(stderr, "assertion in line %d failed. ", __LINE__); goto Tag;} }while(0)

#define utest_assert(Expr, Tag) do{ if(!(Expr)) { goto Tag;} }while(0)
#define utest_assert_ret(Expr) do{ if(!(Expr)) { return -1 ;} }while(0)

#define clean_and_ret(Status, Tag, Cleanup) do{\
    Status = 0; \
Tag: \
    Cleanup; \
    return Status; \
} while (0)

#endif
