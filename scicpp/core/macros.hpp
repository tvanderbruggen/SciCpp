// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_MACROS
#define SCICPP_MACROS

#include <cassert>
#include <type_traits>

#ifdef NDEBUG
#define SCICPP_UNREACHABLE __builtin_unreachable()
#else
#define SCICPP_UNREACHABLE assert(false)
#endif

//https://blog.regehr.org/archives/1096
#ifdef NDEBUG
#define scicpp_require(expr)                                                   \
    ((expr) ? static_cast<void>(0) : __builtin_unreachable())
#else

// We throw an exception on assert to be able to test
// for asserts using CATCH.
// https://github.com/catchorg/Catch2/issues/553

#include <stdexcept>

namespace detail {

inline constexpr void assert_impl(bool cond, const char *msg) {
    if (!cond) {
        throw std::runtime_error(msg);
    }
}

} // namespace detail

#define scicpp_require(x) ::detail::assert_impl((x), #x)
#endif

#define scicpp_pure __attribute__((pure))
#define scicpp_const __attribute__((const))

// Define a signed integer for sized
using signed_size_t = typename std::make_signed_t<std::size_t>;

#endif // SCICPP_MACROS