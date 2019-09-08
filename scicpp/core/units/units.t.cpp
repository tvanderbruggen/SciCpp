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

TEST_CASE("Dimensionless") {
    static_assert(is_dimensionless<quantity_divide<length<int>, length<int>>>);
}

TEST_CASE("Length") {
    static_assert(is_length<meter<>>);
    static_assert(is_length<kilometer<>>);

    static_assert(meter<int>(2000) == kilometer<int>(2));
    REQUIRE(almost_equal(meter<>(12), 12_m));
    REQUIRE(almost_equal(millimeter<>(1), 1_mm));
    REQUIRE(almost_equal(kilometer<>(1), 1_km));
    REQUIRE(almost_equal(1_km, 1000._m));
    REQUIRE(almost_equal<2>(1_in, 2.54_cm));
    REQUIRE(almost_equal<2>(1_ft, 12_in));
    REQUIRE(almost_equal<2>(1_ft, 12_in));
    REQUIRE(almost_equal<2>(1_yd, 3_ft));
    REQUIRE(almost_equal<2>(1_angstrom, 0.1_nm));
    REQUIRE(almost_equal<2>(1_mi, 1.609344_km));
    REQUIRE(almost_equal<2>(1_mi, 1760_yd));
    REQUIRE(almost_equal<2>(1_mi, 5280_ft));
    REQUIRE(almost_equal<2>(1_nmi, 1852_m));
    REQUIRE(almost_equal<2>(1_au, 149597870700_m));
    REQUIRE(almost_equal<2>(1_ly, 9460730472580.8_km));
    REQUIRE(almost_equal<2>(1_pc, 30856775814671.9_km));
}

TEST_CASE("Planar angle") {
    static_assert(is_planar_angle<radian<>>);
    static_assert(is_planar_angle<degree<>>);
    static_assert(!is_planar_angle<meter<>>);

    static_assert(radian<int>(0) == degree<int>(0));
    REQUIRE(almost_equal(radian<>(pi<double>), degree<>(180.)));
    REQUIRE(almost_equal<750>(radian<long double>(pi<long double>),
                              degree<long double>(180.)));
    REQUIRE(almost_equal(radian<>(pi<double> / 2), degree<>(90.)));
    REQUIRE(almost_equal(radian<>(pi<double> / 4), degree<>(45.)));
    REQUIRE(almost_equal(3.1415926535897932_rad, 180_deg));
    REQUIRE(almost_equal(0.5_turn, 180_deg));
    REQUIRE(almost_equal(1_turn, radian<>(2 * pi<double>)));
}

TEST_CASE("Area") {
    static_assert(is_area<square_meter<>>);
    static_assert(square_meter<int>(1) == square_millimeter<int>(1000000));
    static_assert(square_meter<int>(1) == square_centimeter<int>(10000));
    static_assert(square_kilometer<int>(1) == square_meter<int>(1000000));
    REQUIRE(almost_equal(1_m2, 1000000000000_um2));
    REQUIRE(almost_equal(1_m2, 1000000_mm2));
    REQUIRE(almost_equal(1_m2, 10000_cm2));
    REQUIRE(almost_equal(1_km2, 1000000_m2));
    REQUIRE(almost_equal(1_Mm2, 1000000000000_m2));
    REQUIRE(almost_equal(1_in2, 6.4516_cm2));
    REQUIRE(almost_equal(1_ft2, 0.09290304_m2));
    REQUIRE(almost_equal(1_yd2, 0.83612736_m2));
    REQUIRE(almost_equal<2>(1_mi2, 2.589988110336_km2));
    REQUIRE(almost_equal(1_ha, 100_a));
    REQUIRE(almost_equal(1_ha, 10000_m2));
}

TEST_CASE("Volume") {
    static_assert(is_volume<cubic_meter<>>);
    static_assert(cubic_meter<int>(1) == cubic_centimeter<int>(1000000));
    static_assert(cubic_meter<int>(1) == cubic_millimeter<int>(1000000000));
    REQUIRE(almost_equal(1_m3, 1000000_cm3));
    REQUIRE(almost_equal<2>(1_cm3, 1000_mm3));
    REQUIRE(almost_equal(1_km3, 1000000000_m3));
    REQUIRE(almost_equal(1_L, 1000_cm3));
    REQUIRE(almost_equal(1_mL, 1_cm3));
    REQUIRE(almost_equal(1_uL, 1_mm3));
    REQUIRE(almost_equal(1_hL, 100_L));
    REQUIRE(almost_equal(1_in3, 0.000016387064_m3));
    REQUIRE(almost_equal(1_in3, 16.387064_mL));
    REQUIRE(almost_equal(1_ft3, 0.028316846592_m3));
    REQUIRE(almost_equal(1_yd3, 0.764554857984_m3));
    REQUIRE(almost_equal(1_mi3, 4.168181825440579584_km3));
}

TEST_CASE("Energy") {
    static_assert(is_energy<joule<>>);
    static_assert(nanojoule<int64_t>(1) == picojoule<int64_t>(1000));
    REQUIRE(almost_equal(1_kWh, 3.6_MJ));
    REQUIRE(almost_equal(1_cal, 4.1855_J));
}

TEST_CASE("Speed") {
    static_assert(is_speed<kilometer_per_second<>>);
    static_assert(kilometer_per_second<int>(3) == meter_per_second<int>(3000));
    REQUIRE(almost_equal(2_km / 1_s, 2000_m_per_s));
    REQUIRE(almost_equal(2_km / 1_ms, 2000000_m_per_s));
    REQUIRE(almost_equal(2_mm / 1_ms, 2_m_per_s));
    REQUIRE(almost_equal(1_km_per_h, 3.6_m_per_s));
    REQUIRE(almost_equal(1_kn, 1.852_km_per_h));
    REQUIRE(almost_equal(1_mph, 0.44704_m_per_s));
}

TEST_CASE("Acceleration") {
    REQUIRE(almost_equal(2_km / 10_s / 10_s, 20_m_per_s2));
    REQUIRE(almost_equal(1_Gal, 0.01_m_per_s2));
    REQUIRE(almost_equal(1_uGal, 1E-6_Gal));
}

TEST_CASE("Time") {
    static_assert(!is_time<double>);
    static_assert(is_time<minute<int>>);
    static_assert(minute<int>(60) == hour<int>(1));
    REQUIRE(almost_equal(1_h, 3600_s));
    REQUIRE(almost_equal(1_h, 60_min));
    REQUIRE(almost_equal(1_h, 60_min));
    REQUIRE(almost_equal(1_day, 24_h));
    REQUIRE(almost_equal(1_week, 7_day));
}

TEST_CASE("Resistance") {
    static_assert(gigaohm<int>(1) == megaohm<int>(1000));
    REQUIRE(almost_equal(1. / 10_Ohm, 0.1_S));
    REQUIRE(almost_equal(10_Ohm * 100_mA, 1_V));
}

TEST_CASE("Magnetic field") {
    static_assert(millitesla<int>(10) == gauss<int>(100));
    REQUIRE(almost_equal(1_mT, 10_G));
}

TEST_CASE("Pressure") {
    static_assert(hectopascal<int>(1) == pascal<int>(100));
    REQUIRE(almost_equal(1_bar, 100_kPa));
    REQUIRE(almost_equal(1_mmHg, 133.3223684210526315789_Pa));
    REQUIRE(almost_equal(1_mmHg, 1._torr));
    REQUIRE(almost_equal(1_psi, 0.0689476_bar));
    REQUIRE(almost_equal(1_atm, 101.325_kPa));
}

TEST_CASE("Data quantity") {
    REQUIRE(almost_equal(1_B, 8_b));
    REQUIRE(1_B >= 8_b);
    REQUIRE(1_B <= 8_b);
    REQUIRE(1_B >= 8_b);
    REQUIRE(1_MiB >= 2_kiB);
    REQUIRE(1_MiB > 2_kiB);
    REQUIRE(2_kiB < 1_MiB);
    REQUIRE(almost_equal(1_kiB, 1024_B));
}

TEST_CASE("Catalytic activity") {
    REQUIRE(almost_equal<2>(enzyme_unit<>(1.0), 16.67_nkat));
}

} // namespace scicpp::units