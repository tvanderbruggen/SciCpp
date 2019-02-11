// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_NUMERIC
#define SCICPP_CORE_NUMERIC

#include "scicpp/core/functional.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/utils.hpp"

#include <algorithm>
#include <cmath>
#include <complex>
#include <iterator>
#include <limits>
#include <numeric>
#include <type_traits>

namespace scicpp {

//---------------------------------------------------------------------------------
// sum
//---------------------------------------------------------------------------------

template <class InputIt, class Predicate>
constexpr auto sum(InputIt first, InputIt last, Predicate filter) {
    using T = typename std::iterator_traits<InputIt>::value_type;
    return filter_reduce_associative(first, last, std::plus<>(), T{0}, filter);
}

template <class InputIt>
constexpr auto sum(InputIt first, InputIt last) {
    return std::get<0>(sum(first, last, filters::all));
}

template <class Array, class Predicate>
constexpr auto sum(const Array &f, Predicate filter) {
    return sum(f.cbegin(), f.cend(), filter);
}

template <class Array>
constexpr auto sum(const Array &f) {
    return std::get<0>(sum(f, filters::all));
}

template <class Array>
auto nansum(const Array &f) {
    return sum(f, filters::not_nan);
}

//---------------------------------------------------------------------------------
// prod
//---------------------------------------------------------------------------------

template <class InputIt,
          class Predicate,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr auto prod(InputIt first, InputIt last, Predicate filter) {
    return filter_reduce_associative(first, last, std::multiplies<>(), T{1}, filter);
}

template <class InputIt>
constexpr auto prod(InputIt first, InputIt last) {
    return std::get<0>(prod(first, last, filters::all));
}

template <class Array>
constexpr auto prod(const Array &f) {
    return prod(f.cbegin(), f.cend());
}

template <class Array>
auto nanprod(const Array &f) {
    return prod(f.cbegin(), f.cend(), filters::not_nan);
}

//---------------------------------------------------------------------------------
// cumsum
//---------------------------------------------------------------------------------

template <class Array>
auto cumsum(Array &&a) {
    std::partial_sum(a.cbegin(), a.cend(), a.begin());
    return std::move(a);
}

template <class Array>
auto cumsum(const Array &a) {
    return cumsum(Array(a));
}

template <typename T>
auto nancumsum(const std::vector<T> &v) {
    return cumacc(v, std::plus<>(), filters::not_nan);
}

//---------------------------------------------------------------------------------
// cumprod
//---------------------------------------------------------------------------------

template <class Array>
auto cumprod(Array &&a) {
    std::partial_sum(a.cbegin(), a.cend(), a.begin(), std::multiplies<>());
    return std::move(a);
}

template <class Array>
auto cumprod(const Array &a) {
    return cumprod(Array(a));
}

template <typename T>
auto nancumprod(const std::vector<T> &v) {
    return cumacc(v, std::multiplies<>(), filters::not_nan);
}

//---------------------------------------------------------------------------------
// trapz
//---------------------------------------------------------------------------------

template <class InputIt,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr T trapz(InputIt first, InputIt last, T dx) {
    if (std::distance(first, last) == 0) {
        return T{0};
    }

    // https://en.wikipedia.org/wiki/Trapezoidal_rule
    return T{0.5} * dx *
           (*first + T{2} * sum(first + 1, last - 1) + *(last - 1));
}

template <class Array, typename T = typename Array::value_type>
constexpr T trapz(const Array &f, T dx) {
    return trapz(f.cbegin(), f.cend(), dx);
}

//---------------------------------------------------------------------------------
// diff
//---------------------------------------------------------------------------------

namespace detail {

template <typename T, std::size_t N>
auto diff_once(const std::array<T, N> &a) {
    if constexpr (N <= 1) {
        return std::array<T, 0>{};
    } else {
        std::array<T, N - 1> res{};
        std::adjacent_difference(a.cbegin() + 1, a.cend(), res.begin());
        res[0] = a[0];
        return res;
    }
}

template <typename T>
void diff_once(std::vector<T> &res) {
    scicpp_require(!res.empty());

    if (res.size() > 1) {
        const auto tmp = res[0];
        std::adjacent_difference(res.begin() + 1, res.end(), res.begin());
        res[0] = tmp;
    }

    // https://github.com/monero-project/monero/pull/4476/files
    res.pop_back();
}

} // namespace detail

template <int n, typename T, std::size_t N>
auto diff(const std::array<T, N> &a) {
    static_assert(n >= 0);

    if constexpr (n == 0) {
        return std::array{a};
    } else {
        return diff<n - 1>(detail::diff_once(a));
    }
}

template <typename T, std::size_t N>
auto diff(const std::array<T, N> &a) {
    return diff<1>(a);
}

template <typename T>
auto diff(std::vector<T> &&a, int n = 1) {
    scicpp_require(n >= 0);

    while (n-- && !a.empty()) {
        detail::diff_once(a);
    }

    return std::move(a);
}

template <typename T>
auto diff(const std::vector<T> &a, int n = 1) {
    return diff(std::vector<T>(a), n);
}

//---------------------------------------------------------------------------------
// Arithmetic operators
//
// Implements element wise arithmetic operations for std::array and std::vector.
//---------------------------------------------------------------------------------

namespace operators {

namespace detail {

// We define the operator for iterable types which are not
// Eigen::Matrix or Eigen::Array.

template <class T>
constexpr bool is_operator_iterable_v =
    meta::is_iterable_v<T> && !meta::is_eigen_container_v<T>;

template <class T>
using enable_if_operator_iterable =
    std::enable_if_t<detail::is_operator_iterable_v<T>, int>;

template <class T>
using enable_if_scalar =
    std::enable_if_t<std::is_arithmetic_v<T> || meta::is_complex_v<T>, int>;

} // namespace detail

// negate

template <class Array, detail::enable_if_operator_iterable<Array> = 0>
auto operator-(Array &&a) {
    return map(std::negate<>(), std::forward<Array>(a));
}

// scalar multiply
template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator*(Array &&a, T scalar) {
    return map([=](auto v) { return scalar * v; }, std::forward<Array>(a));
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator*(T scalar, Array &&a) {
    return map([=](auto v) { return scalar * v; }, std::forward<Array>(a));
}

// scalar add

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator+(Array &&a, T scalar) {
    return map([=](auto v) { return scalar + v; }, std::forward<Array>(a));
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator+(T scalar, Array &&a) {
    return map([=](auto v) { return scalar + v; }, std::forward<Array>(a));
}

// scalar substract

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator-(Array &&a, T scalar) {
    return map([=](auto v) { return v - scalar; }, std::forward<Array>(a));
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator-(T scalar, Array &&a) {
    return map([=](auto v) { return scalar - v; }, std::forward<Array>(a));
}

// scalar divide

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator/(Array &&a, T scalar) {
    return map([=](auto v) { return v / scalar; }, std::forward<Array>(a));
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator/(T scalar, Array &&a) {
    return map([=](auto v) { return scalar / v; }, std::forward<Array>(a));
}

// scalar modulus

namespace detail {

template <typename T>
auto modulus(T x, T y) {
    if constexpr (std::is_floating_point_v<T>) {
        return std::fmod(x, y);
    } else {
        return x % y;
    }
}

} // namespace detail

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator%(Array &&a, T scalar) {
    return map([=](auto v) { return detail::modulus(v, scalar); },
               std::forward<Array>(a));
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_operator_iterable<Array> = 0,
          detail::enable_if_scalar<T> = 0>
auto operator%(T scalar, Array &&a) {
    return map([=](auto v) { return detail::modulus(scalar, v); },
               std::forward<Array>(a));
}

template <class ArrayLhs,
          class ArrayRhs,
          detail::enable_if_operator_iterable<ArrayLhs> = 0,
          detail::enable_if_operator_iterable<ArrayRhs> = 0>
auto operator*(ArrayLhs &&a, ArrayRhs &&b) {
    return map(std::multiplies<>(),
               std::forward<ArrayLhs>(a),
               std::forward<ArrayRhs>(b));
}

// Sum of 3 vectors:
// https://godbolt.org/z/zsZB29
// No copy, similar assembly code than raw loop:
// https://godbolt.org/z/ptIXJ4
// Except 4 calls to new with raw copy, only one with operator+.

template <class ArrayLhs,
          class ArrayRhs,
          detail::enable_if_operator_iterable<ArrayLhs> = 0,
          detail::enable_if_operator_iterable<ArrayRhs> = 0>
auto operator+(ArrayLhs &&a, ArrayRhs &&b) {
    return map(
        std::plus<>(), std::forward<ArrayLhs>(a), std::forward<ArrayRhs>(b));
}

template <class ArrayLhs,
          class ArrayRhs,
          detail::enable_if_operator_iterable<ArrayLhs> = 0,
          detail::enable_if_operator_iterable<ArrayRhs> = 0>
auto operator-(ArrayLhs &&a, ArrayRhs &&b) {
    return map(
        std::minus<>(), std::forward<ArrayLhs>(a), std::forward<ArrayRhs>(b));
}

template <class ArrayLhs,
          class ArrayRhs,
          detail::enable_if_operator_iterable<ArrayLhs> = 0,
          detail::enable_if_operator_iterable<ArrayRhs> = 0>
auto operator/(ArrayLhs &&a, ArrayRhs &&b) {
    return map(
        std::divides<>(), std::forward<ArrayLhs>(a), std::forward<ArrayRhs>(b));
}

template <class ArrayLhs,
          class ArrayRhs,
          detail::enable_if_operator_iterable<ArrayLhs> = 0,
          detail::enable_if_operator_iterable<ArrayRhs> = 00>
auto operator%(ArrayLhs &&a, ArrayRhs &&b) {
    return map([](auto u, auto v) { return detail::modulus(u, v); },
               std::forward<ArrayLhs>(a),
               std::forward<ArrayRhs>(b));
}

} // namespace operators

//---------------------------------------------------------------------------------
// almost_equal
//---------------------------------------------------------------------------------

namespace detail {

template <typename T>
bool is_zero(T a) {
    return std::fpclassify(a) == FP_ZERO;
}

template <int rel_tol, typename T>
bool fp_equal_predicate(T a, T b) {
    static_assert(rel_tol >= 0);

    constexpr T tol = rel_tol * std::numeric_limits<T>::epsilon() / 2;

    if (std::isnan(a) && std::isnan(b)) {
        return true;
    }

    if (std::isinf(a) && std::isinf(b)) {
        return std::signbit(a) == std::signbit(b);
    }

    if (is_zero(a) || is_zero(b)) {
        return std::fabs(a - b) <= tol;
    }

    return std::fabs(a - b) <= tol * std::max(std::fabs(a), std::fabs(b));
}

} // namespace detail

template <int rel_tol = 1,
          typename T,
          std::enable_if_t<!meta::is_iterable_v<T>, int> = 0>
bool almost_equal(T a, T b) {
    if constexpr (meta::is_complex_v<T>) {
        return (detail::fp_equal_predicate<rel_tol>(a.real(), b.real())) &&
               (detail::fp_equal_predicate<rel_tol>(a.imag(), b.imag()));
    } else {
        return detail::fp_equal_predicate<rel_tol>(a, b);
    }
}

template <int rel_tol = 1,
          class Array,
          std::enable_if_t<meta::is_iterable_v<Array>, int> = 0>
bool scicpp_pure almost_equal(const Array &f1, const Array &f2) {
    return std::equal(
        f1.cbegin(), f1.cend(), f2.cbegin(), f2.cend(), [](auto a, auto b) {
            return almost_equal<rel_tol>(a, b);
        });
}

} // namespace scicpp

#endif // SCICPP_CORE_NUMERIC
