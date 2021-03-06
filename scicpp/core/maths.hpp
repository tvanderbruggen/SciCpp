// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_MATHS
#define SCICPP_CORE_MATHS

#include "scicpp/core/functional.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/units/units.hpp"

#include <array>
#include <cmath>
#include <complex>
#include <limits>
#include <numeric>
#include <vector>

namespace scicpp {

//---------------------------------------------------------------------------------
// fabs
//
// std::fabs is not constexpr
//---------------------------------------------------------------------------------

template <typename T>
constexpr auto fabs(T &&x) {
    if constexpr (meta::is_iterable_v<T>) {
        return map([](auto v) { return fabs(v); }, std::forward<T>(x));
    } else {
        using U = std::decay_t<T>;
        // Handles negative zero
        // https://codereview.stackexchange.com/questions/60140/generic-absolute-value-function
        // Could use std::fpclassify(x) == FP_ZERO
        // to quiet warning, but this is not constexpr.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
        if (x == U(0.)) {
            return U(+0.);
        }
#pragma GCC diagnostic pop

        return (x < U(0.)) ? -x : x;
    }
}

//---------------------------------------------------------------------------------
// vectorized maths functions
//---------------------------------------------------------------------------------

// Trigonometric functions

const auto sin = vectorize([](auto x) { return units::sin(x); });
const auto cos = vectorize([](auto x) { return units::cos(x); });
const auto tan = vectorize([](auto x) { return units::tan(x); });

const auto arcsin = vectorize([](auto x) { return units::asin(x); });
const auto arccos = vectorize([](auto x) { return units::acos(x); });
const auto arctan = vectorize([](auto x) { return units::atan(x); });
const auto arctan2 =
    vectorize([](auto x, auto y) { return units::atan2(x, y); });
const auto hypot = vectorize([](auto x, auto y) { return units::hypot(x, y); });

// Hyperbolic functions

const auto sinh = vectorize([](auto x) { return std::sinh(x); });
const auto cosh = vectorize([](auto x) { return std::cosh(x); });
const auto tanh = vectorize([](auto x) { return std::tanh(x); });
const auto arcsinh = vectorize([](auto x) { return std::asinh(x); });
const auto arccosh = vectorize([](auto x) { return std::acosh(x); });
const auto arctanh = vectorize([](auto x) { return std::atanh(x); });

// Exponents and logarithms

const auto exp = vectorize([](auto x) { return units::exp(x); });
const auto expm1 = vectorize([](auto x) { return units::expm1(x); });
const auto exp2 = vectorize([](auto x) { return units::exp2(x); });
const auto log = vectorize([](auto x) { return units::log(x); });
const auto log2 = vectorize([](auto x) { return units::log2(x); });
const auto log1p = vectorize([](auto x) { return units::log1p(x); });

// Rounding

const auto around = vectorize([](auto x) { return units::round(x); });
const auto floor = vectorize([](auto x) { return units::floor(x); });
const auto ceil = vectorize([](auto x) { return units::ceil(x); });
const auto trunc = vectorize([](auto x) { return units::trunc(x); });
const auto rint = vectorize([](auto x) { return units::rint(x); });

// Complex numbers

const auto real = vectorize([](auto z) { return std::real(z); });
const auto imag = vectorize([](auto z) { return std::imag(z); });
const auto angle = vectorize([](auto z) { return std::arg(z); });
const auto norm = vectorize([](auto z) { return std::norm(z); });
const auto conj = vectorize([](auto z) { return std::conj(z); });

const auto polar = vectorize([](auto r, auto theta) {
    using T = decltype(theta);
    static_assert(units::is_planar_angle<T>,
                  "polar theta argument must be of type units::planar_angle "
                  "(ex. radian or degree)");
    using rad = units::radian<typename T::value_type>;
    return std::polar(r, units::quantity_cast<rad>(theta).value());
});

// Rational routines

const auto gcd =
    vectorize([](auto m, auto n) scicpp_const { return std::gcd(m, n); });
const auto lcm =
    vectorize([](auto m, auto n) scicpp_const { return std::lcm(m, n); });

// Miscellaneous

const auto absolute = vectorize([](auto x) {
    if constexpr (meta::is_complex_v<decltype(x)>) {
        return std::abs(x);
    } else {
        return units::fabs(x);
    }
});

const auto sqrt = vectorize([](auto x) { return units::sqrt(x); });
const auto cbrt = vectorize([](auto x) { return units::cbrt(x); });

} // namespace scicpp

#endif // SCICPPCORE_MATHS
