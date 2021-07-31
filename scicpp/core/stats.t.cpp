// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "stats.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units/units.hpp"

#include <cmath>
#include <iostream>

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

TEST_CASE("gmean") {
    REQUIRE(std::isnan(gmean(std::array<double, 0>{})));
    REQUIRE(almost_equal(gmean(std::array{1., 2., 3.}), 1.8171205928321397));
    REQUIRE(almost_equal(gmean(std::vector{1., 2., 3.}), 1.8171205928321397));
    REQUIRE(std::isnan(gmean(std::array{-1., -2., -3.})));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(nangmean(std::array{1., nan, 2., nan, 3.}),
                         1.8171205928321397));

    // Comparison with scipy results
    auto v = std::vector(500000, 1.);
    v[0] = 1E10;
    REQUIRE(almost_equal(gmean(v), 1.0000460527622559));

    const auto w = std::vector(500000, 1E10);
    REQUIRE(almost_equal(gmean(w), 9999999999.999826));
}

TEST_CASE("gmean physical units") {
    using namespace units::literals;
    REQUIRE(
        almost_equal(gmean(std::array{1_m, 2_m, 3_m}), 1.8171205928321397_m));
    REQUIRE(
        almost_equal(gmean(std::vector{1_m, 2_m, 3_m}), 1.8171205928321397_m));
    REQUIRE(units::isnan(gmean(std::array{-1_m, -2_m, -3_m})));
    REQUIRE(units::isnan(gmean(std::array<units::mass<double>, 0>{})));
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
    REQUIRE(almost_equal(
        std(std::array{-1., 1., 2., 3.}, [](auto x) { return x > 0.; }),
        0.816496580927726));
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
        std::numeric_limits<units::length<double>>::quiet_NaN();
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
    REQUIRE(
        almost_equal(tstd(x, {3_m, 17_m}, {true, false}), 4.183300132670378_m));
    REQUIRE(
        almost_equal(tstd(x, {3_m, 17_m}, {false, true}), 4.183300132670378_m));
}

TEST_CASE("sem") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(sem(std::array<double, 0>{})));
    REQUIRE(almost_equal<2>(sem(std::array{1., 2., 3.}), 0.5773502691896258));
    REQUIRE(almost_equal<2>(sem(std::vector{1., 2., 3.}), 0.5773502691896258));
    // nansem
    REQUIRE(almost_equal<2>(nansem(std::array{1., nan, 2., nan, 3.}),
                            0.5773502691896258));
    REQUIRE(almost_equal<2>(nansem(std::vector{1., nan, 2., nan, 3.}),
                            0.5773502691896258));
    // tsem
    const auto x = arange(0., 20.);
    REQUIRE(almost_equal(tsem(x, {3., 17.}), 1.1547005383792515));
    REQUIRE(almost_equal(tsem(x, {3., 17.}, {true, false}), 1.118033988749895));
    REQUIRE(almost_equal(tsem(x, {3., 17.}, {false, true}), 1.118033988749895));
}

TEST_CASE("sem physical units") {
    using namespace units::literals;
    constexpr auto nan =
        std::numeric_limits<units::length<double>>::quiet_NaN();
    REQUIRE(units::isnan(sem(std::array<units::length<double>, 0>{})));
    REQUIRE(
        almost_equal<2>(sem(std::array{1_m, 2_m, 3_m}), 0.5773502691896258_m));
    REQUIRE(
        almost_equal<2>(sem(std::vector{1_m, 2_m, 3_m}), 0.5773502691896258_m));
    // nansem
    REQUIRE(almost_equal<2>(nansem(std::array{1_m, nan, 2_m, nan, 3_m}),
                            0.5773502691896258_m));
    REQUIRE(almost_equal<2>(nansem(std::vector{1_m, nan, 2_m, nan, 3_m}),
                            0.5773502691896258_m));
    // tsem
    const auto x = arange(0_m, 20_m);
    REQUIRE(almost_equal(tsem(x, {3_m, 17_m}), 1.1547005383792515_m));
    REQUIRE(
        almost_equal(tsem(x, {3_m, 17_m}, {true, false}), 1.118033988749895_m));
    REQUIRE(
        almost_equal(tsem(x, {3_m, 17_m}, {false, true}), 1.118033988749895_m));
}

TEST_CASE("std/var/sem complex") {
    using namespace units::literals;

    const auto a = std::array{1. + 2.i, 3. - 4.i};
    REQUIRE(almost_equal(var(a), 10.));
    REQUIRE(almost_equal(std(a), std::sqrt(10.)));
    REQUIRE(almost_equal(sem(a), std::sqrt(10.)));

    const auto b = std::array{std::complex(1_m, 2_m), std::complex(3_m, -4_m)};
    REQUIRE(almost_equal(var(b), 10_m2));
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

TEST_CASE("covariance") {
    static_assert(
        float_equal(covariance(std::array{1., 2., 3.}, std::array{1., 2., 3.}),
                    var(std::array{1., 2., 3.})));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(std::isnan(
        covariance(std::array<double, 0>{}, std::array<double, 0>{})));

    auto v1 = std::vector(500000, 1.);
    v1[0] = 1E10;
    // printf("%.20f\n", covariance(v1, v1));
    // Compare with result from numpy
    REQUIRE(almost_equal<32>(covariance(v1, v1), 199999599960000.12));
    auto v2 = std::vector(500000, 1.);
    v2.back() = 1E10;
    // printf("%.20f\n", covariance<1>(v1, v2));
    // Compare with result from numpy (np.cov(v1, v1)[0][1]) for which ddof = 1
    REQUIRE(almost_equal<55>(covariance<1>(v1, v2), -400000799.9215977));

    REQUIRE(
        almost_equal(nancovariance(std::array{1., nan, 2., 3., nan, 4., nan},
                                   std::array{1., nan, 2., 3., nan, 4., nan}),
                     var(std::array{1., 2., 3., 4.})));
}

TEST_CASE("covariance complex") {
    using namespace operators;

    const auto a1 = std::array{1. + 2.i, 8. + 4.i};
    const auto a2 = std::array{6. + 3.i, 12.i};

    // printf("%.20f + %.20fi\n", covariance<1>(a1, a1).real(), covariance<1>(a1, a1).imag());
    // printf("%.20f + %.20fi\n", covariance<1>(a2, a2).real(), covariance<1>(a2, a2).imag());
    // printf("%.20f + %.20fi\n", covariance<1>(a1, a2).real(), covariance<1>(a1, a2).imag());

    REQUIRE(almost_equal(covariance<1>(a1, a1), 26.5 + 0.i));
    REQUIRE(almost_equal(covariance<1>(a2, a2), 58.5 + 0.i));
    REQUIRE(almost_equal(covariance<1>(a1, a2), -12. - 37.5i));

    const auto a3 = std::array{6., 12.};
    REQUIRE(almost_equal(covariance<1>(a3, a3), 18.));
    REQUIRE(almost_equal(covariance<1>(a1, a3), 21. + 6.i));
    REQUIRE(almost_equal(covariance<1>(a3, a1), 21. - 6.i));

    // Test with array length > 64
    // Compare with numpy results:
    // >>> np.cov(np.linspace(1, 40, 233) + 1j * np.linspace(50, 865, 233),
    //            np.linspace(5,489,233) + 1j * np.linspace(18,8486,233))
    // array([[  56198.29353076693   +0.j            ,
    //          584169.9776307966 +5420.055142687268j],
    //        [ 584169.9776307966 -5420.055142687268j,
    //         6072852.364744352     +0.j            ]])

    const auto a = linspace(1., 40., 233) + 1.i * linspace(50., 865., 233);
    const auto b = linspace(5., 489., 233) + 1.i * linspace(18., 8486., 233);

    REQUIRE(almost_equal<25>(covariance<1>(a, b),
                             584169.9776307966 + 5420.055142687268i));
    REQUIRE(almost_equal<30>(covariance<1>(b, a),
                             584169.9776307966 - 5420.055142687268i));
    REQUIRE(almost_equal<25>(covariance<1>(a, a), 56198.29353076693 + 0.i));
    REQUIRE(almost_equal<25>(covariance<1>(b, b), 6072852.364744352 + 0.i));
}

TEST_CASE("covariance physical units") {
    using namespace units::literals;
    REQUIRE(units::isnan(covariance(std::vector<units::mass<double>>{},
                                    std::vector<units::length<double>>{})));
    static_assert(
        float_equal(covariance(std::array{1_V / 1_Hz, 2_V / 1_Hz, 3_V / 1_Hz},
                               std::array{1_V, 2_V, 3_V}),
                    var(std::array{1_V_per_rtHz, 2_V_per_rtHz, 3_V_per_rtHz})));

    auto v1 = std::vector(500000, 1_W);
    v1[0] = 1E10_W;
    auto v2 = std::vector(500000, 1_s);
    v2.back() = 1E10_s;
    REQUIRE(almost_equal<55>(covariance<1>(v1, v2), -400000799.9215977_J));
}

TEST_CASE("cov") {
    // >>> np.cov(np.linspace(1, 40, 233), np.linspace(50, 865, 233))
    // array([[  128.39371841557679,  2683.0995002229497 ],
    //        [ 2683.0995002229497 , 56069.89981235136   ]])

    const auto a = linspace(1., 40., 233);
    const auto b = linspace(50., 865., 233);
    // std::cout << cov(a, b) << "\n";

    Eigen::Matrix<double, 2, 2> m;
    m << 128.39371841557679, 2683.0995002229497, //
        2683.0995002229497, 56069.89981235136;   //
    REQUIRE(m.isApprox(cov(a, b)));
}

TEST_CASE("cov complex") {
    using namespace operators;
    // >>> np.cov(np.linspace(1, 40, 2334879) + 1j * np.linspace(50, 865, 2334879),
    //            np.linspace(5,489,2334879) + 1j * np.linspace(18,8486,2334879))
    // array([[  55478.904616093    +0.j             ,
    //          576692.0743033098+5350.6735415460425j],
    //        [ 576692.0743033098-5350.6735415460425j,
    //         5995114.369563842    +0.j             ]])

    const auto a =
        linspace(1., 40., 2334879) + 1.i * linspace(50., 865., 2334879);
    const auto b =
        linspace(5., 489., 2334879) + 1.i * linspace(18., 8486., 2334879);
    // std::cout << cov(a, b) << "\n";

    Eigen::Matrix<std::complex<double>, 2, 2> m;
    m << 55478.904616093 + 0.i, 576692.0743033098 + 5350.6735415460425i,  //
        576692.0743033098 - 5350.6735415460425i, 5995114.369563842 + 0.i; //
    REQUIRE(m.isApprox(cov(a, b)));
}

} // namespace scicpp::stats
