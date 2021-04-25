// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "tuple.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("get field by index") {
    SECTION("std::array tuple") {
        std::array<std::tuple<int, bool, double>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2, 3.14 * double(i * i)};
        }

        // print(get_field<2>(a));
        REQUIRE(almost_equal(
            get_field<0>(std::array<std::tuple<int, bool, double>, 0>{}), {}));
        REQUIRE(almost_equal(get_field<0>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(get_field<1>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_field<2>(a),
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

    SECTION("std::array pair") {
        std::array<std::pair<int, bool>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2};
        }

        // print(get_field<2>(a));
        REQUIRE(almost_equal(
            get_field<0>(std::array<std::pair<int, bool>, 0>{}), {}));
        REQUIRE(almost_equal(get_field<0>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(get_field<1>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
    }

    SECTION("std::vector") {
        std::vector<std::tuple<int, bool, double>> a(10);

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2, 3.14 * double(i * i)};
        }

        // print(get_field<2>(a));
        REQUIRE(almost_equal(
            get_field<0>(std::vector<std::tuple<int, bool, double>>(0)), {}));
        REQUIRE(almost_equal(get_field<0>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(get_field<1>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_field<2>(a),
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

TEST_CASE("get field by type") {
    SECTION("std::array") {
        std::array<std::tuple<int, bool, double>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2, 3.14 * double(i * i)};
        }

        REQUIRE(almost_equal(
            get_field<int>(std::array<std::tuple<int, bool, double>, 0>{}),
            {}));
        REQUIRE(
            almost_equal(get_field<int>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(
            almost_equal(get_field<bool>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_field<double>(a),
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

    SECTION("std::array pair") {
        std::array<std::pair<int, bool>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2};
        }

        REQUIRE(almost_equal(
            get_field<int>(std::array<std::pair<int, bool>, 0>{}), {}));
        REQUIRE(
            almost_equal(get_field<int>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(
            almost_equal(get_field<bool>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
    }

    SECTION("std::vector") {
        std::vector<std::tuple<int, bool, double>> a(10);

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2, 3.14 * double(i * i)};
        }

        REQUIRE(almost_equal(
            get_field<int>(std::vector<std::tuple<int, bool, double>>(0)), {}));
        REQUIRE(
            almost_equal(get_field<int>(a), {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(
            almost_equal(get_field<bool>(a), {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(get_field<double>(a),
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

TEST_CASE("unpack") {
    SECTION("std::array") {
        std::array<std::tuple<int, bool, double>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2, 3.14 * double(i * i)};
        }

        const auto [x, y, z] = unpack(a);

        REQUIRE(almost_equal(x, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(y, {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(z,
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

    SECTION("std::array pair") {
        std::array<std::tuple<int, bool>, 10> a{};

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2};
        }

        const auto [x, y] = unpack(a);

        REQUIRE(almost_equal(x, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(y, {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
    }

    SECTION("std::vector") {
        std::vector<std::tuple<int, bool, double>> a(10);

        for (size_t i = 0; i < a.size(); ++i) {
            a[i] = {int(i), i % 2, 3.14 * double(i * i)};
        }

        const auto [x, y, z] = unpack(a);

        REQUIRE(almost_equal(x, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(almost_equal(y, {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}));
        REQUIRE(almost_equal(z,
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

} // namespace scicpp