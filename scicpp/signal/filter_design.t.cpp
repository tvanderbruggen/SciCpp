// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "filter_design.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp::signal {

TEST_CASE("unique_roots") {
    SECTION("vector") {
        std::vector x{0.0, 1.3, 1.31, 2.8, 1.25, 2.2, 10.3};
        const auto [uniq, mult] = unique_roots<AVG>(x, 2E-2);
        // print(uniq);
        // print(mult);
        REQUIRE(almost_equal<2>(uniq, {0., 1.25, 1.305, 2.2, 2.8, 10.3}));
        REQUIRE(array_equal(mult, {1, 1, 2, 1, 1, 1}));
    }

    SECTION("array") {
        std::array x{0.0, 1.3, 1.31, 2.8, 1.25, 2.2, 10.3};
        const auto [uniq, mult] = unique_roots<AVG>(x, 2E-2);
        // print(uniq);
        // print(mult);
        REQUIRE(almost_equal<2>(uniq, {0., 1.25, 1.305, 2.2, 2.8, 10.3}));
        REQUIRE(array_equal(mult, {1, 1, 2, 1, 1, 1}));
    }
}

} // namespace scicpp::signal