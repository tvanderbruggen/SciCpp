// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_STATS
#define SCICPP_CORE_STATS

#include "scicpp/core/functional.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/units.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <tuple>

namespace scicpp::stats {

namespace detail {

template <class Array>
auto quiet_nan() {
    using T = typename Array::value_type;
    using raw_t = units::representation_t<T>;
    return T(std::numeric_limits<raw_t>::quiet_NaN());
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

template <class Array1, class Array2>
constexpr auto average(const Array1 &f, const Array2 &weights) {
    if (f.empty() || (f.size() != weights.size())) {
        return detail::quiet_nan<Array1>();
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
    using raw_t = units::representation_t<T>;

    if (std::distance(first, last) == 0) {
        return T(std::numeric_limits<raw_t>::quiet_NaN());
    }

    const auto [res, cnt] = sum(first, last, filter);
    return res / raw_t(cnt);
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

template <class InputIt,
          class Predicate,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr auto var(InputIt first, InputIt last, Predicate filter) {
    using raw_t = units::representation_t<T>;
    using prod_t = std::conditional_t<units::is_quantity_v<T>,
                                      units::quantity_multiply<T, T>,
                                      T>;

    if (std::distance(first, last) == 0) {
        return prod_t(std::numeric_limits<raw_t>::quiet_NaN());
    }

    // Pairwise recursive implementation of variance summation
    const auto [m_, v_, c_] = pairwise_accumulate<64>(
        first,
        last,
        [&](auto f, auto l) {
            const auto m0 = mean(f, l, filter);

            // Cannot use structure binding directly here with GCC => bug ??
            // const auto [res, cnt] =
            const auto t = filter_reduce(f,
                                         l,
                                         [m0](auto r, auto v) {
                                             const auto diff = v - m0;
                                             return r + diff * diff;
                                             // Benchmark: this is slower on both GCC and Clang
                                             // (and also not constexpr)
                                             // return std::fma(diff, diff, r);
                                         },
                                         prod_t(0),
                                         filter);

            const auto [res, cnt] = t;
            return std::make_tuple(m0, res / raw_t(cnt), cnt);
        },
        [&](const auto res1, const auto res2) {
            // Combine variances
            // https://www.emathzone.com/tutorials/basic-statistics/combined-variance.html
            const auto [m1, var1, n1] = res1;
            const auto [m2, var2, n2] = res2;

            const auto n_c = n1 + n2;
            const auto r = raw_t{1} / raw_t(n_c);
            const auto m_c = r * (raw_t(n1) * m1 + raw_t(n2) * m2);
            const auto var_c =
                r * (raw_t(n1) * (var1 + (m1 - m_c) * (m1 - m_c)) +
                     raw_t(n2) * (var2 + (m2 - m_c) * (m2 - m_c)));
            return std::make_tuple(m_c, var_c, n_c);
        });

    return v_;
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
    using T = typename Array::value_type;

    if constexpr (units::is_quantity_v<T>) {
        return T(std::sqrt(var(a).value()));
    } else {
        return std::sqrt(var(a));
    }
}

template <class Array>
auto nanstd(const Array &a) {
    using T = typename Array::value_type;

    if constexpr (units::is_quantity_v<T>) {
        return T(std::sqrt(nanvar(a).value()));
    } else {
        return std::sqrt(nanvar(a));
    }
}

} // namespace scicpp::stats

#endif // SCICPP_CORE_STATS
