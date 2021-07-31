// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CATCH
#define SCICPP_CATCH

#include "catch.hpp"

#include "scicpp/core/maths.hpp"

#include <limits>

// Simple float comparision to test constexpr functions
template <typename T>
constexpr bool float_equal(T a, T b) {
    constexpr auto eps = std::numeric_limits<double>::epsilon();
    return scicpp::fabs(a - b) <=
           eps * std::max(scicpp::fabs(a), scicpp::fabs(b));
}

#endif // SCICPP_CATCH
