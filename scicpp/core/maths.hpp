#ifndef SCICPP_CORE_MATHS
#define SCICPP_CORE_MATHS

#include "scicpp/core/functional.hpp"
#include "scicpp/core/meta.hpp"

#include <array>
#include <cmath>
#include <complex>
#include <limits>
#include <vector>

namespace scicpp {

//---------------------------------------------------------------------------------
// fabs
//---------------------------------------------------------------------------------

template <typename T>
constexpr auto fabs(T &&x) {
    if constexpr (meta::is_iterable_v<T>) {
        return map([&](auto v) { return fabs(v); }, std::forward<T>(x));
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

// Hyperbolic functions

const auto sinh = vectorize([](auto x) { return std::sinh(x); });
const auto cosh = vectorize([](auto x) { return std::cosh(x); });
const auto tanh = vectorize([](auto x) { return std::tanh(x); });
const auto arcsinh = vectorize([](auto x) { return std::asinh(x); });
const auto arccosh = vectorize([](auto x) { return std::acosh(x); });
const auto arctanh = vectorize([](auto x) { return std::atanh(x); });

} // namespace scicpp

#endif // SCICPPCORE_MATHS
