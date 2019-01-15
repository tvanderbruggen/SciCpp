// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "stats.hpp"

#include "scicpp/core/numeric.hpp"

#include <cmath>

namespace scicpp {
namespace stats {

namespace stats_constexpr_tests {

constexpr std::array a{1., 2., 3.};
static_assert(float_equal(mean(a), 2.));
static_assert(float_equal(var(a), 2. / 3.));
static_assert(float_equal(amax(a), 3.));
static_assert(float_equal(amin(a), 1.));
static_assert(float_equal(ptp(a), 2.));

} // namespace stats_constexpr_tests

TEST_CASE("amax") {
    REQUIRE(std::isnan(amax(std::array<double, 0>{})));
    REQUIRE(almost_equal(amax(std::array{1., 4., 5., 6., 2., 3.}), 6.));
    REQUIRE(almost_equal(amax(std::vector{1., 4., 5., 6., 2., 3.}), 6.));
}

TEST_CASE("amin") {
    REQUIRE(std::isnan(amin(std::array<double, 0>{})));
    REQUIRE(almost_equal(amin(std::array{1., 4., 5., 6., 2., 3.}), 1.));
    REQUIRE(almost_equal(amin(std::vector{1., 4., 5., 6., 2., 3.}), 1.));
}

TEST_CASE("ptp") {
    REQUIRE(std::isnan(ptp(std::array<double, 0>{})));
    REQUIRE(almost_equal(ptp(std::array{1., 4., 5., 6., 2., 3.}), 5.));
    REQUIRE(almost_equal(ptp(std::vector{1., 4., 5., 6., 2., 3.}), 5.));
}

TEST_CASE("average") {
    REQUIRE(
        std::isnan(average(std::array<double, 0>{}, std::array<double, 0>{})));
    REQUIRE(std::isnan(average(std::vector{1., 2.}, std::vector{3., 4., 5.})));
    REQUIRE(
        almost_equal<2>(average(std::array{1., 2., 3.}, std::array{4., 5., 6.}),
                        2.133333333333333));
    REQUIRE(almost_equal<2>(
        average(std::vector{1., 2., 3.}, std::vector{4., 5., 6.}),
        2.133333333333333));
}

TEST_CASE("mean") {
    REQUIRE(std::isnan(mean(std::array<double, 0>{})));
    REQUIRE(almost_equal(mean(std::array{1.0, 2.0, 3.0}), 2.0));
    REQUIRE(almost_equal(mean(std::vector{1.0, 2.0, 3.0}), 2.0));
}

TEST_CASE("var") {
    REQUIRE(std::isnan(var(std::array<double, 0>{})));
    REQUIRE(almost_equal(var(std::array{1.0, 2.0, 3.0}), 2.0 / 3.0));
    REQUIRE(almost_equal(var(std::vector{1.0, 2.0, 3.0}), 2.0 / 3.0));
}

TEST_CASE("std") {
    REQUIRE(almost_equal(std(std::array{1.0, 2.0, 3.0}), 0.816496580927726));
    REQUIRE(almost_equal(std(std::vector{4.0, 1.0, 12.0}), 4.642796092394707));
}

} // namespace stats
} // namespace scicpp
