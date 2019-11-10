// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "stats.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
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
    REQUIRE(almost_equal(amax(std::array{1_m, 4_m, 5_m, 6_m, 2_m, 3_m}), 6_m));
    REQUIRE(almost_equal(amax(std::vector{1_m, 4_m, 5_m, 6_m, 2_m, 3_m}), 6_m));
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
    REQUIRE(almost_equal(amin(std::array{1_m, 4_m, 5_m, 6_m, 2_m, 3_m}), 1_m));
    REQUIRE(almost_equal(amin(std::vector{1_m, 4_m, 5_m, 6_m, 2_m, 3_m}), 1_m));
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
    REQUIRE(almost_equal(ptp(std::array{1_m, 4_m, 5_m, 6_m, 2_m, 3_m}), 5_m));
    REQUIRE(almost_equal(ptp(std::vector{1_m, 4_m, 5_m, 6_m, 2_m, 3_m}), 5_m));
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
        average(std::vector{1_kg, 2_kg}, std::vector{3_kg, 4_kg, 5_kg})));
    REQUIRE(almost_equal<2>(
        average(std::array{1_s, 2_s, 3_s}, std::array{4., 5., 6.}),
        2.133333333333333333333_s));
    REQUIRE(almost_equal<2>(
        average(std::array{1_s, 2_s, 3_s}, std::array{4_kg, 5_kg, 6_kg}),
        2.133333333333333333333_s));
    REQUIRE(almost_equal<2>(
        average(std::array{1_s, 2_s, 3_s}, std::vector{4., 5., 6.}),
        2.133333333333333333333_s));
}

TEST_CASE("median") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(median(std::array<double, 0>{})));
    REQUIRE(almost_equal(median(std::array{1.}), 1.));
    REQUIRE(almost_equal(median(std::array{1., 2., 3.}), 2.));
    REQUIRE(almost_equal(median(std::vector{1., 2., 3.}), 2.));
    REQUIRE(almost_equal(median(std::array{1., 4., 3., 2.}), 2.5));
    REQUIRE(almost_equal(median(std::vector{1., 3., 2., 4.}), 2.5));
    REQUIRE(almost_equal(nanmedian(std::vector{1., 2., nan, 3.}), 2.));
    REQUIRE(almost_equal(nanmedian(std::array{1., nan, 2., 3., 4.}), 2.5));

    const auto v = std::vector{3., 2., 1.};
    REQUIRE(almost_equal(median(v), 2.));

    const auto a = std::array{3., 4., 1., 2.};
    REQUIRE(almost_equal(median(a), 2.5));
}

TEST_CASE("median physical units") {
    using namespace units::literals;
    REQUIRE(units::isnan(median(std::array<units::mass<double>, 0>{})));
    REQUIRE(almost_equal(median(std::array{1_m, 2_m, 3_m}), 2_m));
    REQUIRE(almost_equal(median(std::vector{1_m, 2_m, 3_m}), 2_m));
    REQUIRE(almost_equal(median(std::array{1_m, 2_m, 3_m, 4_m}), 2.5_m));
    REQUIRE(almost_equal(median(std::vector{1_m, 2_m, 3_m, 4_m}), 2.5_m));
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
    REQUIRE(almost_equal(mean(std::array{1_m, 2_m, 3_m}), 2_m));
    REQUIRE(almost_equal(mean(std::vector{1_m, 2_m, 3_m}), 2_m));
}

TEST_CASE("nanmean") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(nanmean(std::array<double, 0>{})));
    REQUIRE(almost_equal(nanmean(std::array{1., nan, 2., 3., nan}), 2.));
    REQUIRE(almost_equal(nanmean(std::vector{1., 2., nan, 3.}), 2.));
}

TEST_CASE("tmean") {
    static_assert(float_equal(tmean(std::array{1., 2., 3., 4.}, {2., 4.}), 3.));
    static_assert(float_equal(tmean(std::array{1., 2., 3., 4.}, {2., 2.}), 2.));
    const auto x = arange(0., 20.);
    REQUIRE(almost_equal(tmean(x, {3., 17.}), 10.));
    REQUIRE(almost_equal(tmean(x, {3., 17.}, {false, true}), 10.5));
    REQUIRE(almost_equal(tmean(x, {3., 17.}, {true, false}), 9.5));
    REQUIRE(std::isnan(tmean(std::array<double, 0>{}, {3., 17.})));
    REQUIRE(std::isnan(
        tmean(std::array{1., 2., 3., 4.}, {2., 2.}, {false, false})));
}

TEST_CASE("tmean physical units") {
    using namespace units::literals;
    using meter = units::meter<int>;
    static_assert(tmean(std::array{meter(2), meter(3)}, {meter(2), meter(2)}) ==
                  meter(2));
    const auto x = arange(0_Hz, 20_Hz);
    REQUIRE(almost_equal(tmean(x, {3_Hz, 17_Hz}), 10_Hz));
    REQUIRE(almost_equal(tmean(x, {3_Hz, 17_Hz}, {false, true}), 10.5_Hz));
    REQUIRE(almost_equal(tmean(x, {3_Hz, 17_Hz}, {true, false}), 9.5_Hz));
    REQUIRE(units::isnan(
        tmean(std::array<units::hertz<double>, 0>{}, {3_Hz, 17_Hz})));
}

TEST_CASE("var") {
    static_assert(float_equal(var(std::array{1., 2., 3.}), 2. / 3.));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(var(std::array<double, 0>{})));
    REQUIRE(almost_equal(var(std::array{1., 2., 3.}), 2. / 3.));
    REQUIRE(almost_equal(var(std::vector{1., 2., 3.}), 2. / 3.));
    auto v = std::vector(500000, 1.);
    v[0] = 1E10;
    // printf("%.20f\n", var(v));
    // Compare with result from numpy
    REQUIRE(almost_equal<32>(var(v), 199999599960000.12));
    // printf("%.20f\n", var(linspace(0., 1253., 1000000)));
    REQUIRE(almost_equal<16>(var(linspace(0., 1253., 1000000)),
                             130834.34500176184));
    // ddof
    REQUIRE(almost_equal(var<1>(std::array{1., 2., 3.}), 1.));
    REQUIRE(std::isinf(var<3>(std::array{1., 2., 3.})));
    REQUIRE(std::isinf(var<4>(std::array{1., 2., 3.})));
    REQUIRE(almost_equal(var<1>(arange(3., 18.)), 20.));
    // nanvar
    REQUIRE(almost_equal(nanvar(std::array{1., nan, 2., 3., nan}), 2. / 3.));
    REQUIRE(almost_equal(nanvar(std::vector{1., 2., nan, 3.}), 2. / 3.));
    // tvar
    const auto x = arange(0., 20.);
    REQUIRE(almost_equal(tvar(x, {3., 17.}), 20.));
    REQUIRE(almost_equal(tvar(x, {3., 17.}, {true, false}), 17.5));
    REQUIRE(almost_equal(tvar(x, {3., 17.}, {false, true}), 17.5));
}

TEST_CASE("var physical units") {
    using namespace units::literals;
    constexpr auto nan =
        units::length<double>(std::numeric_limits<double>::quiet_NaN());
    REQUIRE(units::isnan(var(std::array<units::mass<double>, 0>{})));
    REQUIRE(almost_equal(var(std::array{1_m, 2_m, 3_m}), 2_m2 / 3.));
    REQUIRE(almost_equal(var(std::vector{1_m, 2_m, 3_m}), 2_m2 / 3.));
    // nanvar
    REQUIRE(
        almost_equal(nanvar(std::array{1_m, nan, 2_m, 3_m, nan}), 2_m2 / 3.));
    REQUIRE(almost_equal(nanvar(std::vector{1_m, 2_m, nan, 3_m}), 2_m2 / 3.));
    // ddof
    REQUIRE(almost_equal(var<1>(std::array{1_m, 2_m, 3_m}), 1_m2));
    REQUIRE(units::isinf(var<3>(std::array{1_m, 2_m, 3_m})));
    REQUIRE(units::isinf(var<4>(std::array{1_m, 2_m, 3_m})));
    // tvar
    const auto x = arange(0_m, 20_m);
    REQUIRE(almost_equal(tvar(x, {3_m, 17_m}), 20_m2));
    REQUIRE(almost_equal(tvar(x, {3_m, 17_m}, {true, false}), 17.5_m2));
    REQUIRE(almost_equal(tvar(x, {3_m, 17_m}, {false, true}), 17.5_m2));
}

TEST_CASE("std") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(std(std::array{1., 2., 3.}), 0.816496580927726));
    REQUIRE(almost_equal(std(std::vector{4., 1., 12.}), 4.642796092394707));
    // nanstd
    REQUIRE(
        almost_equal(nanstd(std::array{1., nan, 2., 3.}), 0.816496580927726));
    REQUIRE(almost_equal(nanstd(std::vector{4., nan, 1., nan, 12.}),
                         4.642796092394707));
    // tstd
    const auto x = arange(0., 20.);
    REQUIRE(almost_equal(tstd(x, {3., 17.}), 4.47213595499958));
    REQUIRE(almost_equal(tstd(x, {3., 17.}, {true, false}), 4.183300132670378));
    REQUIRE(almost_equal(tstd(x, {3., 17.}, {false, true}), 4.183300132670378));
}

TEST_CASE("std physical units") {
    using namespace units::literals;
    constexpr auto nan =
        units::length<double>(std::numeric_limits<double>::quiet_NaN());
    REQUIRE(almost_equal(std(std::array{1_m, 2_m, 3_m}), 0.816496580927726_m));
    REQUIRE(
        almost_equal(std(std::vector{4_m, 1_m, 12_m}), 4.642796092394707_m));
    // nanstd
    REQUIRE(almost_equal(nanstd(std::array{1_m, nan, 2_m, 3_m}),
                         0.816496580927726_m));
    REQUIRE(almost_equal(nanstd(std::vector{4_m, nan, 1_m, nan, 12_m}),
                         4.642796092394707_m));
    // tstd
    const auto x = arange(0_m, 20_m);
    REQUIRE(almost_equal(tstd(x, {3_m, 17_m}), 4.47213595499958_m));
    REQUIRE(almost_equal(tstd(x, {3_m, 17_m}, {true, false}), 4.183300132670378_m));
    REQUIRE(almost_equal(tstd(x, {3_m, 17_m}, {false, true}), 4.183300132670378_m));
}

TEST_CASE("detail::power_v") {
    REQUIRE(almost_equal(detail::power_v<0>(std::vector{1., 2., 3.}),
                         {1., 1., 1.}));
    REQUIRE(almost_equal(detail::power_v<3>(std::array{1., 2., 3.}),
                         {1., 8., 27.}));
}

TEST_CASE("moment") {
    REQUIRE(almost_equal(moment<0>(std::vector{1., 2., 3., 4.}), 1.));
    REQUIRE(almost_equal(moment<1>(std::array{1., 2., 3., 4.}), 0.));
    REQUIRE(almost_equal(moment<2>(std::vector{1., 2., 3., 4.}), 1.25));
    REQUIRE(almost_equal(moment<4>(std::array{1., 2., 3., 4.}), 2.5625));
    REQUIRE(almost_equal(moment<5>(std::vector{1., 2., 3., 4.}), 0.));

    auto v = std::vector(500000, 1.);
    v[0] = 1E10;
    // Compare with result from scipy
    REQUIRE(almost_equal(moment<4>(v), 1.9999839992480064e+34));
    REQUIRE(almost_equal(moment<15>(v), 1.9999399978400832e+144));

    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(
        nanmoment<4>(std::array{1., nan, 2., 3., nan, 4., nan}), 2.5625));
    REQUIRE(
        almost_equal(nanmoment<5>(std::vector{1., 2., nan, 3., 4., nan}), 0.));
}

TEST_CASE("moment physical units") {
    using namespace units::literals;
    REQUIRE(almost_equal(moment<2>(std::vector{1_m, 2_m, 3_m, 4_m}), 1.25_m2));
    REQUIRE(almost_equal(moment<4>(std::vector{1_m, 2_m, 3_m, 4_m}),
                         2.5625_m * 1_m * 1_m * 1_m));
}

TEST_CASE("kurtosis") {
    REQUIRE(std::isnan(kurtosis(std::vector<double>{})));
    REQUIRE(almost_equal(kurtosis(std::vector{1., 2., 3., 4.}), -1.36));
    REQUIRE(almost_equal(
        kurtosis<KurtosisDef::Pearson>(std::vector{1., 2., 3., 4.}), 1.64));

    auto v = std::vector(500000, 1.);
    v[0] = 1E10;
    // Compare with result from scipy
    REQUIRE(almost_equal<60>(kurtosis(v), 499995.0000019997));

    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(nankurtosis(std::array{1., nan, 2., 3., nan, 4., nan}),
                         -1.36));
}

TEST_CASE("kurtosis physical units") {
    using namespace units::literals;
    REQUIRE(units::isnan(kurtosis(std::vector<units::mass<double>>{})));
    REQUIRE(almost_equal(kurtosis(std::array{1_m, 2_m, 3_m, 4_m}),
                         units::dimensionless<double>(-1.36)));
}

TEST_CASE("skew") {
    REQUIRE(std::isnan(skew(std::vector<double>{})));
    REQUIRE(almost_equal(skew(std::vector{1., 2., 3., 4.}), 0.));
    REQUIRE(almost_equal(skew(std::array{2., 8., 0., 4., 1., 9., 9., 0.}),
                         0.2650554122698573));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(
        nanskew(std::array{2., nan, 8., 0., nan, 4., 1., nan, 9., 9., 0.}),
        0.2650554122698573));
}

TEST_CASE("skew physical units") {
    using namespace units::literals;
    REQUIRE(units::isnan(skew(std::vector<units::mass<double>>{})));
    REQUIRE(almost_equal(skew(std::array{1_m, 2_m, 3_m, 4_m}),
                         units::dimensionless<double>(0.)));
    REQUIRE(almost_equal(
        skew(std::array{2_kg, 8_kg, 0_kg, 4_kg, 1_kg, 9_kg, 9_kg, 0_kg}),
        units::dimensionless<double>(0.2650554122698573)));
}

} // namespace scicpp::stats
