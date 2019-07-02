// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/units.hpp"

#include <cstdio>

namespace scicpp {

TEST_CASE("Units comparisons") {
    REQUIRE(meter<int>(2) == meter<int>(2));
    REQUIRE(meter<int>(2) != meter<int>(3));
    REQUIRE(meter<int>(2) < meter<int>(3));
    REQUIRE(meter<int>(2) <= meter<int>(3));
    REQUIRE(meter<int>(3) > meter<int>(2));
    REQUIRE(meter<int>(3) >= meter<int>(2));
    // REQUIRE(meter<int>(2) == second<int>(2)); // Doesn't compile
}

TEST_CASE("Units additions") {
    auto L1 = meter<double>(2.);
    const auto L2 = meter<double>(3.14);

    L1 += L2;
    REQUIRE(L1.is_approx<2>(meter<double>(5.14)));
    REQUIRE((L1 + L2).is_approx<2>(meter<double>(8.28)));

    // const auto T = second<double>(42.0);
    // L1 += T; // Doesn't compile
}

TEST_CASE("Units substractions") {
    auto L1 = meter<double>(2.);
    const auto L2 = meter<double>(3.14);

    L1 -= L2;
    REQUIRE(L1.is_approx<2>(meter<double>(-1.14)));
    REQUIRE((L1 - L2).is_approx<2>(meter<double>(-4.28)));
}

TEST_CASE("Units multiplications") {
    const auto L = meter<double>(2.);
    const auto T = second<double>(3.14);

    const auto P = L * T;

    static_assert(decltype(P)::unit_flag::num ==
                  unit_flags::Meter::num * unit_flags::Second::num);
    static_assert(decltype(P)::unit_flag::den ==
                  unit_flags::Meter::den * unit_flags::Second::den);

    REQUIRE(P.is_approx<2>(decltype(P)(6.28)));
}

TEST_CASE("Units divisions") {
    const auto L = meter<double>(2.);
    const auto T = second<double>(3.14);

    const auto D = L / T;

    static_assert(decltype(D)::unit_flag::num ==
                  unit_flags::Meter::num * unit_flags::Second::den);
    static_assert(decltype(D)::unit_flag::den ==
                  unit_flags::Meter::den * unit_flags::Second::num);

    REQUIRE(D.is_approx<2>(decltype(D)(2. / 3.14)));
}

TEST_CASE("Units invert") {
    const auto L = meter<double>(2.);

    const auto I = L.invert();

    static_assert(decltype(L)::unit_flag::num == unit_flags::Meter::den);
    static_assert(decltype(L)::unit_flag::den == unit_flags::Meter::num);

    REQUIRE(I.is_approx<2>(decltype(I)(0.5)));
}

} // namespace scicpp