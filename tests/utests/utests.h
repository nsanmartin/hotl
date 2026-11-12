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

#ifndef __HOTL_UTESTS_H__
#define __HOTL_UTESTS_H__

#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

enum { TestOk = 0, TestFail = 1 };

#define ASSERT_FAIL_MSG  RED "Test Failure: " RESET "assertion in line %d failed.\n"
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
