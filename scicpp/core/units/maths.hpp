// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UNITS_MATHS
#define SCICPP_CORE_UNITS_MATHS

#include "scicpp/core/units/quantity.hpp"
#include "scicpp/core/units/units.hpp"

#include <cmath>
#include <limits>

// Extend std maths functions that are compatible with units
// (Mostly comparisons and power functions).

namespace scicpp::units {

// Classification and comparison

template <typename T>
bool isnan(T x) {
    return std::isnan(value(x));
}

template <typename T>
bool isinf(T x) {
    return std::isinf(value(x));
}

template <typename T>
bool isfinite(T x) {
    return std::isfinite(value(x));
}

template <typename T>
bool isnormal(T x) {
    return std::isnormal(value(x));
}

// TODO signbit, isgreater, isgreaterequal, isless, islessequal,  islessgreater
// isunordered

// Basic operations

template <typename T>
auto fabs(T x) {
    return T(std::fabs(value(x)));
}

template <typename T>
auto abs(T x) {
    return T(std::abs(value(x)));
}

template <typename T>
auto fmax(T x, T y) {
    return T(std::fmax(value(x), value(y)));
}

template <typename T>
auto fmin(T x, T y) {
    return T(std::fmin(value(x), value(y)));
}

template <typename T>
auto fdim(T x, T y) {
    return T(std::fdim(value(x), value(y)));
}

template <typename T1, typename T2, typename T3>
auto fma(T1 x, T2 y, T3 z) {
    if constexpr (is_quantity_v<T1> || is_quantity_v<T2>) {
        using prod_t = decltype(x * y);
        static_assert(is_same_dimension<prod_t, T3>);

        return prod_t(std::fma(value(x), value(y), value(z)));
    } else {
        return std::fma(x, y, z);
    }
}

// Power functions

template <typename T>
auto sqrt(T x) {
    if constexpr (is_quantity_v<T>) {
        using rept_t = typename T::value_type;
        using DimRoot = dimension_root<typename T::dim, 2>;
        using ScalRoot = scale_root<typename T::scal, 2>;
        return quantity<rept_t, DimRoot, ScalRoot>(std::sqrt(value(x)));
    } else {
        return std::sqrt(x);
    }
}

template <typename T>
auto cbrt(T x) {
    if constexpr (is_quantity_v<T>) {
        using rept_t = typename T::value_type;
        using DimRoot = dimension_root<typename T::dim, 3>;
        using ScalRoot = scale_root<typename T::scal, 3>;
        return quantity<rept_t, DimRoot, ScalRoot>(std::cbrt(value(x)));
    } else {
        return std::cbrt(x);
    }
}

// TODO pow

template <typename T>
auto hypot(T x, T y) {
    return T(std::hypot(value(x), value(y)));
}

template <typename T>
auto hypot(T x, T y, T z) {
    return T(std::hypot(value(x), value(y), value(z)));
}

// Trigonometric functions

namespace detail {

template <typename T>
constexpr inline bool is_dimensionless_like =
    is_dimensionless<T> || std::is_arithmetic_v<T> || meta::is_complex_v<T>;

template <typename T>
constexpr auto to_radian(T x) {
    if constexpr (meta::is_complex_v<T>) {
        return x;
    } else {
        static_assert(
            is_planar_angle<T>,
            "Trigonometric functions sin, cos, tan require an argument "
            "of type units::planar_angle (ex. radian or degree)");
        using rad = radian<typename T::value_type>;
        return quantity_cast<rad>(x).value();
    }
}

} // namespace detail

template <typename T>
auto sin(T x) {
    return std::sin(detail::to_radian(x));
}

template <typename T>
auto cos(T x) {
    return std::cos(detail::to_radian(x));
}

template <typename T>
auto tan(T x) {
    return std::tan(detail::to_radian(x));
}

template <typename T>
auto asin(T x) {
    static_assert(detail::is_dimensionless_like<T>,
                  "asin requires a dimensionless argument");
    return radian<representation_t<T>>(std::asin(value(x)));
}

template <typename T>
auto acos(T x) {
    static_assert(detail::is_dimensionless_like<T>,
                  "acos requires a dimensionless argument");
    return radian<representation_t<T>>(std::acos(value(x)));
}

template <typename T>
auto atan(T x) {
    static_assert(detail::is_dimensionless_like<T>,
                  "atan requires a dimensionless argument");
    return radian<representation_t<T>>(std::atan(value(x)));
}

template <typename T>
auto atan2(T x, T y) {
    return radian<representation_t<T>>(std::atan2(value(x), value(y)));
}

// Nearest integer floating point operations

template <typename T>
auto floor(T x) {
    return T(std::floor(value(x)));
}

template <typename T>
auto ceil(T x) {
    return T(std::ceil(value(x)));
}

template <typename T>
auto trunc(T x) {
    return T(std::trunc(value(x)));
}

template <typename T>
auto round(T x) {
    return T(std::round(value(x)));
}

template <typename T>
auto nearbyint(T x) {
    return T(std::nearbyint(value(x)));
}

template <typename T>
auto rint(T x) {
    return T(std::rint(value(x)));
}

// Floating point manipulation functions

// TODO nextafter, copysign

} // namespace scicpp::units

// Numeric limits

namespace std {

template <typename T, typename Dim, typename Scale, typename Offset>
struct numeric_limits<scicpp::units::quantity<T, Dim, Scale, Offset>> {
    static constexpr auto quiet_NaN() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::quiet_NaN());
    }

    static constexpr auto min() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::min());
    }

    static constexpr auto max() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::max());
    }

    static constexpr auto lowest() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::lowest());
    }

    static constexpr auto infinity() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::infinity());
    }

    static constexpr auto epsilon() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::epsilon());
    }

    static constexpr auto round_error() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::round_error());
    }

    static constexpr auto denorm_min() {
        return scicpp::units::quantity<T, Dim, Scale, Offset>(
            std::numeric_limits<T>::denorm_min());
    }
};

} // namespace std

#endif // SCICPP_CORE_UNITS_MATHS