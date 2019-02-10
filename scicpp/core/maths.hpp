#ifndef SCICPP_CORE_MATHS
#define SCICPP_CORE_MATHS

#include "scicpp/core/functional.hpp"
#include "scicpp/core/meta.hpp"

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
        // Handles negative zero
        // https://codereview.stackexchange.com/questions/60140/generic-absolute-value-function
        // Could use std::fpclassify(x) == FP_ZERO
        // to quiet warning, but this is not constexpr.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
        if (x == 0.) {
            return +0.;
        }
#pragma GCC diagnostic pop

        return (x < 0.) ? -x : x;
    }
}

//---------------------------------------------------------------------------------
// vectorized maths functions
//---------------------------------------------------------------------------------

// Trigonometric functions

const auto sin = vectorize([](auto x) { return std::sin(x); });
const auto cos = vectorize([](auto x) { return std::cos(x); });
const auto tan = vectorize([](auto x) { return std::tan(x); });
const auto arcsin = vectorize([](auto x) { return std::asin(x); });
const auto arccos = vectorize([](auto x) { return std::acos(x); });
const auto arctan = vectorize([](auto x) { return std::atan(x); });
const auto arctan2 = vectorize([](auto x, auto y) { return std::atan2(x, y); });
const auto hypot = vectorize([](auto x, auto y) { return std::hypot(x, y); });

// Hyperbolic functions

const auto sinh = vectorize([](auto x) { return std::sinh(x); });
const auto cosh = vectorize([](auto x) { return std::cosh(x); });
const auto tanh = vectorize([](auto x) { return std::tanh(x); });
const auto arcsinh = vectorize([](auto x) { return std::asinh(x); });
const auto arccosh = vectorize([](auto x) { return std::acosh(x); });
const auto arctanh = vectorize([](auto x) { return std::atanh(x); });

// Exponents and logarithms

const auto exp = vectorize([](auto x) { return std::exp(x); });
const auto expm1 = vectorize([](auto x) { return std::expm1(x); });
const auto exp2 = vectorize([](auto x) { return std::exp2(x); });
const auto log = vectorize([](auto x) { return std::log(x); });
const auto log2 = vectorize([](auto x) { return std::log2(x); });
const auto log1p = vectorize([](auto x) { return std::log1p(x); });

// Rounding

const auto around = vectorize([](auto x) { return std::round(x); });
const auto floor = vectorize([](auto x) { return std::floor(x); });
const auto ceil = vectorize([](auto x) { return std::ceil(x); });
const auto trunc = vectorize([](auto x) { return std::trunc(x); });
const auto rint = vectorize([](auto x) { return std::rint(x); });

// Complex numbers

const auto real = vectorize([](auto z) { return std::real(z); });
const auto imag = vectorize([](auto z) { return std::imag(z); });
const auto angle = vectorize([](auto z) { return std::arg(z); });
const auto norm = vectorize([](auto z) { return std::norm(z); });
const auto conj = vectorize([](auto z) { return std::conj(z); });
const auto polar =
    vectorize([](auto r, auto theta) { return std::polar(r, theta); });

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
        return std::fabs(x);
    }
});

const auto sqrt = vectorize([](auto x) { return std::sqrt(x); });
const auto cbrt = vectorize([](auto x) { return std::cbrt(x); });

} // namespace scicpp

#endif // SCICPPCORE_MATHS
