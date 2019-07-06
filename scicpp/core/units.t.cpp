// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/units.hpp"

#include <cstdio>

namespace scicpp::units {

using namespace literals;

// Power = Voltage x Current = Force x Speed
static_assert(
    is_same_quantity<power<double>,
                     quantity_multiply<voltage<double>, current<double>>>);

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

    REQUIRE((+L1).is_approx(L1));
    L1 += L2;
    REQUIRE(L1.is_approx<2>(5.14_m));
    REQUIRE((L1 + L2).is_approx<2>(8.28_m));

    // const auto T = 42._s;
    // L1 += T; // Doesn't compile

    REQUIRE((1._m + 1._km).is_approx(1001._m));
    REQUIRE((1._m + 1._km + 1._mm).is_approx(1001001._mm));
}

TEST_CASE("Units substractions") {
    auto L1 = 2._m;
    const auto L2 = 3.14_m;

    REQUIRE((-L1).is_approx(-L1));
    L1 -= L2;
    REQUIRE(L1.is_approx<2>(-1.14_m));
    REQUIRE((L1 - L2).is_approx(-4.28_m));

    REQUIRE((1._m - 1._km).is_approx(-999._m));
    REQUIRE((-1._km).is_approx(-1000._m));
}

TEST_CASE("Units multiplications") {
    const auto V = 12.2_V;
    const auto I = 0.5_A;
    const auto P = 6.1_W;
    REQUIRE(P.is_approx(V * I));
    // REQUIRE(P.is_approx(I)); // Doesn't compile
}

TEST_CASE("Units divisions") {
    const auto L = 4._m;
    const auto T = 2._s;
    const auto V = 2._m_per_s;
    REQUIRE(V.is_approx(L / T));
}

TEST_CASE("Units inv") {
    const auto T = 10._s;
    const auto f = 0.1_Hz;
    REQUIRE(f.is_approx(T.inv()));
}

TEST_CASE("Divide by constant") {
    REQUIRE((3.14 / 0.1_V).is_approx<2500>(31.4 * (1._V).inv()));
    REQUIRE((3.14_V / 0.1).is_approx<2500>(31.4_V));
}

TEST_CASE("Multiply by constant") {
    REQUIRE((10._V * 100._mA).is_approx(1._W));
    REQUIRE((3.14 * 10._V).is_approx<750>(31.4_V));
    REQUIRE((3.14 * 10._V * 100._mA).is_approx<750>(3140._mW));
    REQUIRE((10._V * 100._mA * 3.14).is_approx<750>(3140._mW));
}

TEST_CASE("Equivalence") {
    SECTION("Length") {
        REQUIRE((1._km).is_approx(1000._m));
        REQUIRE((1._inch).is_approx(2.54_cm));
        REQUIRE((1._angstrom).is_approx(0.1_nm));
    }

    SECTION("Energy") {
        REQUIRE((1._kWh).is_approx(3.6_MJ));
        REQUIRE((1._cal).is_approx(4.1855_J));
    }

    SECTION("Speed") {
        REQUIRE((2._km / 1._s).is_approx(2000._m_per_s));
        REQUIRE((2._km / 1._ms).is_approx(2000000._m_per_s));
        REQUIRE((1._km_per_h).is_approx(3.6_m_per_s));
    }

    SECTION("Time") {
        REQUIRE((1._h).is_approx(3600._s));
        REQUIRE((1._h).is_approx(60._min));
    }

    SECTION("Magnetic field") { REQUIRE((1._mT).is_approx<2>(10._G)); }

    SECTION("Energy") {
        REQUIRE((1._bar).is_approx(100._kPa));
    }
}

} // namespace scicpp::units