// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_STATS
#define SCICPP_CORE_STATS

#include "scicpp/core/functional.hpp"
#include "scicpp/core/numeric.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>

namespace scicpp::stats {

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

template <class Array, class Predicate, typename T = typename Array::value_type>
constexpr T mean(const Array &f, Predicate filter) {
    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto [res, cnt] = sum(f, filter);
    return res / T(cnt);
}

template <class Array, typename T = typename Array::value_type>
constexpr T mean(const Array &f) {
    return mean(f, filters::all);
}

template <class Array, typename T = typename Array::value_type>
T nanmean(const Array &f) {
    return mean(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// var
//---------------------------------------------------------------------------------

template <class Array, class Predicate, typename T = typename Array::value_type>
constexpr T var(const Array &f, Predicate filter) {
    if (f.empty()) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const T m0 = mean(f, filter);
    const auto [res, cnt] =
        filter_reduce_associative(f.cbegin(),
                                  f.cend(),
                                  [m0](auto r, auto v) {
                                      const T diff = v - m0;
                                      return r + diff * diff;
                                      // Benchmark: this is slower on both GCC and Clang
                                      // (and not constexpr)
                                      //   return std::fma(diff, diff, r);
                                  },
                                  T{0},
                                  filter);
    return res / T(cnt);
}

template <class Array, typename T = typename Array::value_type>
constexpr T var(const Array &f) {
    return var(f, filters::all);
}

template <class Array, typename T = typename Array::value_type>
T nanvar(const Array &f) {
    return var(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// std
//---------------------------------------------------------------------------------

template <class Array>
auto std(const Array &a) {
    return std::sqrt(var(a));
}

template <class Array>
auto nanstd(const Array &a) {
    return std::sqrt(nanvar(a));
}

} // namespace scicpp::stats

#endif // SCICPP_CORE_STATS
