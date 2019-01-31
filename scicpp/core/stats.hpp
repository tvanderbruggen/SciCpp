// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_STATS
#define SCICPP_CORE_STATS

#include "scicpp/core/numeric.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

namespace scicpp {
namespace stats {

//---------------------------------------------------------------------------------
// amax
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto amax(const Array &f) {
    using T = typename Array::value_type;

    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return *std::max_element(f.cbegin(), f.cend());
}

//---------------------------------------------------------------------------------
// amin
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto amin(const Array &f) {
    using T = typename Array::value_type;

    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return *std::min_element(f.cbegin(), f.cend());
}

//---------------------------------------------------------------------------------
// ptp
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto ptp(const Array &f) {
    using T = typename Array::value_type;

    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto [it_min, it_max] = std::minmax_element(f.cbegin(), f.cend());
    return *it_max - *it_min;
}

//---------------------------------------------------------------------------------
// average
//---------------------------------------------------------------------------------

template <class Array, typename T = typename Array::value_type>
T average(const Array &f, const Array &weights) {
    if (f.empty() || (f.size() != weights.size())) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return std::inner_product(f.cbegin(), f.cend(), weights.cbegin(), T(0)) /
           sum(weights);
}

//---------------------------------------------------------------------------------
// mean
//---------------------------------------------------------------------------------

template <class Array, typename T = typename Array::value_type>
constexpr T mean(const Array &f) {
    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return sum(f) / T(f.size());
}

//---------------------------------------------------------------------------------
// var
//---------------------------------------------------------------------------------

template <class Array, typename T = typename Array::value_type>
constexpr T var(const Array &f) {
    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T m0 = mean(f);
    T acc = T(0);

    for (const auto &v : f) {
        const T diff = v - m0;
        acc += diff * diff;
    }

    return acc / T(f.size());
}

//---------------------------------------------------------------------------------
// std
//---------------------------------------------------------------------------------

template <class Array>
auto std(const Array &a) {
    return std::sqrt(var(a));
}

} // namespace stats
} // namespace scicpp

#endif // SCICPP_CORE_STATS
