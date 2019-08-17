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