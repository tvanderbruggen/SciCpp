// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "maths.hpp"

namespace scicpp::units {

using namespace literals;

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
}

TEST_CASE("cbrt") {
    REQUIRE(almost_equal(cbrt(8_Hz) + cbrt(8_Hz), 4. * cbrt(1_Hz)));
}

TEST_CASE("root") {
    REQUIRE(almost_equal(sqrt(9_Hz), root<2>(9_Hz)));
    REQUIRE(almost_equal(root<4>(16_Hz) + root<4>(16_Hz), 4. * root<4>(1_Hz)));
}

} // namespace scicpp::units