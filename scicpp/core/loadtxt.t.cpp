// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "scicpp/core/loadtxt.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("scicpp::to_number") {
    REQUIRE(almost_equal(to_number<double>("42"), 42.0));
    REQUIRE(almost_equal(to_number<int>("42"), 42));
}

TEST_CASE("scicpp::loadtxt") {
    auto [data, num_cols] = loadtxt("tests/data0.csv", ',', 1);
    print(data);
    REQUIRE(true);
}

} // namespace scicpp