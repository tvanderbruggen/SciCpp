// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "scicpp/core/io.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("scicpp::detail::to_number") {
    REQUIRE(almost_equal(detail::to_number<double>("42"), 42.0));
    REQUIRE(detail::to_number<int>("42") == 42);
}

TEST_CASE("scicpp::fromstring") {
    REQUIRE(almost_equal(fromstring("42, 89, 66", ','), {42., 89., 66.}));
    REQUIRE(almost_equal(fromstring("42 89 66", ' '), {42., 89., 66.}));
    REQUIRE(almost_equal(fromstring(std::string("42 89 66"), ' '),
                         {42., 89., 66.}));
}

TEST_CASE("scicpp::loadtxt to vector") {
    const auto [data, num_cols] =
        loadtxt<std::vector<double>>("tests/data0.csv", ',', 1);
    // print(data);
    REQUIRE(data.size() == 195);
    REQUIRE(almost_equal(data[0], 1.));
    REQUIRE(almost_equal(data[data.size() - 1], 0.02));
}

TEST_CASE("scicpp::loadtxt to Eigen matrix") {
    const auto m1 = loadtxt("tests/data0.csv", ',', 1);
    // std::cout << m1 << '\n';
    const auto m2 = loadtxt<int>("tests/data0.csv", ',', 1);
    // std::cout << m2 << '\n';
    REQUIRE(true);
}

} // namespace scicpp