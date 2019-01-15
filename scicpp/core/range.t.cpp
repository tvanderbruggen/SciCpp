// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "range.hpp"

namespace scicpp {

namespace numerics_static_tests {
// linspace
constexpr auto empty = linspace<0>(2.0, 3.0);
static_assert(empty.size() == 0);

constexpr auto single = linspace<1>(2.0, 3.0);
static_assert(single.size() == 1);
static_assert(float_equal(single[0], 2.0));

constexpr auto a = linspace<5>(2.0, 3.0);
static_assert(a.size() == 5);
static_assert(float_equal(a[0], 2.0));
static_assert(float_equal(a[1], 2.25));
} // namespace numerics_static_tests

TEST_CASE("linspace std::array") {
    auto empty = linspace<0>(2.0, 3.0);
    REQUIRE(empty.size() == 0);

    auto single = linspace<1>(2.0, 3.0);
    REQUIRE(single.size() == 1);
    REQUIRE_FLOAT_EQUAL(single[0], 2.0);

    auto a = linspace<5>(2.0, 3.0);
    REQUIRE(a.size() == 5);

    for (std::size_t i = 0; i < a.size(); i++) {
        REQUIRE_FLOAT_EQUAL(a[i], 2.0 + 0.25 * double(i));
    }

    auto reverse = linspace<5>(3.0, 2.0);
    REQUIRE(reverse.size() == 5);

    for (std::size_t i = 0; i < reverse.size(); i++) {
        REQUIRE_FLOAT_EQUAL(reverse[i], 3.0 - 0.25 * double(i));
    }
}

TEST_CASE("linspace std::vector") {
    auto empty = linspace(2.0, 3.0, 0);
    REQUIRE(empty.size() == 0);

    auto single = linspace(2.0, 3.0, 1);
    REQUIRE(single.size() == 1);
    REQUIRE_FLOAT_EQUAL(single[0], 2.0);

    auto v = linspace(2.0, 3.0, 5);
    REQUIRE(v.size() == 5);

    for (std::size_t i = 0; i < v.size(); i++) {
        REQUIRE_FLOAT_EQUAL(v[i], 2.0 + 0.25 * double(i));
    }

    auto reverse = linspace(3.0, 2.0, 5);
    REQUIRE(reverse.size() == 5);

    for (std::size_t i = 0; i < reverse.size(); i++) {
        REQUIRE_FLOAT_EQUAL(reverse[i], 3.0 - 0.25 * double(i));
    }
}

TEST_CASE("logspace std::array") {
    auto empty = logspace<0>(2.0, 3.0);
    REQUIRE(empty.size() == 0);

    auto single = logspace<1>(2.0, 3.0);
    REQUIRE(single.size() == 1);
    REQUIRE_FLOAT_EQUAL(single[0], 100.0);

    auto a = logspace<4>(2.0, 3.0);
    REQUIRE(a.size() == 4);
    REQUIRE_FLOAT_EQUAL(a[0], 100.0);
    REQUIRE_FLOAT_EQUAL(a[1], 215.44346900318845);
    REQUIRE_FLOAT_EQUAL(a[2], 464.15888336127773);
    REQUIRE_FLOAT_EQUAL(a[3], 1000.0);
}

TEST_CASE("logspace std::vector") {
    auto empty = logspace(2.0, 3.0, 0);
    REQUIRE(empty.size() == 0);

    auto single = logspace(2.0, 3.0, 1);
    REQUIRE(single.size() == 1);
    REQUIRE_FLOAT_EQUAL(single[0], 100.0);

    auto a = logspace(2.0, 3.0, 4);
    REQUIRE(a.size() == 4);
    REQUIRE_FLOAT_EQUAL(a[0], 100.0);
    REQUIRE_FLOAT_EQUAL(a[1], 215.44346900318845);
    REQUIRE_FLOAT_EQUAL(a[2], 464.15888336127773);
    REQUIRE_FLOAT_EQUAL(a[3], 1000.0);
}

TEST_CASE("arange std::vector") {
    auto empty = arange(2.0, 3.0, 10.0);
    REQUIRE(empty.size() == 0);

    auto single = arange(2.0, 3.0, 0.75);
    REQUIRE(single.size() == 1);
    REQUIRE_FLOAT_EQUAL(single[0], 2.0);

    auto a = arange(0.0, 3.0);
    REQUIRE(a.size() == 3);
    REQUIRE_FLOAT_EQUAL(a[0], 0.0);
    REQUIRE_FLOAT_EQUAL(a[1], 1.0);
    REQUIRE_FLOAT_EQUAL(a[2], 2.0);

    auto r1 = arange(1.0, 0.0, 0.5);
    REQUIRE(r1.size() == 0);

    auto r2 = arange(0.0, 3.0, -1.0);
    REQUIRE(r2.size() == 0);

    auto r3 = arange(3.0, 0.0, -1.0);
    REQUIRE(r3.size() == 3);
    REQUIRE_FLOAT_EQUAL(r3[0], 3.0);
    REQUIRE_FLOAT_EQUAL(r3[1], 2.0);
    REQUIRE_FLOAT_EQUAL(r3[2], 1.0);
}

} // namespace scicpp
