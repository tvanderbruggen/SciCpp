// SPDX-License-Identifier: MIT
// Copyright (c) 2020 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "interpolate.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units/quantity.hpp"

namespace scicpp::interpolate {

TEST_CASE("interp1d - SLINEAR") {
    SECTION("std::vector") {
        using namespace operators;
        const auto x = arange(0.0, 5.0);
        const auto f = interp1d(x, exp(-x));
        const auto xnew = arange(0.0, 4.0, 2.0);
        REQUIRE(almost_equal(f(xnew), {1.0, 0.1353352832366127}));
    }

    SECTION("std::array") {
        const std::array x = {1.0, 2.0, 3.0, 4.0, 5.0};
        const auto f = interp1d(x, log(x));
        REQUIRE(almost_equal(f(std::array{1.5, 2.5}),
                             {0.34657359027997264, 0.8958797346140275}));
    }

    SECTION("std::array units in input") {
        using namespace units::literals;
        const std::array x = {0_rad, 1_rad, 2_rad, 3_rad, 4_rad, 5_rad};
        const auto f = interp1d(x, sin(x));
        REQUIRE(
            almost_equal(f(std::array{0.0, 2.0}), {0.0, 0.9092974268256817}));
    }
}

// /!\ Doesn't work
// TEST_CASE("interp1d - ZERO") {
//     SECTION("std::vector") {
//         using namespace operators;
//         const auto x = arange(0.0, 5.0);
//         const auto f = interp1d<InterpKind::ZERO>(x, exp(-x));
//         const auto xnew = arange(3.0, 4.0, 0.25);
//         const auto xres = 0.049787068367863944;
//         REQUIRE(almost_equal(f(xnew), {xres, xres, xres, xres}));
//     }
// }

TEST_CASE("interp1d - QUADRATIC") {
    SECTION("std::vector") {
        using namespace operators;
        const auto x = arange(0.0, 5.0, 0.5);
        const auto f = interp1d<InterpKind::QUADRATIC>(x, exp(-x));
        const auto xnew = arange(3.0, 4.0, 0.25);
        REQUIRE(almost_equal<4>(f(xnew),
                                {0.04978706836786395,
                                 0.03875345918927947,
                                 0.030197383422318504,
                                 0.023519998644726408}));
    }
}

TEST_CASE("interp1d - CUBIC") {
    SECTION("std::vector") {
        const auto x = arange(0.0, 5.0, 0.5);
        const auto f = interp1d<InterpKind::CUBIC>(x, sqrt(x));
        const auto xnew = arange(3.0, 4.0, 0.25);
        REQUIRE(almost_equal<4>(f(xnew),
                                {1.7320508075688772,
                                 1.8027594680495171,
                                 1.8708286933869707,
                                 1.9365023866974787}));
    }
}

} // namespace scicpp::interpolate