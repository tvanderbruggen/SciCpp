// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_META
#define SCICPP_CORE_META

#include <Eigen/Dense>
#include <array>
#include <complex>
#include <ratio>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp::meta {

//---------------------------------------------------------------------------------
// is_complex
//---------------------------------------------------------------------------------

namespace detail {

template <class T>
struct is_complex : std::false_type {};
template <class T>
struct is_complex<std::complex<T>> : std::true_type {};

} // namespace detail

template <class T>
inline constexpr bool is_complex_v = detail::is_complex<T>::value;

static_assert(!is_complex_v<double>);
static_assert(is_complex_v<std::complex<double>>);

//---------------------------------------------------------------------------------
// is_iterable
//---------------------------------------------------------------------------------

// https://stackoverflow.com/questions/13830158/check-if-a-variable-is-iterable

namespace detail {

// To allow ADL with custom begin/end
using std::begin;
using std::end;

template <typename T>
auto is_iterable_impl(int) -> decltype(
    begin(std::declval<T &>()) !=
        end(std::declval<T &>()), // begin/end and operator !=
    void(),                       // Handle evil operator ,
    ++std::declval<decltype(begin(std::declval<T &>())) &>(), // operator ++
    void(*begin(std::declval<T &>())),                        // operator*
    std::true_type{});

template <typename T>
std::false_type is_iterable_impl(...);

template <typename T>
using is_iterable = decltype(detail::is_iterable_impl<T>(0));

} // namespace detail

template <typename T>
inline constexpr bool is_iterable_v = detail::is_iterable<T>::value;

static_assert(is_iterable_v<std::array<double, 3>>);
static_assert(is_iterable_v<std::vector<float>>);
static_assert(!is_iterable_v<double>);

//---------------------------------------------------------------------------------
// std::vector traits
//---------------------------------------------------------------------------------

namespace detail {

template <class T>
struct is_std_vector : std::false_type {};
template <typename Scalar>
struct is_std_vector<std::vector<Scalar>> : std::true_type {};

} // namespace detail

template <class T>
inline constexpr bool is_std_vector_v = detail::is_std_vector<T>::value;

static_assert(!is_std_vector_v<Eigen::Matrix2d>);
static_assert(is_std_vector_v<std::vector<double>>);

//---------------------------------------------------------------------------------
// std::tuple traits
//---------------------------------------------------------------------------------

namespace detail {

template <class T>
struct is_std_tuple : std::false_type {};
template <typename... Args>
struct is_std_tuple<std::tuple<Args...>> : std::true_type {};

} // namespace detail

template <class T>
inline constexpr bool is_std_tuple_v = detail::is_std_tuple<T>::value;

static_assert(!is_std_tuple_v<std::vector<double>>);
static_assert(is_std_tuple_v<std::tuple<double, int, float>>);

//---------------------------------------------------------------------------------
// subtuple
// https://stackoverflow.com/questions/17854219/creating-a-sub-tuple-starting-from-a-stdtuplesome-types
//---------------------------------------------------------------------------------

namespace detail {

template <typename... T, std::size_t... I>
auto subtuple_(const std::tuple<T...> &t, std::index_sequence<I...>) {
    return std::make_tuple(std::get<I>(t)...);
}

} // namespace detail

template <int Trim, typename... T>
auto subtuple(const std::tuple<T...> &t) {
    return detail::subtuple_(t,
                             std::make_index_sequence<sizeof...(T) - Trim>());
}

//---------------------------------------------------------------------------------
// is_ratio
//---------------------------------------------------------------------------------

namespace detail {

template <class T>
struct is_ratio : std::false_type {};

template <intmax_t num, intmax_t den>
struct is_ratio<std::ratio<num, den>> : std::true_type {};

} // namespace detail

template <class T>
inline constexpr bool is_ratio_v = detail::is_ratio<T>::value;

static_assert(is_ratio_v<std::ratio<1, 42>>);
static_assert(!is_ratio_v<std::array<float, 42>>);

//---------------------------------------------------------------------------------
// Eigen type traits
//---------------------------------------------------------------------------------

namespace detail {

template <class T>
struct is_eigen_matrix : std::false_type {};
template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
struct is_eigen_matrix<
    Eigen::Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime>>
    : std::true_type {};

template <class T>
struct is_eigen_array : std::false_type {};
template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
struct is_eigen_array<
    Eigen::Array<Scalar, RowsAtCompileTime, ColsAtCompileTime>>
    : std::true_type {};

} // namespace detail

template <class T>
inline constexpr bool is_eigen_matrix_v = detail::is_eigen_matrix<T>::value;

static_assert(is_eigen_matrix_v<Eigen::Matrix2d>);
static_assert(!is_eigen_matrix_v<std::vector<double>>);
static_assert(!is_eigen_matrix_v<std::array<double, 4>>);

template <class T>
inline constexpr bool is_eigen_array_v = detail::is_eigen_array<T>::value;

static_assert(is_eigen_array_v<Eigen::Array2Xf>);
static_assert(!is_eigen_matrix_v<Eigen::Array2Xf>);
static_assert(!is_eigen_array_v<std::vector<double>>);
static_assert(!is_eigen_array_v<std::array<double, 4>>);

template <class T>
inline constexpr bool is_eigen_container_v =
    is_eigen_matrix_v<T> || is_eigen_array_v<T>;

static_assert(is_eigen_container_v<Eigen::Array2Xf>);
static_assert(is_eigen_container_v<Eigen::Matrix2d>);
static_assert(!is_eigen_container_v<std::vector<double>>);
static_assert(!is_eigen_container_v<std::array<double, 4>>);

} // namespace scicpp::meta

#endif // SCICPP_CORE_META