// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_EQUAL
#define SCICPP_CORE_EQUAL

#include "scicpp/core/units.hpp"

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
          std::enable_if_t<!meta::is_iterable_v<T>, int> = 0,
          std::enable_if_t<!units::is_quantity_v<T>, int> = 0>
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
          typename Unit,
          typename Scale1,
          typename Scale2>
auto almost_equal(const units::quantity<T, Unit, Scale1> &q1,
                  const units::quantity<T, Unit, Scale2> &q2) {
    constexpr auto factor1 = T(Scale1::num) / T(Scale1::den);
    constexpr auto factor2 = T(Scale2::num) / T(Scale2::den);
    return almost_equal<rel_tol>(q1.value() * factor1, q2.value() * factor2);
}

template <int rel_tol = 1,
          class Array,
          std::enable_if_t<meta::is_iterable_v<Array>, int> = 0>
bool scicpp_pure almost_equal(const Array &f1, const Array &f2) {
    return std::equal(
        f1.cbegin(), f1.cend(), f2.cbegin(), f2.cend(), [](auto a, auto b) {
            return almost_equal<rel_tol>(a, b);
        });
}

} // namespace scicpp

#endif // SCICPP_CORE_EQUAL