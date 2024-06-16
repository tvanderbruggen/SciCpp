// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "maths.hpp"

#include "scicpp/core/equal.hpp"

#include <cfenv>

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

    REQUIRE(fdim(4, 1) == 3);
    REQUIRE(fdim(1, 4) == 0);
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

    REQUIRE(almost_equal(sqrt(1_uH * 1_uF), 1_us));
    REQUIRE(almost_equal(sqrt(1_uH / 1_uF), 1_Ohm));
    REQUIRE(almost_equal(1.0 / sqrt(1_uH * 1_uF), 1_MHz));
    REQUIRE(almost_equal(1.0 / sqrt(1_uH * 1_uF), 1_MHz));
}

TEST_CASE("cbrt") {
    REQUIRE(almost_equal(cbrt(8_Hz) + cbrt(8_Hz), 4. * cbrt(1_Hz)));
}

TEST_CASE("pow") {
    REQUIRE(almost_equal(pow<0>(2.), 1.));
    REQUIRE(almost_equal(pow<3>(3.), 27.));

    REQUIRE(almost_equal(pow<0>(2_m), dimensionless<double>(1.)));
    REQUIRE(almost_equal(pow<3>(3_m), 27_m3));
}

TEST_CASE("root") {
    REQUIRE(almost_equal(sqrt(9_Hz), root<2>(9_Hz)));
    REQUIRE(almost_equal(root<4>(16_Hz) + root<4>(16_Hz), 4. * root<4>(1_Hz)));
}

TEST_CASE("hypot") {
    REQUIRE(almost_equal(hypot(2_m, 2_m), 2_m * std::sqrt(2)));
    REQUIRE(almost_equal(hypot(2_m, 2_m, 2_m), 2_m * std::sqrt(3)));
}

TEST_CASE("floor, ceil, trunc, round, nearbyint, rint") {
    REQUIRE(almost_equal(floor(2.7_m), 2_m));
    REQUIRE(almost_equal(ceil(2.7_m), 3_m));
    REQUIRE(almost_equal(trunc(2.7_m), 2_m));
    REQUIRE(almost_equal(round(2.7_m), 3_m));
    std::fesetround(FE_TONEAREST);
    REQUIRE(almost_equal(nearbyint(2.1_m), 2_m));
    REQUIRE(almost_equal(rint(2.1_m), 2_m));
}

TEST_CASE("nextafter") {
    REQUIRE(almost_equal(nextafter(1_m, 2_m), std::nextafter(1, 2) * 1_m));
}

TEST_CASE("copysign") {
    REQUIRE(almost_equal(copysign(1_m, 2_m), 1_m));
    REQUIRE(almost_equal(copysign(1_m, -2_m), -1_m));
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

TEST_CASE("sinc") {
    REQUIRE(almost_equal(sinc(0.), 1.));
    REQUIRE(almost_equal(sinc(-0.), 1.));
    REQUIRE(almost_equal(sinc(1.), 0.));
    REQUIRE(almost_equal(sinc(-1.), 0.));
    REQUIRE(almost_equal(sinc(1_m / 1_m), 0.));
    REQUIRE(almost_equal(sinc(1E-5), 0.9999999998355066));
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
    REQUIRE(signbit(-1_m));
    REQUIRE(!signbit(1_m));
}

TEST_CASE("exp, log") {
    REQUIRE(almost_equal(exp(0.), 1.));
    REQUIRE(almost_equal(exp(0_m / 1_m), 1.));

    REQUIRE(almost_equal(expm1(0.), 0.));
    REQUIRE(almost_equal(expm1(0_m / 1_m), 0.));

    REQUIRE(almost_equal(exp2(0.), 1.));
    REQUIRE(almost_equal(exp2(0_m / 1_m), 1.));

    REQUIRE(almost_equal(log(1.), 0.));
    REQUIRE(almost_equal(log(1_m / 1_m), 0.));

    REQUIRE(almost_equal(log2(1.), 0.));
    REQUIRE(almost_equal(log2(1_m / 1_m), 0.));

    REQUIRE(almost_equal(log10(1.), 0.));
    REQUIRE(almost_equal(log10(1_m / 1_m), 0.));

    REQUIRE(almost_equal(log1p(0.), 0.));
    REQUIRE(almost_equal(log1p(0_m / 1_m), 0.));
}

TEST_CASE("Complex") {
    const auto z1 = std::complex(1_m, 2_m);
    const auto z2 = std::complex(1_m, 1_m);

    REQUIRE(almost_equal<2>(units::norm(z1), 5_m2));
    REQUIRE(almost_equal<2>(units::norm(std::complex(1.0, 2.0)), 5.0));
    REQUIRE(almost_equal<2>(units::norm(2.0), 4.0));
    REQUIRE(almost_equal<2>(units::norm(2_m), 4_m2));

    REQUIRE(almost_equal(units::abs(z1), 1_m * std::sqrt(5.0)));

    REQUIRE(almost_equal(units::arg(z2), 45_deg));
    REQUIRE(almost_equal(units::arg(1_m), 0_deg));

    REQUIRE(almost_equal(units::polar(1., 0_rad), 1. + 0.i));
    REQUIRE(
        almost_equal<2>(units::polar(1., 45_deg),
                        std::complex(1. / std::sqrt(2.), 1. / std::sqrt(2.))));
    REQUIRE(almost_equal<2>(
        units::polar(1_m, 45_deg),
        std::complex(1_m / std::sqrt(2.), 1_m / std::sqrt(2.))));

    REQUIRE(almost_equal(std::conj(z1), std::complex(1_m, -2_m)));
    REQUIRE(almost_equal(units::proj(z1), z1));
    constexpr auto inf_m = std::numeric_limits<meter<>>::infinity();
    REQUIRE(almost_equal(units::proj(std::complex(inf_m, -1_m)),
                         std::complex(inf_m, -0_m)));
    REQUIRE(almost_equal(units::proj(std::complex(0_m, -inf_m)),
                         std::complex(inf_m, -0_m)));
}

} // namespace scicpp::units