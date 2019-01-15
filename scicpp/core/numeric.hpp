// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_NUMERIC
#define SCICPP_CORE_NUMERIC

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"

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

template <class InputIt,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr T sum(InputIt first, InputIt last) {
    // We don't use std::accumulate because it is not constexpr
    T res = T(0);

    for (; first != last; ++first) {
        res += *first;
    }

    return res;
}

template <class Array>
constexpr auto sum(const Array &f) {
    return sum(f.cbegin(), f.cend());
}

//---------------------------------------------------------------------------------
// prod
//---------------------------------------------------------------------------------

template <class InputIt,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr T prod(InputIt first, InputIt last) {
    // We don't use std::accumulate because it is not constexpr
    T res = T(1);

    for (; first != last; ++first) {
        res *= *first;
    }

    return res;
}

template <class Array>
constexpr auto prod(const Array &f) {
    return prod(f.cbegin(), f.cend());
}

//---------------------------------------------------------------------------------
// cumsum
//---------------------------------------------------------------------------------

template <typename T, std::size_t N>
auto cumsum(const std::array<T, N> &a) {
    std::array<T, N> res{};
    std::partial_sum(a.cbegin(), a.cend(), res.begin());
    return res;
}

template <typename T>
auto cumsum(const std::vector<T> &a) {
    std::vector<T> res(a.size());
    std::partial_sum(a.cbegin(), a.cend(), res.begin());
    return res;
}

//---------------------------------------------------------------------------------
// trapz
//---------------------------------------------------------------------------------

template <class InputIt,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr T trapz(InputIt first, InputIt last, T dx) {
    if (std::distance(first, last) == 0) {
        return T(0);
    }

    // https://en.wikipedia.org/wiki/Trapezoidal_rule
    return T(0.5) * dx *
           (*first + T(2) * sum(first + 1, last - 1) + *(last - 1));
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
    SCICPP_REQUIRE(!res.empty());

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
void diff_inplace(std::vector<T> &a, int n = 1) {
    SCICPP_REQUIRE(n >= 0);

    while (n-- && !a.empty()) {
        detail::diff_once(a);
    }
}

template <typename T>
auto diff(const std::vector<T> &a, int n = 1) {
    std::vector<T> res(a);
    diff_inplace(res, n);
    return res;
}

//---------------------------------------------------------------------------------
// Arithmetic operators
//
// Implements element wise arithmetic operations for std::array and std::vector.
//---------------------------------------------------------------------------------

namespace detail {

// We define the operator for iterable types which are not
// Eigen::Matrix or Eigen::Array.

template <class T>
constexpr bool has_valid_operator_overload_v =
    meta::is_iterable_v<T> && !meta::is_eigen_container_v<T>;

template <class T>
using enable_if_valid_operator_overload =
    std::enable_if_t<detail::has_valid_operator_overload_v<T>, int>;

} // namespace detail

// negate

template <class Array, detail::enable_if_valid_operator_overload<Array> = 0>
auto operator-(const Array &a) {
    Array res(a);
    std::transform(a.cbegin(), a.cend(), res.begin(), std::negate<>());
    return res;
}

// scalar multiply

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator*=(Array &a, T scalar) {
    std::transform(
        a.cbegin(), a.cend(), a.begin(), [=](auto v) { return scalar * v; });
    return a;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator*(const Array &a, T scalar) {
    Array res(a);
    res *= scalar;
    return res;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator*(T scalar, const Array &a) {
    return a * scalar;
}

// scalar add

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator+=(Array &a, T scalar) {
    std::transform(
        a.cbegin(), a.cend(), a.begin(), [=](auto v) { return scalar + v; });
    return a;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator+(const Array &a, T scalar) {
    Array res(a);
    res += scalar;
    return res;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator+(T scalar, const Array &a) {
    return a + scalar;
}

// scalar substract

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator-=(Array &a, T scalar) {
    std::transform(
        a.cbegin(), a.cend(), a.begin(), [=](auto v) { return v - scalar; });
    return a;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator-(const Array &a, T scalar) {
    Array res(a);
    res -= scalar;
    return res;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator-(T scalar, const Array &a) {
    Array res(a);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), [=](auto v) { return scalar - v; });
    return res;
}

// scalar divide

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator/=(Array &a, T scalar) {
    std::transform(
        a.cbegin(), a.cend(), a.begin(), [=](auto v) { return v / scalar; });
    return a;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator/(const Array &a, T scalar) {
    Array res(a);
    res /= scalar;
    return res;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator/(T scalar, const Array &a) {
    Array res(a);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), [=](auto v) { return scalar / v; });
    return res;
}

// scalar modulus

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator%=(Array &a, T scalar) {
    std::transform(a.cbegin(), a.cend(), a.begin(), [=](auto v) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::fmod(v, scalar);
        } else {
            return v % scalar;
        }
    });
    return a;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator%(const Array &a, T scalar) {
    Array res(a);
    res %= scalar;
    return res;
}

template <class Array,
          typename T = typename Array::value_type,
          detail::enable_if_valid_operator_overload<Array> = 0>
auto operator%(T scalar, const Array &a) {
    Array res(a);
    std::transform(a.cbegin(), a.cend(), res.begin(), [=](auto v) {
        if constexpr (std::is_floating_point_v<T>) {
            return std::fmod(scalar, v);
        } else {
            return scalar % v;
        }
    });
    return res;
}

// array multiply

template <class Array, detail::enable_if_valid_operator_overload<Array> = 0>
auto operator*(const Array &a, const Array &b) {
    SCICPP_REQUIRE(a.size() == b.size());

    Array res(b);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), res.begin(), std::multiplies<>());
    return res;
}

// array add

template <class Array, detail::enable_if_valid_operator_overload<Array> = 0>
auto operator+(const Array &a, const Array &b) {
    SCICPP_REQUIRE(a.size() == b.size());

    Array res(b);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), res.begin(), std::plus<>());
    return res;
}

// array substract

template <class Array, detail::enable_if_valid_operator_overload<Array> = 0>
auto operator-(const Array &a, const Array &b) {
    SCICPP_REQUIRE(a.size() == b.size());

    Array res(b);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), res.begin(), std::minus<>());
    return res;
}

// array divide

template <class Array, detail::enable_if_valid_operator_overload<Array> = 0>
auto operator/(const Array &a, const Array &b) {
    SCICPP_REQUIRE(a.size() == b.size());

    Array res(b);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), res.begin(), std::divides<>());
    return res;
}

// array modulus

template <class Array, detail::enable_if_valid_operator_overload<Array> = 0>
auto operator%(const Array &a, const Array &b) {
    using T = typename Array::value_type;
    SCICPP_REQUIRE(a.size() == b.size());

    Array res(b);
    std::transform(
        a.cbegin(), a.cend(), res.begin(), res.begin(), [](auto u, auto v) {
            if constexpr (std::is_floating_point_v<T>) {
                return std::fmod(u, v);
            } else {
                return u % v;
            }
        });
    return res;
}

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
bool almost_equal(const Array &f1, const Array &f2) {
    return std::equal(
        f1.cbegin(), f1.cend(), f2.cbegin(), f2.cend(), [](auto a, auto b) {
            return almost_equal<rel_tol>(a, b);
        });
}

} // namespace scicpp

#endif // SCICPP_CORE_NUMERIC
