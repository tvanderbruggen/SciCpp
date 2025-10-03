// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_STATS
#define SCICPP_CORE_STATS

#include "scicpp/core/equal.hpp"
#include "scicpp/core/functional.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <limits>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp::stats {

//---------------------------------------------------------------------------------
// amax
//---------------------------------------------------------------------------------

template <std::ranges::input_range R, class Proj = std::identity>
    requires std::indirect_strict_weak_order<
        std::less<>,
        std::projected<std::ranges::iterator_t<R>, Proj>>
[[nodiscard]] constexpr scicpp_pure auto amax(R &&r, Proj proj = {}) {
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;

    if (unlikely(std::ranges::empty(r))) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return *std::ranges::max_element(r, std::less<>{}, proj);
}

//---------------------------------------------------------------------------------
// amin
//---------------------------------------------------------------------------------

template <std::ranges::input_range R, class Proj = std::identity>
    requires std::indirect_strict_weak_order<
        std::less<>,
        std::projected<std::ranges::iterator_t<R>, Proj>>
[[nodiscard]] constexpr scicpp_pure auto amin(R &&r, Proj proj = {}) {
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;

    if (unlikely(std::ranges::empty(r))) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return *std::ranges::min_element(r, std::less<>{}, proj);
}

//---------------------------------------------------------------------------------
// ptp
//---------------------------------------------------------------------------------

template <std::ranges::input_range R>
    requires requires(const std::ranges::range_value_t<R> &x) { x - x; }
[[nodiscard]] constexpr scicpp_pure auto ptp(R &&r) {
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;

    if (unlikely(std::ranges::empty(r))) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    auto [it_min, it_max] = std::ranges::minmax_element(r, std::less<>{});
    return *it_max - *it_min;
}

//---------------------------------------------------------------------------------
// average
//---------------------------------------------------------------------------------

template <std::ranges::input_range R, std::ranges::input_range Weights>
[[nodiscard]] constexpr auto average(R &&r, Weights &&weights) {
    using T = std::remove_cvref_t<std::ranges::range_value_t<R>>;

    if (unlikely(std::ranges::empty(r) ||
                 (std::ranges::size(r) != std::ranges::size(weights)))) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    return inner(std::forward<R>(r), std::forward<Weights>(weights)) /
           sum(std::forward<Weights>(weights));
}

//---------------------------------------------------------------------------------
// median
//---------------------------------------------------------------------------------

namespace detail {

// https://stackoverflow.com/questions/1719070/what-is-the-right-approach-when-using-stl-container-for-median-calculation
template <std::input_iterator It, std::sentinel_for<It> S>
[[nodiscard]] constexpr auto median_inplace(It first, S last) {
    using T = std::iter_value_t<It>;
    const auto size = std::distance(first, last);

    if (unlikely(size == 0)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const signed_size_t half = size / 2;
    const auto target = first + half;
    std::nth_element(first, target, last);

    if (size % 2 != 0) { // vector size is odd
        return *target;
    } else {
        const auto max_it = std::max_element(first, first + half);
        return units::midpoint(*max_it, *target);
    }
}

} // namespace detail

template <std::input_iterator It, std::sentinel_for<It> S, class Predicate>
[[nodiscard]] auto median(It first, S last, Predicate &&pred) {
    using T = std::iter_value_t<It>;
    auto v = filter(std::vector<T>(first, last), std::forward<Predicate>(pred));

    return detail::median_inplace(v.begin(), v.end());
}

template <std::ranges::input_range R, class Predicate>
[[nodiscard]] auto median(R &&r, Predicate &&pred) {
    using T = std::ranges::range_value_t<R>;
    auto v = filter(std::vector<T>(std::begin(r), std::end(r)),
                    std::forward<Predicate>(pred));
    return detail::median_inplace(v.begin(), v.end());
}

template <std::ranges::input_range R>
[[nodiscard]] constexpr auto median(R &&r) {
    constexpr bool can_be_done_inplace =
        std::ranges::random_access_range<R> && !std::is_lvalue_reference_v<R> &&
        !std::is_const_v<
            std::remove_reference_t<std::ranges::range_reference_t<R>>>;

    if constexpr (can_be_done_inplace) {
        return detail::median_inplace(std::begin(r), std::end(r));
    } else {
        using T = std::ranges::range_value_t<R>;
        constexpr std::size_t N = meta::range_size_v<std::remove_cvref_t<R>>;

        // If size known at compile-time copy in std::array else use std::vector
        if constexpr (N != std::dynamic_extent &&
                      std::is_default_constructible_v<T>) {
            std::array<T, N> buf{}; // requires T default-constructible
            std::ranges::copy(r, buf.begin());
            return detail::median_inplace(buf.begin(), buf.end());
        } else {
            std::vector<T> v(std::begin(r), std::end(r));
            return detail::median_inplace(v.begin(), v.end());
        }
    }
}

template <std::ranges::input_range R>
[[nodiscard]] auto nanmedian(R &&r) {
    return median(std::forward<R>(r), filters::not_nan);
}

//---------------------------------------------------------------------------------
// quantile, percentile, iqr
//---------------------------------------------------------------------------------

enum class QuantileInterp : int { LOWER, HIGHER, NEAREST, MIDPOINT, LINEAR };

namespace detail {

// TODO move to math header
template <class T>
constexpr T cxx20_floor(T x) {
    if constexpr (std::is_integral_v<T>) {
        return x;
    }

    if (std::is_constant_evaluated()) {
        auto i = static_cast<long long>(x);
        T ti = static_cast<T>(i);
        return ti > x ? static_cast<T>(i - 1) : ti;
    } else {
        using std::floor;
        return floor(x);
    }
}

template <class T>
constexpr T cxx20_ceil(T x) {
    if constexpr (std::is_integral_v<T>) {
        return x;
    }

    if (std::is_constant_evaluated()) {
        auto i = static_cast<long long>(x);
        T ti = static_cast<T>(i);
        return ti < x ? static_cast<T>(i + 1) : ti;
    } else {
        using std::ceil;
        return ceil(x);
    }
}

// tie-breaking: “nearest, ties to away-from-zero” (close to default FE_TONEAREST for non-.5 cases)
template <class T>
constexpr T cxx20_nearbyint(T x) {
    if constexpr (std::is_integral_v<T>) {
        return x;
    }

    if (std::is_constant_evaluated()) {
        return x >= T{0} ? cxx20_floor(x + T{0.5}) : cxx20_ceil(x - T{0.5});
    } else {
        using std::nearbyint;
        return nearbyint(x);
    }
}

template <QuantileInterp interpolation, class T>
[[nodiscard]] constexpr T quantile_interp_index(T h) {
    if constexpr (interpolation == QuantileInterp::LOWER) {
        return cxx20_floor(h);
    } else if constexpr (interpolation == QuantileInterp::HIGHER) {
        return cxx20_ceil(h);
    } else if constexpr (interpolation == QuantileInterp::NEAREST) {
        return cxx20_nearbyint(h);
    } else if constexpr (interpolation == QuantileInterp::MIDPOINT) {
        return std::midpoint(cxx20_floor(h), cxx20_ceil(h));
    } else { // LINEAR
        return h;
    }
}

// https://stackoverflow.com/questions/28548703/why-does-stdnth-element-return-sorted-vectors-for-input-vectors-with-n-33-el
template <QuantileInterp interpolation,
          std::input_iterator It,
          std::sentinel_for<It> S,
          typename T>
[[nodiscard]] constexpr auto quantile_inplace(It first, S last, T q) {
    scicpp_require(q >= T{0} && q <= T{1});

    using ItTp = std::iter_value_t<It>;
    using RetTp = std::conditional_t<std::is_integral_v<ItTp>, double, ItTp>;

    const auto size = std::distance(first, last);

    if (unlikely(size == 0)) {
        return std::numeric_limits<RetTp>::quiet_NaN();
    }

    if (size == 1) {
        return RetTp(*first);
    }

    const auto h0 =
        quantile_interp_index<interpolation>(q * static_cast<T>(size - 1));

    if (float_equal(cxx20_nearbyint(h0), h0)) { // h0 is an integer
        const auto n0 = std::min(first + signed_size_t(h0), last);
        std::nth_element(first, n0, last);
        return RetTp(*n0);
    } else { // h0 not an integral index
        const auto h_low = signed_size_t(h0);
        const auto n_high = first + std::min(h_low + 1, size - 1);
        std::nth_element(first, n_high, last);
        const auto x_low = *std::max_element(first, n_high);
        const auto x_high = *n_high;
        return lerp(x_low, x_high, h0 - std::floor(h_low));
    }
}

} // namespace detail

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::input_iterator It,
          std::sentinel_for<It> S,
          class Predicate,
          typename T>
[[nodiscard]] auto quantile(It first, S last, T q, Predicate &&p) {
    using ItTp = std::iter_value_t<It>;
    auto v = filter(std::vector<ItTp>(first, last), std::forward<Predicate>(p));
    return detail::quantile_inplace<interpolation>(v.begin(), v.end(), q);
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R,
          class Predicate,
          typename T>
[[nodiscard]] auto quantile(const R &r, T q, Predicate &&filter) {
    return quantile<interpolation>(
        std::cbegin(r), std::cend(r), q, std::forward<Predicate>(filter));
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R,
          typename T>
[[nodiscard]] constexpr auto quantile(R &&r, T q) {
    constexpr bool can_be_done_inplace =
        std::ranges::random_access_range<R> && !std::is_lvalue_reference_v<R> &&
        !std::is_const_v<
            std::remove_reference_t<std::ranges::range_reference_t<R>>>;

    if constexpr (can_be_done_inplace) {
        return detail::quantile_inplace<interpolation>(
            std::begin(r), std::end(r), q);
    } else {
        using RTp = std::ranges::range_value_t<R>;
        constexpr std::size_t N = meta::range_size_v<std::remove_cvref_t<R>>;

        // If size known at compile-time copy in std::array else use std::vector
        if constexpr (N != std::dynamic_extent &&
                      std::is_default_constructible_v<RTp>) {
            std::array<RTp, N> buf{}; // requires T default-constructible
            std::ranges::copy(r, buf.begin());
            return detail::quantile_inplace<interpolation>(
                buf.begin(), buf.end(), q);
        } else {
            std::vector<RTp> v(std::begin(r), std::end(r));
            return detail::quantile_inplace<interpolation>(
                v.begin(), v.end(), q);
        }
    }
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R,
          typename T>
[[nodiscard]] auto nanquantile(R &&r, T q) {
    return quantile<interpolation>(std::forward<R>(r), q, filters::not_nan);
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::input_iterator It,
          std::sentinel_for<It> S,
          class Predicate,
          typename T>
[[nodiscard]] auto percentile(It first, S last, T p, Predicate &&filter) {
    return quantile<interpolation>(
        first, last, p / 100., std::forward<Predicate>(filter));
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R,
          class Predicate,
          typename T>
[[nodiscard]] auto percentile(const R &r, T p, Predicate &&filter) {
    return quantile<interpolation>(
        r, p / 100., std::forward<Predicate>(filter));
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R,
          typename T>
[[nodiscard]] constexpr auto percentile(R &&r, T p) {
    return quantile<interpolation>(std::forward<R>(r), p / 100.);
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R,
          typename T>
[[nodiscard]] auto nanpercentile(R &&r, T p) {
    return nanquantile<interpolation>(std::forward<R>(r), p / 100.);
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R>
[[nodiscard]] constexpr auto iqr(R &&r, double rng0 = 25., double rng1 = 75.) {
    const auto pct0 = percentile<interpolation>(std::forward<R>(r), rng0);
    const auto pct1 = percentile<interpolation>(std::forward<R>(r), rng1);
    return pct1 - pct0;
}

template <QuantileInterp interpolation = QuantileInterp::LINEAR,
          std::ranges::input_range R>
[[nodiscard]] auto naniqr(R &&r, double rng0 = 25., double rng1 = 75.) {
    const auto pct0 = nanpercentile<interpolation>(std::forward<R>(r), rng0);
    const auto pct1 = nanpercentile<interpolation>(std::forward<R>(r), rng1);
    return pct1 - pct0;
}

//---------------------------------------------------------------------------------
// mean
//---------------------------------------------------------------------------------

template <class InputIt, class Predicate>
constexpr auto mean(InputIt first, InputIt last, Predicate filter) {
    using T = std::iterator_traits<InputIt>::value_type;

    if (unlikely(std::distance(first, last) == 0)) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    const auto [res, cnt] = sum(first, last, filter);
    return res / units::representation_t<T>(static_cast<int>(cnt));
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

template <class Array, typename T = Array::value_type>
constexpr auto tmean(const Array &f,
                     const std::array<T, 2> &limits,
                     const std::array<bool, 2> &inclusive = {true, true}) {
    return mean(f, filters::Trim<T>(limits, inclusive));
}

//---------------------------------------------------------------------------------
// gmean
//---------------------------------------------------------------------------------

template <class Array>
auto gmean(Array &&f) {
    using T = std::decay_t<Array>::value_type;

    if (unlikely(f.empty())) {
        return std::numeric_limits<T>::quiet_NaN();
    }

    if constexpr (units::is_quantity_v<T>) {
        using namespace operators;
        return T(std::exp(mean(log(std::forward<Array>(f) / T(1)))));
    } else {
        return std::exp(mean(log(std::forward<Array>(f))));
    }
}

template <class Array, class Predicate>
auto gmean(Array &&f, Predicate p) {
    return gmean(filter(std::forward<Array>(f), p));
}

template <class Array>
auto nangmean(Array &&f) {
    return gmean(std::forward<Array>(f), filters::not_nan);
}

//---------------------------------------------------------------------------------
// covariance
//---------------------------------------------------------------------------------

template <int ddof = 0, class InputIt1, class InputIt2, class Predicate>
constexpr scicpp_pure auto covariance(InputIt1 first1,
                                      InputIt1 last1,
                                      InputIt2 first2,
                                      InputIt2 last2,
                                      Predicate filter) {
    using T1 = std::iterator_traits<InputIt1>::value_type;
    using T2 = std::iterator_traits<InputIt2>::value_type;
    using raw_t1 = units::representation_t<T1>;
    using raw_t2 = units::representation_t<T2>;
    using raw_t = std::common_type_t<raw_t1, raw_t2>;
    using prod_t = decltype(std::declval<T1>() * std::declval<T2>());

    static_assert(meta::is_predicate<Predicate, T1>);
    static_assert(meta::is_predicate<Predicate, T2>);

    scicpp_require(std::distance(first1, last1) ==
                   std::distance(first2, last2));

    if (unlikely(std::distance(first1, last1) == 0)) {
        return std::tuple{std::numeric_limits<prod_t>::quiet_NaN(),
                          signed_size_t(0)};
    }

    // Pairwise recursive implementation of covariance summation
    const auto [m1_, m2_, cov_, c_] = pairwise_accumulate<64>(
        first1,
        last1,
        first2,
        last2,
        [&](auto f1, auto l1, auto f2, auto l2) {
            const auto m1 = mean(f1, l1, filter);
            const auto m2 = mean(f2, l2, filter);

            auto res = utils::set_zero<prod_t>();
            signed_size_t cnt = 0;

            for (; f1 != l1; ++f1, (void)++f2) {
                if (filter(*f1) && filter(*f2)) {
                    if constexpr (meta::is_complex_v<T2>) {
                        res += (*f1 - m1) * std::conj(*f2 - m2);
                    } else {
                        res += (*f1 - m1) * (*f2 - m2);
                    }
                    cnt++;
                }
            }

            return std::tuple{m1, m2, res, cnt};
        },
        [&](const auto res1, const auto res2) {
            // Combine covariances
            // https://stackoverflow.com/questions/45773857/merging-covariance-from-two-sets-to-create-new-covariance
            const auto [m11, m12, covar1, n1] = res1;
            const auto [m21, m22, covar2, n2] = res2;

            const auto n_c = n1 + n2;
            const auto m1_c = (raw_t1{1} / raw_t1(static_cast<int>(n_c))) *
                              (raw_t1(static_cast<int>(n1)) * m11 +
                               raw_t1(static_cast<int>(n2)) * m21);
            const auto m2_c = (raw_t2{1} / raw_t2(static_cast<int>(n_c))) *
                              (raw_t2(static_cast<int>(n1)) * m12 +
                               raw_t2(static_cast<int>(n2)) * m22);
            const auto covar_c =
                covar1 + covar2 +
                (raw_t(static_cast<int>(n1)) * raw_t(static_cast<int>(n2)) /
                 raw_t(static_cast<int>(n_c))) *
                    conj(m12 - m22) * (m11 - m21);
            return std::tuple{m1_c, m2_c, covar_c, n_c};
        });

    if (unlikely(c_ - ddof <= 0)) {
        return std::tuple{std::numeric_limits<decltype(cov_)>::infinity(), c_};
    } else {
        return std::tuple{cov_ / raw_t(static_cast<int>(c_) - ddof), c_};
    }
}

template <int ddof = 0, class Array1, class Array2, class Predicate>
constexpr scicpp_pure auto
covariance(const Array1 &f1, const Array2 &f2, Predicate filter) {
    return std::get<0>(covariance<ddof>(
        f1.cbegin(), f1.cend(), f2.cbegin(), f2.cend(), filter));
}

template <int ddof = 0, class Array1, class Array2>
constexpr auto covariance(const Array1 &f1, const Array2 &f2) {
    return covariance<ddof>(f1, f2, filters::all);
}

template <int ddof = 0, class Array1, class Array2>
scicpp_pure auto nancovariance(const Array1 &f1, const Array2 &f2) {
    return covariance<ddof>(f1, f2, filters::not_nan);
}

//---------------------------------------------------------------------------------
// var
//---------------------------------------------------------------------------------

template <int ddof = 0, class InputIt, class Predicate>
constexpr auto var(InputIt first, InputIt last, Predicate filter) {
    const auto [v, n] = covariance<ddof>(first, last, first, last, filter);
    using T = std::decay_t<decltype(v)>;

    if constexpr (meta::is_complex_v<T>) {
        // The variance is always a nonnegative real number
        return std::tuple{std::real(v), n};
    } else {
        return std::tuple{v, n};
    }
}

template <int ddof = 0, class Array, class Predicate>
constexpr scicpp_pure auto var(const Array &f, Predicate filter) {
    return std::get<0>(var<ddof>(f.cbegin(), f.cend(), filter));
}

template <int ddof = 0, class Array>
constexpr scicpp_pure auto var(const Array &f) {
    return var<ddof>(f, filters::all);
}

template <int ddof = 0, class Array>
auto nanvar(const Array &f) {
    return var<ddof>(f, filters::not_nan);
}

template <int ddof = 1, class Array, typename T = Array::value_type>
constexpr auto tvar(const Array &f,
                    const std::array<T, 2> &limits,
                    const std::array<bool, 2> &inclusive = {true, true}) {
    return var<ddof>(f, filters::Trim<T>(limits, inclusive));
}

//---------------------------------------------------------------------------------
// std
//---------------------------------------------------------------------------------

template <int ddof = 0, class Array, class Predicate>
scicpp_pure auto std(const Array &a, Predicate filter) {
    return units::sqrt(var<ddof>(a, filter));
}

template <int ddof = 0, class Array>
scicpp_pure auto std(const Array &a) {
    return units::sqrt(var<ddof>(a));
}

template <int ddof = 0, class Array>
auto nanstd(const Array &a) {
    return units::sqrt(nanvar<ddof>(a));
}

template <int ddof = 1, class Array, typename T = Array::value_type>
auto tstd(const Array &a,
          const std::array<T, 2> &limits,
          const std::array<bool, 2> &inclusive = {true, true}) {
    return units::sqrt(tvar<ddof>(a, limits, inclusive));
}

//---------------------------------------------------------------------------------
// sem
//---------------------------------------------------------------------------------

template <int ddof = 1, class Array, class Predicate>
auto sem(const Array &a, Predicate filter) {
    const auto [v, n] = var<ddof>(a.cbegin(), a.cend(), filter);
    using T = std::decay_t<decltype(v)>;
    using raw_t = units::representation_t<T>;
    return units::sqrt(v / raw_t(n));
}

template <int ddof = 1, class Array>
auto sem(const Array &a) {
    const auto v = var<ddof>(a);
    using T = std::decay_t<decltype(v)>;
    using raw_t = units::representation_t<T>;
    return units::sqrt(v / raw_t(a.size()));
}

template <int ddof = 1, class Array>
auto nansem(const Array &a) {
    return sem<ddof>(a, filters::not_nan);
}

template <int ddof = 1, class Array, typename T = Array::value_type>
constexpr auto tsem(const Array &f,
                    const std::array<T, 2> &limits,
                    const std::array<bool, 2> &inclusive = {true, true}) {
    return sem<ddof>(f, filters::Trim<T>(limits, inclusive));
}

//---------------------------------------------------------------------------------
// moment
//---------------------------------------------------------------------------------

template <intmax_t n, class Array, class Predicate>
scicpp_pure auto moment(const Array &f, [[maybe_unused]] Predicate filter) {
    using namespace operators;
    using T = Array::value_type;

    if constexpr (n == 0) {
        return T{1};
    } else if constexpr (n == 1) {
        return T{0};
    } else if constexpr (n == 2) {
        return var(f, filter);
    } else {
        // This allocates an extra array,
        // but preserves pairwise recursion precision
        return mean(pow<n>(f - mean(f, filter)), filter);
        // Combination of moments
        // http://prod.sandia.gov/techlib/access-control.cgi/2008/086212.pdf
    }
}

template <intmax_t n, class Array>
scicpp_pure auto moment(const Array &f) {
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
scicpp_pure auto kurtosis(const Array &f) {
    return kurtosis<def>(f, filters::all);
}

template <KurtosisDef def = KurtosisDef::Fisher, class Array>
scicpp_pure auto nankurtosis(const Array &f) {
    return kurtosis<def>(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// skew
//---------------------------------------------------------------------------------

template <class Array, class Predicate>
scicpp_pure auto skew(const Array &f, Predicate filter) {
    const auto m2 = moment<2>(f, filter);
    const auto m3 = moment<3>(f, filter);
    return m3 / units::sqrt(m2 * m2 * m2);
}

template <class Array>
scicpp_pure auto skew(const Array &f) {
    return skew(f, filters::all);
}

template <class Array>
auto nanskew(const Array &f) {
    return skew(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// covariance matrix
//---------------------------------------------------------------------------------

template <int ddof = 1, class Array1, class Array2, class Predicate>
scicpp_pure auto cov(const Array1 &f1, const Array2 &f2, Predicate filter) {
    const auto covar = covariance<ddof>(f1, f2, filter);
    using T = std::decay_t<decltype(covar)>;

    Eigen::Matrix<T, 2, 2> res;
    res(0, 0) = T(var<ddof>(f1, filter));
    res(0, 1) = covar;
    res(1, 0) = conj(covar);
    res(1, 1) = T(var<ddof>(f2, filter));
    return res;
}

template <int ddof = 1, class Array1, class Array2>
scicpp_pure auto cov(const Array1 &f1, const Array2 &f2) {
    return cov<ddof>(f1, f2, filters::all);
}

template <int ddof = 1, class Array1, class Array2>
auto nancov(const Array1 &f1, const Array2 &f2) {
    return cov<ddof>(f1, f2, filters::not_nan);
}

} // namespace scicpp::stats

#endif // SCICPP_CORE_STATS
