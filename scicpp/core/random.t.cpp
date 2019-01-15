// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "random.hpp"

#include "scicpp/core/stats.hpp"

namespace scicpp {
namespace random {

TEST_CASE("rand") {
    SECTION("std::array") {
        auto r = rand<double, 1000>();
        REQUIRE(r.size() == 1000);
        REQUIRE_FLOAT_EQUAL(stats::mean(r), 0.5010793723598663);
        REQUIRE_FLOAT_EQUAL(stats::std(r),
                            0.2929667023696285); // Theory 1/sqrt(12)
    }

    SECTION("std::vector") {
        auto r = rand<double>(1000);
        REQUIRE(r.size() == 1000);
        REQUIRE_FLOAT_EQUAL(stats::mean(r), 0.5010793723598663);
        REQUIRE_FLOAT_EQUAL(stats::std(r),
                            0.2929667023696285); // Theory 1/sqrt(12)
    }
}

TEST_CASE("randn") {
    SECTION("std::array") {
        auto r = randn<double, 1000>();
        REQUIRE(r.size() == 1000);
        REQUIRE(std::fabs(stats::mean(r)) < 0.01);
        REQUIRE(std::fabs(stats::std(r) - 1.0) < 0.01);
    }

    SECTION("std::vector") {
        auto r = randn<double>(1000);
        REQUIRE(r.size() == 1000);
        REQUIRE(std::fabs(stats::mean(r)) < 0.01);
        REQUIRE(std::fabs(stats::std(r) - 1.0) < 0.01);
    }
}

} // namespace random
} // namespace scicpp