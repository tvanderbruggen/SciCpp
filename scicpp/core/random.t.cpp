// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "random.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/stats.hpp"

namespace scicpp::random {

TEST_CASE("rand") {
    SECTION("std::array") {
        const auto r = rand<double, 1000>();
        printf("%f %f\n", stats::std(r), stats::var(r));
        REQUIRE(r.size() == 1000);
        REQUIRE(std::fabs(stats::mean(r) - 0.5) < 1. / std::sqrt(1000));
        REQUIRE(std::fabs(stats::std(r) - 1. / std::sqrt(12)) <
                1. / std::sqrt(1000));
    }

    SECTION("std::vector") {
        const auto r = rand<double>(1000);
        REQUIRE(r.size() == 1000);
        REQUIRE(std::fabs(stats::mean(r) - 0.5) < 1. / std::sqrt(1000));
        REQUIRE(std::fabs(stats::std(r) - 1. / std::sqrt(12)) <
                1. / std::sqrt(1000));
    }
}

TEST_CASE("randn") {
    SECTION("std::array") {
        const auto r = randn<double, 1000>();
        REQUIRE(r.size() == 1000);
        REQUIRE(std::fabs(stats::mean(r)) < 0.01);
        REQUIRE(std::fabs(stats::std(r) - 1.0) < 0.01);
    }

    SECTION("std::vector") {
        const auto r = randn<double>(1000);
        REQUIRE(r.size() == 1000);
        REQUIRE(std::fabs(stats::mean(r)) < 0.01);
        REQUIRE(std::fabs(stats::std(r) - 1.0) < 0.01);
    }
}

TEST_CASE("normal") {
    SECTION("std::array") {
        const auto mu = 0.0;
        const auto sigma = 0.1;
        const auto s = normal<1000>(mu, sigma);
        REQUIRE(s.size() == 1000);
        REQUIRE(std::fabs(stats::mean(s) - mu) < 0.01);
        REQUIRE(std::fabs(stats::std(s) - sigma) < 0.01);
    }

    SECTION("std::vector") {
        const auto mu = 0.0;
        const auto sigma = 0.1;
        const auto s = normal(mu, sigma, 1000);
        REQUIRE(s.size() == 1000);
        REQUIRE(std::fabs(stats::mean(s) - mu) < 0.01);
        REQUIRE(std::fabs(stats::std(s) - sigma) < 0.01);
    }

    SECTION("Physical units") {
        using namespace units::literals;

        const auto mu = 0.0_V;
        const auto sigma = 0.1_V;

        const auto s = normal(mu, sigma, 1000);
        REQUIRE(s.size() == 1000);
        REQUIRE(units::fabs(stats::mean(s) - mu) < 0.01_V);
        REQUIRE(units::fabs(stats::std(s) - sigma) < 0.01_V);
    }
}

} // namespace scicpp::random