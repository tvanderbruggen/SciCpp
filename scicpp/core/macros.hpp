// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_MACROS
#define SCICPP_MACROS

#include <cassert>

#ifdef NDEBUG
#define SCICPP_UNREACHABLE __builtin_unreachable()
#else
#define SCICPP_UNREACHABLE assert(false)
#endif

//https://blog.regehr.org/archives/1096
#ifdef NDEBUG
#define SCICPP_REQUIRE(expr)                                                   \
    ((expr) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define SCICPP_REQUIRE(expr) assert((expr))
#endif

#define scicpp_pure __attribute__((pure))

#endif // SCICPP_MACROS