// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "histogram.hpp"

#include "scicpp/core/print.hpp"
#include "scicpp/core/units/units.hpp"

#include <array>

namespace scicpp::stats {

TEST_CASE("histogram_bin_edges") {
    const auto arr = std::array{0., 0., 0., 1., 2., 3., 3., 4., 5.};
    // print(histogram_bin_edges<SQRT>(arr));

    REQUIRE(almost_equal<2>(histogram_bin_edges<SQRT>(arr),
                            {0., 1. + 2. / 3., 10. / 3., 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<SCOTT>(arr), {0., 2.5, 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<RICE>(arr),
                            {0., 1., 2., 3., 4., 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<STURGES>(arr),
                            {0., 1., 2., 3., 4., 5.}));
}

TEST_CASE("histogram_bin_edges physical quantities") {
    using namespace units::literals;
    const auto arr = std::array{0_m, 0_m, 1_m, 2_m, 3_m, 3_m, 4_m, 5_m};
    // print(histogram_bin_edges<SQRT>(arr));

    REQUIRE(almost_equal<2>(histogram_bin_edges<SQRT>(arr),
                            {0_m, 1_m + 2_m / 3., 10_m / 3., 5_m}));
    REQUIRE(
        almost_equal<2>(histogram_bin_edges<SCOTT>(arr), {0_m, 2.5_m, 5_m}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<RICE>(arr),
                            {0_m, 1.25_m, 2.5_m, 3.75_m, 5_m}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<STURGES>(arr),
                            {0_m, 1.25_m, 2.5_m, 3.75_m, 5_m}));
}

} // namespace scicpp::stats