// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "maths.hpp"

namespace scicpp::units {

using namespace literals;

TEST_CASE("fabs, abs") {
    REQUIRE(almost_equal(fabs(-3.14), 3.14));
    REQUIRE(almost_equal(fabs(-3.14_km), 3.14_km));
    REQUIRE(almost_equal(abs(-3.14), 3.14));
    REQUIRE(almost_equal(abs(-3.14_torr), 3.14_torr));
}

TEST_CASE("fmax, fmin, fdim") {
    REQUIRE(almost_equal(fmax(-3.14, 0.), 0.));
    REQUIRE(almost_equal(fmax(-3.14_m, 0_m), 0_m));
    REQUIRE(almost_equal(fmin(-3.14, 0.), -3.14));
    REQUIRE(almost_equal(fmin(-3.14_m, 0_m), -3.14_m));

    REQUIRE(almost_equal(fdim(4, 1), 3));
    REQUIRE(almost_equal(fdim(1, 4), 0));
    REQUIRE(almost_equal(fdim(4_nmol, 1_nmol), 3_nmol));
    REQUIRE(almost_equal(fdim(1_nmol, 4_nmol), 0_nmol));
}

TEST_CASE("fma") {
    REQUIRE(almost_equal(fma(1_m, 2_m, 3_m2), 5_m2));
    // REQUIRE(almost_equal(fma(1_m, 2_m2, 3_m2), 5_m2)); // Does not compile
    REQUIRE(almost_equal(fma(1_m, 2., 3_m), 5_m));
    REQUIRE(almost_equal(fma(1., 2_m, 3_m), 5_m));
    REQUIRE(almost_equal(fma(1., 2., 3.), 5.));
}

TEST_CASE("sqrt") {
    static_assert(
        std::is_same_v<decltype(sqrt(1_MHz))::scal::ratio, std::ratio<1000>>);

    REQUIRE(almost_equal(sqrt(9.), 3.));
    REQUIRE(almost_equal(sqrt(9_Hz), 3. * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(9_MHz), 3000. * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(9_uHz), 0.003 * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(1_uHz) * sqrt(9_Hz), 3_mHz));

    REQUIRE(almost_equal(sqrt(9_Hz) + sqrt(9_Hz), 6. * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(2_kHz) + sqrt(2_Hz), sqrt(2000_Hz) + sqrt(2_Hz)));
    REQUIRE(almost_equal(sqrt(9_MHz) + sqrt(9_Hz), 3003. * sqrt(1_Hz)));
}

TEST_CASE("cbrt") {
    REQUIRE(almost_equal(cbrt(8_Hz) + cbrt(8_Hz), 4. * cbrt(1_Hz)));
}

TEST_CASE("root") {
    REQUIRE(almost_equal(sqrt(9_Hz), root<2>(9_Hz)));
    REQUIRE(almost_equal(root<4>(16_Hz) + root<4>(16_Hz), 4. * root<4>(1_Hz)));
}

TEST_CASE("hypot") {
    REQUIRE(almost_equal(hypot(2_m, 2_m), 2_m * std::sqrt(2)));
    REQUIRE(almost_equal(hypot(2_m, 2_m, 2_m), 2_m * std::sqrt(3)));
}

TEST_CASE("floor") {
    REQUIRE(almost_equal(floor(2.7_m), 2_m));
}

TEST_CASE("ceil") {
    REQUIRE(almost_equal(ceil(2.7_m), 3_m));
}

TEST_CASE("sin") {
    REQUIRE(almost_equal(sin(0_deg), 0.));
    REQUIRE(almost_equal(sin(90_deg), 1.));
    REQUIRE(almost_equal(sin(1_mrad), std::sin(0.001)));
}

TEST_CASE("cos") {
    REQUIRE(almost_equal(cos(0_deg), 1.));
    REQUIRE(almost_equal(cos(90_deg), 0.));
    REQUIRE(almost_equal(cos(1_mrad), std::cos(0.001)));
}

TEST_CASE("tan") {
    REQUIRE(almost_equal(tan(0_deg), 0.));
    REQUIRE(almost_equal(tan(45_deg), 1.));
    REQUIRE(almost_equal(tan(1_mrad), std::tan(0.001)));
}

TEST_CASE("asin") {
    REQUIRE(almost_equal(asin(0.), 0_deg));
    REQUIRE(almost_equal(asin(1.), 90_deg));
    REQUIRE(almost_equal(asin(1_m / 1_m), 90_deg));
}

TEST_CASE("acos") {
    REQUIRE(almost_equal(acos(0.), 90_deg));
    REQUIRE(almost_equal(acos(1.), 0_deg));
    REQUIRE(almost_equal(acos(1_m / 1_m), 0_deg));
}

TEST_CASE("atan") {
    REQUIRE(almost_equal(atan(0.), 0_deg));
    REQUIRE(almost_equal(atan(1.), 45_deg));
    REQUIRE(almost_equal(atan(1_m / 1_m), 45_deg));
}

TEST_CASE("atan2") {
    REQUIRE(almost_equal(atan2(0., 1.), 0_deg));
    REQUIRE(almost_equal(atan2(1., 1.), 45_deg));
    REQUIRE(almost_equal(atan2(1_m, 1_m), 45_deg));
}

TEST_CASE("numeric_limits") {
    REQUIRE(isnan(std::numeric_limits<meter<>>::quiet_NaN()));
    REQUIRE(isinf(std::numeric_limits<meter<>>::infinity()));
    REQUIRE(!isfinite(std::numeric_limits<meter<>>::infinity()));
    REQUIRE(almost_equal(std::numeric_limits<meter<>>::epsilon().value(),
                         std::numeric_limits<double>::epsilon()));
    REQUIRE(almost_equal(std::numeric_limits<meter<>>::min().value(),
                         std::numeric_limits<double>::min()));
    REQUIRE(almost_equal(std::numeric_limits<meter<>>::max().value(),
                         std::numeric_limits<double>::max()));
    REQUIRE(almost_equal(std::numeric_limits<meter<>>::lowest().value(),
                         std::numeric_limits<double>::lowest()));
    REQUIRE(almost_equal(std::numeric_limits<meter<>>::round_error().value(),
                         std::numeric_limits<double>::round_error()));
    REQUIRE(!isnormal(std::numeric_limits<meter<>>::denorm_min()));
}

} // namespace scicpp::units