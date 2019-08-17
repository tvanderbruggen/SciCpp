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

TEST_CASE("Length") {
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

TEST_CASE("Angle") {
    static_assert(is_planar_angle_v<radian<>>);
    static_assert(is_planar_angle_v<degree<>>);
    static_assert(!is_planar_angle_v<meter<>>);
    static_assert(radian<int>(0) == degree<int>(0));

    REQUIRE(almost_equal(radian<>(pi<double>), degree<>(180.)));
    REQUIRE(almost_equal<750>(radian<long double>(pi<long double>),
                              degree<long double>(180.)));
    REQUIRE(almost_equal(radian<>(pi<double> / 2), degree<>(90.)));
    REQUIRE(almost_equal(radian<>(pi<double> / 4), degree<>(45.)));
    REQUIRE(almost_equal(3.1415926535897932_rad, 180_deg));
}

TEST_CASE("Surface") {
    REQUIRE(almost_equal(1._km2, 1000000._m2));
    REQUIRE(almost_equal(1._ha, 10000._m2));
}

TEST_CASE("Energy") {
    static_assert(nanojoule<int64_t>(1) == picojoule<int64_t>(1000));
    REQUIRE(almost_equal(1._kWh, 3.6_MJ));
    REQUIRE(almost_equal(1._cal, 4.1855_J));
}

TEST_CASE("Speed") {
    static_assert(kilometer_per_second<int>(3) == meter_per_second<int>(3000));
    REQUIRE(almost_equal(2._km / 1._s, 2000._m_per_s));
    REQUIRE(almost_equal(2._km / 1._ms, 2000000._m_per_s));
    REQUIRE(almost_equal(2_mm / 1_ms, 2_m_per_s));
    REQUIRE(almost_equal(1._km_per_h, 3.6_m_per_s));
}

TEST_CASE("Acceleration") {
    REQUIRE(almost_equal(2_km / 10_s / 10_s, 20_m_per_s2));
}

TEST_CASE("Time") {
    static_assert(minute<int>(60) == hour<int>(1));
    REQUIRE(almost_equal(1._h, 3600._s));
    REQUIRE(almost_equal(1._h, 60._min));
    REQUIRE(almost_equal(1_h, 60_min));
}

TEST_CASE("Resistance") {
    static_assert(gigaohm<int>(1) == megaohm<int>(1000));
    REQUIRE(almost_equal(1. / 10_Ohm, 0.1_S));
    REQUIRE(almost_equal(10_Ohm * 100_mA, 1_V));
}

TEST_CASE("Magnetic field") {
    static_assert(millitesla<int>(10) == gauss<int>(100));
    REQUIRE(almost_equal(1._mT, 10._G));
}

TEST_CASE("Pressure") {
    static_assert(hectopascal<int>(1) == pascal<int>(100));
    REQUIRE(almost_equal(1._bar, 100._kPa));
    REQUIRE(almost_equal(1._mmHg, 133.3223684210526315789_Pa));
    REQUIRE(almost_equal(1._mmHg, 1._torr));
    REQUIRE(almost_equal(1._psi, 0.0689476_bar));
    REQUIRE(almost_equal(1._atm, 101.325_kPa));
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

} // namespace scicpp::units