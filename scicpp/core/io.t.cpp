// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "scicpp/core/io.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("scicpp::to_number") {
    REQUIRE(almost_equal(to_number<double>("42"), 42.0));
    REQUIRE(to_number<int>("42") == 42);
}

TEST_CASE("scicpp::loadtxt to vector") {
    const auto [data, num_cols] =
        loadtxt<std::vector<double>>("tests/data0.csv", ',', 1);
    print(data);
    REQUIRE(true);
}

TEST_CASE("scicpp::loadtxt to Eigen matrix") {
    const auto m1 = loadtxt("tests/data0.csv", ',', 1);
    std::cout << m1 << '\n';
    const auto m2 = loadtxt<int>("tests/data0.csv", ',', 1);
    std::cout << m2 << '\n';
    REQUIRE(true);
}

} // namespace scicpp