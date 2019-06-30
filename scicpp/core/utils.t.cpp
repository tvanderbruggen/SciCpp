// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "utils.hpp"

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp::utils {

TEST_CASE("set_array") {
    const std::vector v{1, 2, 3};
    const std::array a{1, 2, 3};

    REQUIRE(set_array(v) == std::vector<int>(v.size()));
    REQUIRE(set_array(a) == std::array<int, a.size()>{});
    REQUIRE(set_array<float>(a) == std::array<float, a.size()>{});
    REQUIRE(set_array<float>(v) == std::vector<float>(v.size()));
}

TEST_CASE("move_subvector") {
    REQUIRE(move_subvector(std::vector{1, 2, 3}, 2) == std::vector{1, 2});
    REQUIRE(move_subvector(std::vector{1, 2, 3}, 0) == std::vector<int>{});
    REQUIRE(move_subvector(std::vector{1, 2, 3}, 4) == std::vector{1, 2, 3});
}

TEST_CASE("get elements by index from array of tuples") {
    SECTION("std::array") {
        std::array<std::tuple<int, bool, double>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = std::make_tuple(int(i), i % 2, 3.14 * double(i * i));
        }

        // print(get_elements<2>(a));
        REQUIRE(almost_equal(
            get_elements<0>(std::array<std::tuple<int, bool, double>, 0>{}),
            {}));
        REQUIRE(
            almost_equal(get_elements<0>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(
            almost_equal(get_elements<1>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_elements<2>(a),
                             {0.,
                              3.14,
                              12.56,
                              28.26,
                              50.24,
                              78.5,
                              113.04,
                              153.86,
                              200.96,
                              254.34}));
    }

    SECTION("std::vector") {
        std::vector<std::tuple<int, bool, double>> a(10);

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = std::make_tuple(int(i), i % 2, 3.14 * double(i * i));
        }

        // print(get_elements<2>(a));
        REQUIRE(almost_equal(
            get_elements<0>(std::vector<std::tuple<int, bool, double>>(0)),
            {}));
        REQUIRE(
            almost_equal(get_elements<0>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(
            almost_equal(get_elements<1>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_elements<2>(a),
                             {0.,
                              3.14,
                              12.56,
                              28.26,
                              50.24,
                              78.5,
                              113.04,
                              153.86,
                              200.96,
                              254.34}));
    }
}

TEST_CASE("get elements by type from array of tuples") {
    SECTION("std::array") {
        std::array<std::tuple<int, bool, double>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = std::make_tuple(int(i), i % 2, 3.14 * double(i * i));
        }

        // print(get_elements<2>(a));
        REQUIRE(almost_equal(
            get_elements<int>(std::array<std::tuple<int, bool, double>, 0>{}),
            {}));
        REQUIRE(
            almost_equal(get_elements<int>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(get_elements<bool>(a),
                             {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_elements<double>(a),
                             {0.,
                              3.14,
                              12.56,
                              28.26,
                              50.24,
                              78.5,
                              113.04,
                              153.86,
                              200.96,
                              254.34}));
    }

    SECTION("std::vector") {
        std::vector<std::tuple<int, bool, double>> a(10);

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = std::make_tuple(int(i), i % 2, 3.14 * double(i * i));
        }

        // print(get_elements<2>(a));
        REQUIRE(almost_equal(
            get_elements<int>(std::vector<std::tuple<int, bool, double>>(0)),
            {}));
        REQUIRE(
            almost_equal(get_elements<int>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(get_elements<bool>(a),
                             {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_elements<double>(a),
                             {0.,
                              3.14,
                              12.56,
                              28.26,
                              50.24,
                              78.5,
                              113.04,
                              153.86,
                              200.96,
                              254.34}));
    }
}

} // namespace scicpp::utils