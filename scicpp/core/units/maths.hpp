// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UNITS_MATHS
#define SCICPP_CORE_UNITS_MATHS

#include "scicpp/core/units/quantity.hpp"

#include <cmath>

// Extend std maths function to be compatible with units,
// when this makes sense (Mostly power functions).

namespace scicpp::units {

template <typename T>
bool isnan(T x) {
    return std::isnan(value(x));
}

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

// TODO pow

// TODO hypot

} // namespace scicpp::units

#endif // SCICPP_CORE_UNITS_MATHS