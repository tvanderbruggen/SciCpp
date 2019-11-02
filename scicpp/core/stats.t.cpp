// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "stats.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units/units.hpp"

#include <cmath>

namespace scicpp::stats {

TEST_CASE("amax") {
    static_assert(float_equal(amax(std::array{1., 2., 3.}), 3.));
    REQUIRE(std::isnan(amax(std::array<double, 0>{})));
    REQUIRE(almost_equal(amax(std::array{1., 4., 5., 6., 2., 3.}), 6.));
    REQUIRE(almost_equal(amax(std::vector{1., 4., 5., 6., 2., 3.}), 6.));
}

TEST_CASE("amax physical quantities") {
    using namespace units::literals;
    REQUIRE(units::isnan(amax(std::array<units::length<double>, 0>{})));
    REQUIRE(almost_equal(amax(std::array{1._m, 4._m, 5._m, 6._m, 2._m, 3._m}),
                         6._m));
    REQUIRE(almost_equal(amax(std::vector{1._m, 4._m, 5._m, 6._m, 2._m, 3._m}),
                         6._m));
}

TEST_CASE("amin") {
    static_assert(float_equal(amin(std::array{1., 2., 3.}), 1.));
    REQUIRE(std::isnan(amin(std::array<double, 0>{})));
    REQUIRE(almost_equal(amin(std::array{1., 4., 5., 6., 2., 3.}), 1.));
    REQUIRE(almost_equal(amin(std::vector{1., 4., 5., 6., 2., 3.}), 1.));
}

TEST_CASE("amin physical quantities") {
    using namespace units::literals;
    REQUIRE(units::isnan(amin(std::array<units::length<double>, 0>{})));
    REQUIRE(almost_equal(amin(std::array{1._m, 4._m, 5._m, 6._m, 2._m, 3._m}),
                         1._m));
    REQUIRE(almost_equal(amin(std::vector{1._m, 4._m, 5._m, 6._m, 2._m, 3._m}),
                         1._m));
}

TEST_CASE("ptp") {
    static_assert(float_equal(ptp(std::array{1., 2., 3.}), 2.));
    REQUIRE(std::isnan(ptp(std::array<double, 0>{})));
    REQUIRE(almost_equal(ptp(std::array{1., 4., 5., 6., 2., 3.}), 5.));
    REQUIRE(almost_equal(ptp(std::vector{1., 4., 5., 6., 2., 3.}), 5.));
}

TEST_CASE("ptp physical quantities") {
    using namespace units::literals;
    REQUIRE(units::isnan(ptp(std::array<units::length<double>, 0>{})));
    REQUIRE(almost_equal(ptp(std::array{1._m, 4._m, 5._m, 6._m, 2._m, 3._m}),
                         5._m));
    REQUIRE(almost_equal(ptp(std::vector{1._m, 4._m, 5._m, 6._m, 2._m, 3._m}),
                         5._m));
}

TEST_CASE("average") {
    static_assert(
        float_equal(average(std::array{1., 2., 3.}, std::array{4., 5., 6.}),
                    2.133333333333333));
    REQUIRE(
        std::isnan(average(std::array<double, 0>{}, std::array<double, 0>{})));
    REQUIRE(std::isnan(average(std::vector{1., 2.}, std::vector{3., 4., 5.})));
    REQUIRE(
        almost_equal<2>(average(std::array{1., 2., 3.}, std::array{4., 5., 6.}),
                        2.133333333333333));
    REQUIRE(almost_equal<2>(
        average(std::vector{1., 2., 3.}, std::vector{4., 5., 6.}),
        2.133333333333333));
    // printf("%.20f\n",
    //        average(linspace(0., 87946., 1000000), linspace(0., 156., 1000000)));
    REQUIRE(almost_equal<6>(
        average(linspace(0., 87946., 1000000), linspace(0., 156., 1000000)),
        58630.695982029356));
}

TEST_CASE("average physical quantities") {
    using namespace units::literals;
    REQUIRE(units::isnan(average(std::array<units::length<double>, 0>{},
                                 std::array<units::length<double>, 0>{})));
    REQUIRE(units::isnan(
        average(std::vector{1._kg, 2._kg}, std::vector{3._kg, 4._kg, 5._kg})));
    REQUIRE(almost_equal<2>(
        average(std::array{1._s, 2._s, 3._s}, std::array{4., 5., 6.}),
        2.133333333333333333333_s));
    REQUIRE(almost_equal<2>(
        average(std::array{1._s, 2._s, 3._s}, std::array{4._kg, 5._kg, 6._kg}),
        2.133333333333333333333_s));
    REQUIRE(almost_equal<2>(
        average(std::array{1._s, 2._s, 3._s}, std::vector{4., 5., 6.}),
        2.133333333333333333333_s));
}

TEST_CASE("mean") {
    static_assert(float_equal(mean(std::array{1., 2., 3.}), 2.));
    REQUIRE(std::isnan(mean(std::array<double, 0>{})));
    REQUIRE(almost_equal(mean(std::array{1., 2., 3.}), 2.));
    REQUIRE(almost_equal(mean(std::vector{1., 2., 3.}), 2.));
}

TEST_CASE("mean physical units") {
    using namespace units::literals;
    REQUIRE(units::isnan(mean(std::array<units::mass<double>, 0>{})));
    REQUIRE(almost_equal(mean(std::array{1._m, 2._m, 3._m}), 2._m));
    REQUIRE(almost_equal(mean(std::vector{1._m, 2._m, 3._m}), 2._m));
}

TEST_CASE("nan") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(nanmean(std::array<double, 0>{})));
    REQUIRE(almost_equal(nanmean(std::array{1., nan, 2., 3., nan}), 2.));
    REQUIRE(almost_equal(nanmean(std::vector{1., 2., nan, 3.}), 2.));
}

TEST_CASE("var") {
    static_assert(float_equal(var(std::array{1., 2., 3.}), 2. / 3.));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(var(std::array<double, 0>{})));
    REQUIRE(almost_equal(var(std::array{1., 2., 3.}), 2. / 3.));
    REQUIRE(almost_equal(var(std::vector{1., 2., 3.}), 2. / 3.));
    REQUIRE(almost_equal(nanvar(std::array{1., nan, 2., 3., nan}), 2. / 3.));
    REQUIRE(almost_equal(nanvar(std::vector{1., 2., nan, 3.}), 2. / 3.));
    auto v = std::vector(500000, 1.);
    v[0] = 1E10;
    // printf("%.20f\n", var(v));
    // Compare with result from numpy
    REQUIRE(almost_equal<32>(var(v), 199999599960000.12));
    // printf("%.20f\n", var(linspace(0., 1253., 1000000)));
    REQUIRE(almost_equal<16>(var(linspace(0., 1253., 1000000)),
                             130834.34500176184));
}

TEST_CASE("var physical units") {
    using namespace units::literals;
    constexpr auto nan =
        units::length<double>(std::numeric_limits<double>::quiet_NaN());
    REQUIRE(units::isnan(var(std::array<units::mass<double>, 0>{})));
    REQUIRE(almost_equal(var(std::array{1._m, 2._m, 3._m}), 2._m2 / 3.));
    REQUIRE(almost_equal(var(std::vector{1._m, 2._m, 3._m}), 2._m2 / 3.));
    REQUIRE(almost_equal(nanvar(std::array{1._m, nan, 2._m, 3._m, nan}),
                         2._m2 / 3.));
    REQUIRE(
        almost_equal(nanvar(std::vector{1._m, 2._m, nan, 3._m}), 2._m2 / 3.));
}

TEST_CASE("std") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(std(std::array{1., 2., 3.}), 0.816496580927726));
    REQUIRE(almost_equal(std(std::vector{4., 1., 12.}), 4.642796092394707));
    REQUIRE(
        almost_equal(nanstd(std::array{1., nan, 2., 3.}), 0.816496580927726));
    REQUIRE(almost_equal(nanstd(std::vector{4., nan, 1., nan, 12.}),
                         4.642796092394707));
}

TEST_CASE("std physical units") {
    using namespace units::literals;
    constexpr auto nan =
        units::length<double>(std::numeric_limits<double>::quiet_NaN());
    REQUIRE(
        almost_equal(std(std::array{1._m, 2._m, 3._m}), 0.816496580927726_m));
    REQUIRE(
        almost_equal(std(std::vector{4._m, 1._m, 12._m}), 4.642796092394707_m));
    REQUIRE(almost_equal(nanstd(std::array{1._m, nan, 2._m, 3._m}),
                         0.816496580927726_m));
    REQUIRE(almost_equal(nanstd(std::vector{4._m, nan, 1._m, nan, 12._m}),
                         4.642796092394707_m));
}

} // namespace scicpp::stats
