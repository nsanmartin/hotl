#ifndef __HOTL_UTESTS_H__
#define __HOTL_UTESTS_H__

#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

enum { TestOk = 0, TestFail = 1 };

#define ASSERT_FAIL_MSG  RED "Test Failure: " RESET "assertion in line %d failed. "
#define utest_assert(Expr, Tag) do{ \
    if(!(Expr)) { fprintf(stderr, ASSERT_FAIL_MSG, __LINE__); goto Tag;} }while(0)

#define utest_assert_ret(Expr) do{ if(!(Expr)) { return -1 ;} }while(0)

#define clean_and_ret(Status, Tag, Cleanup) do{\
    Status = 0; \
Tag: \
    Cleanup; \
    return Status; \
} while (0)

#endif
