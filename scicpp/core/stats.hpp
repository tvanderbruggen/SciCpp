// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_STATS
#define SCICPP_CORE_STATS

#include "scicpp/core/functional.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <tuple>

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

template <class Array1, class Array2>
constexpr auto average(const Array1 &f, const Array2 &weights) {
    if (f.empty() || (f.size() != weights.size())) {
        return detail::quiet_nan<Array1>();
    }

    return inner(f, weights) / sum(weights);
}

//---------------------------------------------------------------------------------
// median
//---------------------------------------------------------------------------------

namespace detail {

// https://stackoverflow.com/questions/1719070/what-is-the-right-approach-when-using-stl-container-for-median-calculation
template <class InputIt>
auto median_inplace(InputIt first, InputIt last) {
    using T = typename std::iterator_traits<InputIt>::value_type;
    using raw_t = units::representation_t<T>;
    const auto size = std::distance(first, last);

    if (size == 0) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const signed_size_t half = size / 2;
    const auto target = first + half;
    std::nth_element(first, target, last);

    if (size % 2 != 0) { // vector size is odd
        return *target;
    } else {
        const auto max_it = std::max_element(first, first + half);
        return (*max_it + *target) / raw_t{2}; // cf. std::midpoint (C++20)
    }
}

} // namespace detail

template <class InputIt, class Predicate>
auto median(InputIt first, InputIt last, Predicate p) {
    auto v = filter(std::vector(first, last), p);
    return detail::median_inplace(v.begin(), v.end());
}

template <class Array, class Predicate>
auto median(const Array &f, Predicate filter) {
    return median(f.cbegin(), f.cend(), filter);
}

template <class Array>
auto median(Array &&f) {
    if constexpr (std::is_lvalue_reference_v<Array>) {
        auto tmp = f;
        return detail::median_inplace(tmp.begin(), tmp.end());
    } else {
        return detail::median_inplace(f.begin(), f.end());
    }
}

template <class Array>
auto nanmedian(const Array &f) {
    return median(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// mean
//---------------------------------------------------------------------------------

template <class InputIt, class Predicate>
constexpr auto mean(InputIt first, InputIt last, Predicate filter) {
    using T = typename std::iterator_traits<InputIt>::value_type;

    if (std::distance(first, last) == 0) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto [res, cnt] = sum(first, last, filter);
    return res / units::representation_t<T>(cnt);
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

template <class Array, typename T = typename Array::value_type>
constexpr auto tmean(const Array &f,
                     const std::array<T, 2> &limits,
                     const std::array<bool, 2> &inclusive = {true, true}) {
    return mean(f, filters::Trim<T>(limits, inclusive));
}

//---------------------------------------------------------------------------------
// var
//---------------------------------------------------------------------------------

template <int ddof = 0, class InputIt, class Predicate>
constexpr auto var(InputIt first, InputIt last, Predicate filter) {
    using T = typename std::iterator_traits<InputIt>::value_type;
    using raw_t = units::representation_t<T>;
    using prod_t = decltype(std::declval<T>() * std::declval<T>());

    if (std::distance(first, last) == 0) {
        return std::numeric_limits<prod_t>::quiet_NaN();
    }

    // Pairwise recursive implementation of variance summation
    const auto [m_, v_, c_] = pairwise_accumulate<64>(
        first,
        last,
        [&](auto f, auto l) {
            const auto m0 = mean(f, l, filter);

            const auto square_acc = [m0](auto r, auto v) {
                const auto diff = v - m0;
                return r + diff * diff;
                // Benchmark: this is slower on both GCC and Clang
                // (and also not constexpr)
                // return std::fma(diff, diff, r);
            };

            const auto [res, cnt] =
                filter_reduce(f, l, square_acc, prod_t(0), filter);
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

    if constexpr (ddof == 0) {
        return v_;
    } else {
        if (unlikely(c_ - ddof <= 0)) {
            return std::numeric_limits<decltype(v_)>::infinity();
        } else {
            return v_ * raw_t(c_) / raw_t(c_ - ddof);
        }
    }
}

template <int ddof = 0, class Array, class Predicate>
constexpr auto var(const Array &f, Predicate filter) {
    return var<ddof>(f.cbegin(), f.cend(), filter);
}

template <int ddof = 0, class Array>
constexpr auto var(const Array &f) {
    return var<ddof>(f, filters::all);
}

template <int ddof = 0, class Array>
auto nanvar(const Array &f) {
    return var<ddof>(f, filters::not_nan);
}

template <int ddof = 1, class Array, typename T = typename Array::value_type>
constexpr auto tvar(const Array &f,
                    const std::array<T, 2> &limits,
                    const std::array<bool, 2> &inclusive = {true, true}) {
    return var<ddof>(f, filters::Trim<T>(limits, inclusive));
}

//---------------------------------------------------------------------------------
// std
//---------------------------------------------------------------------------------

template <int ddof = 0, class Array>
auto std(const Array &a) {
    return units::sqrt(var<ddof>(a));
}

template <int ddof = 0, class Array>
auto nanstd(const Array &a) {
    return units::sqrt(nanvar<ddof>(a));
}

template <int ddof = 1, class Array, typename T = typename Array::value_type>
auto tstd(const Array &a,
            const std::array<T, 2> &limits,
            const std::array<bool, 2> &inclusive = {true, true}) {
    return units::sqrt(tvar<ddof>(a, limits, inclusive));
}

//---------------------------------------------------------------------------------
// moment
//---------------------------------------------------------------------------------

namespace detail {

template <intmax_t n>
const auto power_v = vectorize([](auto x) { return units::pow<n>(x); });

} // namespace detail

template <intmax_t n, class Array, class Predicate>
auto moment(const Array &f, [[maybe_unused]] Predicate filter) {
    using namespace operators;
    using T = typename Array::value_type;

    if constexpr (n == 0) {
        return T{1};
    } else if constexpr (n == 1) {
        return T{0};
    } else if constexpr (n == 2) {
        return var(f, filter);
    } else {
        // This allocates an extra array,
        // but preserves pairwise recursion precision
        return mean(detail::power_v<n>(f - mean(f, filter)), filter);
    }
}

template <intmax_t n, class Array>
auto moment(const Array &f) {
    return moment<n>(f, filters::all);
}

template <intmax_t n, class Array>
auto nanmoment(const Array &f) {
    return moment<n>(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// kurtosis
//---------------------------------------------------------------------------------

enum KurtosisDef { Fisher = 0, Pearson = 1 };

template <KurtosisDef def = KurtosisDef::Fisher, class Array, class Predicate>
auto kurtosis(const Array &f, Predicate filter) {
    const auto m2 = moment<2>(f, filter);
    const auto m4 = moment<4>(f, filter);
    const auto k = m4 / (m2 * m2);

    if constexpr (def == KurtosisDef::Fisher) {
        using T = decltype(k);
        return k - T(3);
    } else {
        return k;
    }
}

template <KurtosisDef def = KurtosisDef::Fisher, class Array>
auto kurtosis(const Array &f) {
    return kurtosis<def>(f, filters::all);
}

template <KurtosisDef def = KurtosisDef::Fisher, class Array>
auto nankurtosis(const Array &f) {
    return kurtosis<def>(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// skew
//---------------------------------------------------------------------------------

template <class Array, class Predicate>
auto skew(const Array &f, Predicate filter) {
    const auto m2 = moment<2>(f, filter);
    const auto m3 = moment<3>(f, filter);
    return m3 / units::sqrt(m2 * m2 * m2);
}

template <class Array>
auto skew(const Array &f) {
    return skew(f, filters::all);
}

template <class Array>
auto nanskew(const Array &f) {
    return skew(f, filters::not_nan);
}

} // namespace scicpp::stats

#endif // SCICPP_CORE_STATS
