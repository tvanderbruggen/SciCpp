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

namespace detail {

template <class Array>
auto quiet_nan() {
    return std::numeric_limits<typename Array::value_type>::quiet_NaN();
}

} // namespace detail

//---------------------------------------------------------------------------------
// amax
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto amax(const Array &f) {
    if (f.empty()) {
        return detail::quiet_nan<Array>();
    }

    return *std::max_element(f.cbegin(), f.cend());
}

//---------------------------------------------------------------------------------
// amin
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto amin(const Array &f) {
    if (f.empty()) {
        return detail::quiet_nan<Array>();
    }

    return *std::min_element(f.cbegin(), f.cend());
}

//---------------------------------------------------------------------------------
// ptp
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto ptp(const Array &f) {
    if (f.empty()) {
        return detail::quiet_nan<Array>();
    }

    const auto [it_min, it_max] = std::minmax_element(f.cbegin(), f.cend());
    return *it_max - *it_min;
}

//---------------------------------------------------------------------------------
// average
//---------------------------------------------------------------------------------

template <class Array>
constexpr auto average(const Array &f, const Array &weights) {
    if (f.empty() || (f.size() != weights.size())) {
        return detail::quiet_nan<Array>();
    }

    return inner(f, weights) / sum(weights);
}

//---------------------------------------------------------------------------------
// mean
//---------------------------------------------------------------------------------

template <class InputIt,
          class Predicate,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr T mean(InputIt first, InputIt last, Predicate filter) {
    if (std::distance(first, last) == 0) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto [res, cnt] = sum(first, last, filter);
    return res / T(cnt);
}

template <class Array, class Predicate>
constexpr auto mean(const Array &f, Predicate filter) {
    return mean(f.cbegin(), f.cend(), filter);
}

template <class Array>
constexpr auto mean(const Array &f) {
    return mean(f, filters::all);
}

template <class Array>
auto nanmean(const Array &f) {
    return mean(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// var
//---------------------------------------------------------------------------------

namespace detail {

// Pairwise recursive implementation of variance summation

template <class InputIt,
          class Predicate,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr auto variance_helper(InputIt first, InputIt last, Predicate filter) {
    constexpr long PW_BLOCKSIZE = 64;
    const auto size = std::distance(first, last);

    if (size <= PW_BLOCKSIZE) {
        const T m0 = mean(first, last, filter);

        const auto [res, cnt] =
            filter_reduce(first,
                          last,
                          [m0](auto r, auto v) {
                              const T diff = v - m0;
                              return r + diff * diff;
                              // Benchmark: this is slower on both GCC and Clang
                              // (and not constexpr)
                              // return std::fma(diff, diff, r);
                          },
                          T{0},
                          filter);

        return std::make_tuple(m0, res / T(cnt), cnt);
    } else {
        const auto [m1, var1, n1] =
            variance_helper(first, first + size / 2, filter);
        const auto [m2, var2, n2] =
            variance_helper(first + size / 2, last, filter);
        // Combine variances
        // https://www.emathzone.com/tutorials/basic-statistics/combined-variance.html
        const auto n_c = n1 + n2;
        const auto r = T{1} / T(n_c);
        const auto m_c = r * (T(n1) * m1 + T(n2) * m2);
        const auto var_c = r * (T(n1) * (var1 + (m1 - m_c) * (m1 - m_c)) +
                                T(n2) * (var2 + (m2 - m_c) * (m2 - m_c)));
        return std::make_tuple(m_c, var_c, n_c);
    }
}

} // namespace detail

template <class InputIt,
          class Predicate,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr auto var(InputIt first, InputIt last, Predicate filter) {
    if (std::distance(first, last) == 0) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto [m, v, c] = detail::variance_helper(first, last, filter);
    return v;
}

template <class Array, class Predicate>
constexpr auto var(const Array &f, Predicate filter) {
    return var(f.cbegin(), f.cend(), filter);
}

template <class Array>
constexpr auto var(const Array &f) {
    return var(f, filters::all);
}

template <class Array>
auto nanvar(const Array &f) {
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
