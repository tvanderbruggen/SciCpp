// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "quantity.hpp"

#include "scicpp/core/constants.hpp"
#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units/maths.hpp"
#include "scicpp/core/units/units.hpp"

#include <complex>
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
    REQUIRE(almost_equal(1_mmHg + 2_mmHg, 3_mmHg));

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
    REQUIRE(almost_equal(sqrt(2_ms).eval(), std::sqrt(2E-3)));
    REQUIRE(sqrt(2_ms) < sqrt(2_s));
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

TEST_CASE("Quantity complex") {
    const auto z1 = std::complex(1_m, 2_m);
    const auto z2 = std::complex(1_m, 1_m);
    const auto z3 = std::complex(1_Hz, 1_Hz);

    static_assert(meta::is_complex_v<std::decay_t<decltype(z1)>>);
    // static_assert(is_complex_quantity<decltype(z1)>());
    // static_assert(!is_complex_quantity<double>());
    // static_assert(!is_complex_quantity<std::complex<double>>());

    REQUIRE(almost_equal(std::real(z1), 1_m));
    REQUIRE(almost_equal(std::imag(z1), 2_m));
    REQUIRE(almost_equal(std::real(value(z1)), 1.0));
    REQUIRE(almost_equal(value(z1), std::complex(1.0, 2.0)));

    REQUIRE(almost_equal(z1 - z2, std::complex(0_m, 1_m)));
    REQUIRE(almost_equal(z1 + z2, std::complex(2_m, 3_m)));
    REQUIRE(almost_equal(z1 * z2, std::complex(-1_m2, 3_m2)));
    REQUIRE(almost_equal(z1 * z3, std::complex(-1_m_per_s, 3_m_per_s)));
    REQUIRE(almost_equal(1i * z1, std::complex(-2_m, 1_m)));
    REQUIRE(almost_equal(z1 * 1i, std::complex(-2_m, 1_m)));
}

} // namespace scicpp::units