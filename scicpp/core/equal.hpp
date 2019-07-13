// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_EQUAL
#define SCICPP_CORE_EQUAL

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/units.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <limits>
#include <numeric>

namespace scicpp {

//---------------------------------------------------------------------------------
// almost_equal
//---------------------------------------------------------------------------------

namespace detail {

template <typename T>
bool is_zero(T a) {
    return std::fpclassify(a) == FP_ZERO;
}

template <int rel_tol, typename T>
bool fp_equal_predicate(T a, T b) {
    static_assert(rel_tol >= 0);

    constexpr T tol = rel_tol * std::numeric_limits<T>::epsilon() / 2;

    if (std::isnan(a) && std::isnan(b)) {
        return true;
    }

    if (std::isinf(a) && std::isinf(b)) {
        return std::signbit(a) == std::signbit(b);
    }

    if (is_zero(a) || is_zero(b)) {
        return std::fabs(a - b) <= tol;
    }

    return std::fabs(a - b) <= tol * std::max(std::fabs(a), std::fabs(b));
}

} // namespace detail

template <int rel_tol = 1,
          typename T,
          meta::disable_if_iterable<T> = 0,
          units::disable_if_is_quantity<T> = 0>
bool almost_equal(T a, T b) {
    if constexpr (meta::is_complex_v<T>) {
        return (detail::fp_equal_predicate<rel_tol>(a.real(), b.real())) &&
               (detail::fp_equal_predicate<rel_tol>(a.imag(), b.imag()));
    } else {
        return detail::fp_equal_predicate<rel_tol>(a, b);
    }
}

template <int rel_tol = 1,
          typename T,
          typename Dim,
          typename Scale1,
          typename Scale2>
auto almost_equal(const units::quantity<T, Dim, Scale1> &q1,
                  const units::quantity<T, Dim, Scale2> &q2) {
    constexpr auto factor1 = T(Scale1::num) / T(Scale1::den);
    constexpr auto factor2 = T(Scale2::num) / T(Scale2::den);
    return almost_equal<rel_tol>(q1.value() * factor1, q2.value() * factor2);
}

template <int rel_tol = 1, class Array, meta::enable_if_iterable<Array> = 0>
bool scicpp_pure almost_equal(const Array &f1, const Array &f2) {
    return std::equal(
        f1.cbegin(), f1.cend(), f2.cbegin(), f2.cend(), [](auto a, auto b) {
            return almost_equal<rel_tol>(a, b);
        });
}

} // namespace scicpp

#endif // SCICPP_CORE_EQUAL