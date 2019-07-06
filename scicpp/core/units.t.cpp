// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "units.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"

#include <cstdio>

namespace scicpp::units {

using namespace literals;

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

    REQUIRE(almost_equal(1._m + 1._km, 1001._m));
    REQUIRE(almost_equal(1._m + 1._km + 1._mm, 1001001._mm));
}

TEST_CASE("Units substractions") {
    auto L1 = 2._m;
    const auto L2 = 3.14_m;

    REQUIRE(almost_equal(-L1, -L1));
    L1 -= L2;
    REQUIRE(almost_equal<2>(L1, -1.14_m));
    REQUIRE(almost_equal(L1 - L2, -4.28_m));

    REQUIRE(almost_equal(1._m - 1._km, -999._m));
    REQUIRE(almost_equal(-1._km, -1000._m));
}

TEST_CASE("Units multiplications") {
    const auto V = 12.2_V;
    const auto I = 0.5_A;
    const auto P = 6.1_W;
    REQUIRE(almost_equal(P, V * I));
    // REQUIRE(almost_equal(P, I)); // Doesn't compile
}

TEST_CASE("Units divisions") {
    const auto L = 4._m;
    const auto T = 2._s;
    const auto V = 2._m_per_s;
    REQUIRE(almost_equal(V, L / T));
}

TEST_CASE("Units inv") {
    const auto T = 10._s;
    const auto f = 0.1_Hz;
    REQUIRE(almost_equal(f, T.inv()));
}

TEST_CASE("Divide by constant") {
    REQUIRE(almost_equal<2500>(3.14 / 0.1_V, 31.4 * (1._V).inv()));
    REQUIRE(almost_equal<2500>(3.14_V / 0.1, 31.4_V));
}

TEST_CASE("Multiply by constant") {
    REQUIRE(almost_equal(10._V * 100._mA, 1._W));
    REQUIRE(almost_equal<750>(3.14 * 10._V, 31.4_V));
    REQUIRE(almost_equal<750>(3.14 * 10._V * 100._mA, 3140._mW));
    REQUIRE(almost_equal<750>(10._V * 100._mA * 3.14, 3140._mW));
}

TEST_CASE("Equivalence") {
    SECTION("Length") {
        REQUIRE(almost_equal(1._km, 1000._m));
        REQUIRE(almost_equal(1._inch, 2.54_cm));
        REQUIRE(almost_equal(1._angstrom, 0.1_nm));
    }

    SECTION("Surface") {
        REQUIRE(almost_equal(1._km2, 1000000._m2));
        REQUIRE(almost_equal(1._ha, 10000._m2));
    }

    SECTION("Energy") {
        REQUIRE(almost_equal(1._kWh, 3.6_MJ));
        REQUIRE(almost_equal(1._cal, 4.1855_J));
    }

    SECTION("Speed") {
        REQUIRE(almost_equal(2._km / 1._s, 2000._m_per_s));
        REQUIRE(almost_equal(2._km / 1._ms, 2000000._m_per_s));
        REQUIRE(almost_equal(1._km_per_h, 3.6_m_per_s));
    }

    SECTION("Time") {
        REQUIRE(almost_equal(1._h, 3600._s));
        REQUIRE(almost_equal(1._h, 60._min));
    }

    SECTION("Magnetic field") { REQUIRE(almost_equal<2>(1._mT, 10._G)); }

    SECTION("Pressure") {
        REQUIRE(almost_equal(1._bar, 100._kPa));
        REQUIRE(almost_equal(1._mmHg, 133.3223684210526315789_Pa));
    }
}

} // namespace scicpp::units