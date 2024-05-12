// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "meta.hpp"

#include "scicpp/core/equal.hpp"

namespace scicpp::meta {

TEST_CASE("is_complex") {
    static_assert(!is_complex_v<double>);
    static_assert(is_complex_v<std::complex<double>>);
}

TEST_CASE("is_iterable") {
    static_assert(is_iterable_v<std::array<double, 3>>);
    static_assert(is_iterable_v<std::vector<float>>);
    static_assert(!is_iterable_v<double>);
}

TEST_CASE("is_std_vector") {
    static_assert(!is_std_vector_v<Eigen::Matrix2d>);
    static_assert(is_std_vector_v<std::vector<double>>);
}

TEST_CASE("is_std_array") {
    static_assert(!is_std_array_v<Eigen::Matrix2d>);
    static_assert(is_std_array_v<std::array<double, 10>>);
}

TEST_CASE("is_std_tuple") {
    static_assert(!is_std_tuple_v<std::vector<double>>);
    static_assert(is_std_tuple_v<std::tuple<double, int, float>>);
}

TEST_CASE("is_std_pair") {
    static_assert(!is_std_pair_v<std::vector<double>>);
    static_assert(is_std_pair_v<std::pair<double, int>>);
}

TEST_CASE("subtuple") {
    constexpr auto t = std::make_tuple(10, 20.0, "OK");
    constexpr auto st = subtuple<2>(t);
    static_assert(std::tuple_size_v<decltype(st)> == 1);
    static_assert(std::get<0>(st) == 10);

    constexpr auto st1 = subtuple<1>(t);
    static_assert(std::tuple_size_v<decltype(st1)> == 2);
    static_assert(std::get<0>(st1) == 10);
    static_assert(float_equal(std::get<1>(st1), 20.0));

    constexpr auto st2 = subtuple<0>(t);
    static_assert(std::tuple_size_v<decltype(st2)> == 3);
    static_assert(std::get<0>(st2) == 10);
    static_assert(float_equal(std::get<1>(st2), 20.0));
    static_assert(strings_equal(std::get<2>(st2), "OK"));

    constexpr auto p = std::make_pair(10, 20.0);
    constexpr auto sp = subtuple<2>(p);
    static_assert(std::tuple_size_v<decltype(sp)> == 1);
    static_assert(std::get<0>(sp) == 10);
}

TEST_CASE("is_ratio") {
    static_assert(is_ratio_v<std::ratio<1, 42>>);
    static_assert(!is_ratio_v<std::array<float, 42>>);
}

TEST_CASE("is_eigen_matrix/is_eigen_array/is_eigen_container") {
    static_assert(is_eigen_matrix_v<Eigen::Matrix2d>);
    static_assert(!is_eigen_matrix_v<std::vector<double>>);
    static_assert(!is_eigen_matrix_v<std::array<double, 4>>);

    static_assert(is_eigen_array_v<Eigen::Array2Xf>);
    static_assert(!is_eigen_matrix_v<Eigen::Array2Xf>);
    static_assert(!is_eigen_array_v<std::vector<double>>);
    static_assert(!is_eigen_array_v<std::array<double, 4>>);

    static_assert(is_eigen_container_v<Eigen::Array2Xf>);
    static_assert(is_eigen_container_v<Eigen::Matrix2d>);
    static_assert(!is_eigen_container_v<std::vector<double>>);
    static_assert(!is_eigen_container_v<std::array<double, 4>>);
}

TEST_CASE("is_predicate") {
    static_assert(is_predicate<std::greater<>, double, double>);
    static_assert(!is_predicate<std::plus<>, double, double>);
}

TEST_CASE("is_string") {
    static_assert(is_string_v<const char *>);
    static_assert(is_string_v<char *>);
    static_assert(is_string_v<std::string>);
    static_assert(!is_string_v<float *>);
}

TEST_CASE("is_movable") {
    static_assert(is_movable_v<int>);
    static_assert(is_movable_v<std::string>);
    static_assert(is_movable_v<std::vector<double>>);

    // https://en.cppreference.com/w/cpp/types/is_move_constructible
    struct NoMove {
        // Not move-constructible since the lvalue reference
        // can't bind to the rvalue argument
        NoMove(NoMove &) {}
    };

    static_assert(!is_movable_v<NoMove>);
}

} // namespace scicpp::meta