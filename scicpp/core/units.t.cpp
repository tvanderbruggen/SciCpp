// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "units.hpp"

#include "scicpp/core/constants.hpp"
#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"

#include <cstdio>
#include <type_traits>

namespace scicpp::units {

using namespace literals;

TEST_CASE("A quantity is an arithmetic-like type") {
    static_assert(std::is_trivial_v<meter<>>);
    static_assert(std::is_trivially_copyable_v<meter<>>);
    static_assert(std::is_trivially_destructible_v<meter<>>);
    static_assert(std::is_trivially_default_constructible_v<meter<>>);
    static_assert(std::is_trivially_copy_constructible_v<meter<>>);
    static_assert(std::is_trivially_copy_assignable_v<meter<>>);
    static_assert(std::is_trivially_move_constructible_v<meter<>>);
    static_assert(std::is_trivially_move_assignable_v<meter<>>);
}

TEST_CASE("Casting") {
    SECTION("Implicit conversions") {
        // Implicit conversions between floating point types are allowed
        const auto L = 1_m;
        REQUIRE(almost_equal(length<float>(L).value(), float(L.value())));

        // Implicit conversions from int to float are allowed
        const auto Mk = kilogram<int>(1);
        REQUIRE(almost_equal<200>(gram<double>(Mk).value(),
                                  1000. * double(Mk.value())));

        const auto Mg = gram<int>(1);
        REQUIRE(almost_equal<200>(1000. * kilogram<double>(Mg).value(),
                                  double(Mg.value())));

        // Implicit conversion between integral types are allowed
        // only if input Scale is an exact multiple of output scale
        REQUIRE(millisecond<int32_t>(second<int64_t>(1)) ==
                millisecond<int32_t>(1000));

        // Does not compile
        // REQUIRE(second<int32_t>(millisecond<int64_t>(1)) == second<int32_t>(1000));
    }

    SECTION("Explicit conversions") {
        REQUIRE(quantity_cast<second<int32_t>>(millisecond<int64_t>(1)) ==
                second<int32_t>(0));
    }
}

TEST_CASE("Units comparisons") {
    REQUIRE(length<int>(2) == length<int>(2));
    REQUIRE(length<int>(2) != length<int>(3));
    REQUIRE(length<int>(2) < length<int>(3));
    REQUIRE(length<int>(2) <= length<int>(3));
    REQUIRE(length<int>(3) > length<int>(2));
    REQUIRE(length<int>(3) >= length<int>(2));
    // REQUIRE(length<int>(2) == second<int>(2)); // Doesn't compile

    REQUIRE(1._km > 1._m);
}

TEST_CASE("Units additions") {
    auto L1 = 2._m;
    const auto L2 = 3.14_m;

    REQUIRE(almost_equal(+L1, L1));
    L1 += L2;
    REQUIRE(almost_equal<2>(L1, 5.14_m));
    REQUIRE(almost_equal<2>(L1 + L2, 8.28_m));

    // const auto T = 42._s;
    // L1 += T; // Doesn't compile

    static_assert(meter<int>(1) + kilometer<int>(2) == meter<int>(2001));
    REQUIRE(almost_equal(1._m + 1._km, 1001._m));
    REQUIRE(almost_equal(1._m + 1._km + 1._mm, 1001001._mm));

    REQUIRE(almost_equal(++1._m, 2._m));
    L1++;
    REQUIRE(almost_equal<2>(L1, 6.14_m));

    REQUIRE(almost_equal(--2._m, 1._m));
    L1--;
    REQUIRE(almost_equal<2>(L1, 5.14_m));

    L1 += 6_cm;
    REQUIRE(almost_equal<2>(L1, 5.2_m));

    REQUIRE(almost_equal(0_degC, 273.15_K));
    REQUIRE(almost_equal(0_K, -273.15_degC));
    // print(quantity_cast<kelvin<>>(10_degC));
    // print(quantity_cast<celsius<>>(10_K));
    REQUIRE(almost_equal(quantity_cast<kelvin<>>(10_degC), 283.15_K));
    REQUIRE(almost_equal(quantity_cast<celsius<>>(10_K), -263.15_degC));

    REQUIRE(almost_equal(10_degC + 15_degC, 298.15_K));
    REQUIRE(almost_equal(quantity_cast<kelvin<>>(10_degC) + 15_K, 298.15_K));
    REQUIRE(almost_equal<2>(0_degF, -17.77777777777777777777_degC));
    REQUIRE(almost_equal<2>(32_degF, 0_degC));
    REQUIRE(almost_equal<2>(212_degF, 100_degC));
    REQUIRE(almost_equal<2>(quantity_cast<celsius<>>(32_degF) + 100_degC,
                            100_degC));

    static_assert(kelvin<int>(0) != celsius<int>(-73));
    static_assert(kelvin<int>(0) < celsius<int>(0));
    static_assert(kelvin<int>(273) < celsius<int>(0));
    static_assert(kelvin<int>(300) > celsius<int>(0));
}

TEST_CASE("Units substractions") {
    auto L1 = 2._m;
    const auto L2 = 3.14_m;

    REQUIRE(almost_equal(-L1, -L1));
    L1 -= L2;
    REQUIRE(almost_equal<2>(L1, -1.14_m));
    REQUIRE(almost_equal(L1 - L2, -4.28_m));

    static_assert(kilometer<int>(2) - meter<int>(1) == meter<int>(1999));
    REQUIRE(almost_equal(1._m - 1._km, -999._m));
    REQUIRE(almost_equal(-1._km, -1000._m));
    REQUIRE(almost_equal(-1_km, -1000_m));

    REQUIRE(almost_equal(10_degC - 15_degC, -5_degC));
    REQUIRE(almost_equal(10_degF + 25_degF, 35_degF));
}

TEST_CASE("Units multiplications") {
    const auto V = 12.2_V;
    const auto I = 0.5_A;
    const auto P = 6.1_W;
    REQUIRE(almost_equal(P, V * I));
    // REQUIRE(almost_equal(P, I)); // Doesn't compile

    static_assert(volt<int>(10) * milliampere<int>(100) == watt<int>(1));
    REQUIRE(almost_equal(10._V * 100._mA, 1._W));
    REQUIRE(almost_equal(10_V * 100_mA, 1_W));

    REQUIRE(almost_equal(5_degC * 1_m2, 5_K * 1_m2));

    auto L = 1_m;
    L *= 1000.;
    REQUIRE(almost_equal(L, 1_km));
}

TEST_CASE("Units divisions") {
    const auto L = 4._m;
    const auto T = 2._s;
    const auto V = 2._m_per_s;
    REQUIRE(almost_equal(V, L / T));

    REQUIRE(almost_equal(5_degC / 1_W, 5_K / 1_W));
    REQUIRE(almost_equal(5_degC / 1_kW, 5_mK / 1_W));

    auto M = 1_kg;
    M /= 1000.;
    REQUIRE(almost_equal(M, 1_g));
}

TEST_CASE("Units inv") {
    REQUIRE(almost_equal(0.1_Hz, (10_s).inv()));
    REQUIRE(almost_equal((10_degC).inv(), 1. / 10_K));
}

TEST_CASE("Units root") {
    static_assert(std::is_same_v<scale_root<scale<std::mega>, 2>::ratio,
                                 std::ratio<1000>>);
    static_assert(
        std::is_same_v<decltype(sqrt(1_MHz))::scal::ratio, std::ratio<1000>>);

    REQUIRE(almost_equal(sqrt(9_Hz), 3. * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(9_MHz), 3000. * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(9_uHz), 0.003 * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(1_uHz) * sqrt(9_Hz), 3_mHz));

    REQUIRE(almost_equal(sqrt(9_Hz) + sqrt(9_Hz), 6. * sqrt(1_Hz)));
    REQUIRE(almost_equal(sqrt(9_MHz) + sqrt(9_Hz), 3003. * sqrt(1_Hz)));
    REQUIRE(almost_equal(cbrt(8_Hz) + cbrt(8_Hz), 4. * cbrt(1_Hz)));
    REQUIRE(almost_equal(root<4>(16_Hz) + root<4>(16_Hz), 4. * root<4>(1_Hz)));
}

TEST_CASE("Divide by constant") {
    REQUIRE(almost_equal<2>(3.14 / 0.1_V, 31.4 * (1._V).inv()));
    REQUIRE(almost_equal<2>(3.14_V / 0.1, 31.4_V));
    REQUIRE(almost_equal<2>(3.14_degF / 0.1, 31.4_degF));
}

TEST_CASE("Multiply by constant") {
    REQUIRE(almost_equal<2>(3.14 * 10._V, 31.4_V));
    REQUIRE(almost_equal(3.14 * 10._V * 100._mA, 3140._mW));
    REQUIRE(almost_equal(10._V * 100._mA * 3.14, 3140._mW));
    REQUIRE(almost_equal<2>(3.14_degF * 10, 31.4_degF));
    REQUIRE(almost_equal<2>(10 * 3.14_degF, 31.4_degF));
}

TEST_CASE("Equivalence") {
    SECTION("Length") {
        static_assert(meter<int>(2000) == kilometer<int>(2));
        REQUIRE(almost_equal(meter<>(12), 12_m));
        REQUIRE(almost_equal(millimeter<>(1), 1_mm));
        REQUIRE(almost_equal(kilometer<>(1), 1_km));
        REQUIRE(almost_equal(1._km, 1000._m));
        REQUIRE(almost_equal<2>(1._in, 2.54_cm));
        REQUIRE(almost_equal<2>(1._ft, 12._in));
        REQUIRE(almost_equal<2>(1_ft, 12_in));
        REQUIRE(almost_equal<2>(1._yd, 3._ft));
        REQUIRE(almost_equal<2>(1._angstrom, 0.1_nm));
    }

    SECTION("Angle") {
        static_assert(is_angle_v<radian<>>);
        static_assert(is_angle_v<degree<>>);
        static_assert(!is_angle_v<meter<>>);
        static_assert(radian<int>(0) == degree<int>(0));

        REQUIRE(almost_equal(radian<>(pi<double>), degree<>(180.)));
        REQUIRE(almost_equal<750>(radian<long double>(pi<long double>),
                                  degree<long double>(180.)));
        REQUIRE(almost_equal(radian<>(pi<double> / 2), degree<>(90.)));
        REQUIRE(almost_equal(radian<>(pi<double> / 4), degree<>(45.)));
        REQUIRE(almost_equal(3.1415926535897932_rad, 180_deg));
    }

    SECTION("Surface") {
        REQUIRE(almost_equal(1._km2, 1000000._m2));
        REQUIRE(almost_equal(1._ha, 10000._m2));
    }

    SECTION("Energy") {
        static_assert(nanojoule<int64_t>(1) == picojoule<int64_t>(1000));
        REQUIRE(almost_equal(1._kWh, 3.6_MJ));
        REQUIRE(almost_equal(1._cal, 4.1855_J));
    }

    SECTION("Speed") {
        static_assert(kilometer_per_second<int>(3) ==
                      meter_per_second<int>(3000));
        REQUIRE(almost_equal(2._km / 1._s, 2000._m_per_s));
        REQUIRE(almost_equal(2._km / 1._ms, 2000000._m_per_s));
        REQUIRE(almost_equal(2_mm / 1_ms, 2_m_per_s));
        REQUIRE(almost_equal(1._km_per_h, 3.6_m_per_s));
    }

    SECTION("Acceleration") {
        REQUIRE(almost_equal(2_km / 10_s / 10_s, 20_m_per_s2));
    }

    SECTION("Time") {
        static_assert(minute<int>(60) == hour<int>(1));
        REQUIRE(almost_equal(1._h, 3600._s));
        REQUIRE(almost_equal(1._h, 60._min));
        REQUIRE(almost_equal(1_h, 60_min));
    }

    SECTION("Resistance") {
        static_assert(gigaohm<int>(1) == megaohm<int>(1000));
        REQUIRE(almost_equal(1. / 10_Ohm, 0.1_S));
        REQUIRE(almost_equal(10_Ohm * 100_mA, 1_V));
    }

    SECTION("Magnetic field") {
        static_assert(millitesla<int>(10) == gauss<int>(100));
        REQUIRE(almost_equal(1._mT, 10._G));
    }

    SECTION("Pressure") {
        static_assert(hectopascal<int>(1) == pascal<int>(100));
        REQUIRE(almost_equal(1._bar, 100._kPa));
        REQUIRE(almost_equal(1._mmHg, 133.3223684210526315789_Pa));
        REQUIRE(almost_equal(1._mmHg, 1._torr));
        REQUIRE(almost_equal(1._psi, 0.0689476_bar));
        REQUIRE(almost_equal(1._atm, 101.325_kPa));
    }

    SECTION("Data quantity") {
        REQUIRE(almost_equal(1_B, 8_b));
        REQUIRE(1_B >= 8_b);
        REQUIRE(1_B <= 8_b);
        REQUIRE(1_B >= 8_b);
        REQUIRE(1_MiB >= 2_kiB);
        REQUIRE(1_MiB > 2_kiB);
        REQUIRE(2_kiB < 1_MiB);
        REQUIRE(almost_equal(1_kiB, 1024_B));
    }
}

} // namespace scicpp::units