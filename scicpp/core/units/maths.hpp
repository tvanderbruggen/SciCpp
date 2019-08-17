// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UNITS_MATHS
#define SCICPP_CORE_UNITS_MATHS

#include "scicpp/core/units/quantity.hpp"

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

template <intmax_t Root, typename T>
auto root(T x) {
    static_assert(Root > 0);

    if constexpr (Root == 1) {
        return x;
    } else if constexpr (Root == 2) {
        return sqrt(x);
    } else if constexpr (Root == 3) {
        return cbrt(x);
    } else {
        if constexpr (is_quantity_v<T>) {
            using rept_t = typename T::value_type;
            using DimRoot = dimension_root<typename T::dim, Root>;
            using ScalRoot = scale_root<typename T::scal, Root>;
            return quantity<rept_t, DimRoot, ScalRoot>(
                std::pow(value(x), rept_t{1} / rept_t(Root)));
        } else {
            return std::pow(x, T{1} / T(Root));
        }
    }
}

// TODO pow, hypot, ceil, floor

// Nearest integer floating point operations

// TODO  trunc, round, nearbyint, rint

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