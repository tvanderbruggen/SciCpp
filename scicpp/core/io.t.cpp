// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "scicpp/core/io.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

#include <iostream>

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

TEST_CASE("scicpp::fromstring with converters") {
    const ConvertersDict<double> converters = {
        {1, [](auto x) { return std::max(std::atof(x), 1.0); }},
        {2, [](auto x) { return std::min(std::atof(x), 10.0); }}};
    REQUIRE(almost_equal(fromstring("42, 0.1, 66", ',', converters),
                         {42., 1., 10.}));
}

// TEST_CASE("scicpp::TxtLoader to vector") {
//     const auto [data, num_cols] =
//         TxtLoader<std::vector<double>>().delimiter(',').skiprows(1).load(
//             "tests/data0.csv");
//     // print(data);
//     REQUIRE(data.size() == 195);
//     REQUIRE(almost_equal(data[0], 1.));
//     REQUIRE(almost_equal(data[data.size() - 1], 0.02));
// }

TEST_CASE("scicpp::loadtxt to tuple") {
    // detail::fprint_element(stdout, std::complex<double>(1., -2.));
    // std::vector<std::tuple<int, double>> v = {std::make_tuple(1, 3.14),
    //                                           std::make_tuple(0, 4.78)};
    // print(v);
    const auto data = TxtLoader<int, double, double, double, double>()
                          .delimiter(',')
                          .skiprows(1)
                          .load("tests/data0.csv");
    print(data);
    REQUIRE(true);
}

// TEST_CASE("scicpp::loadtxt to vector with converters") {
//     const ConvertersDict<double> converters = {
//         {0, [](auto x) { return -std::atof(x); }}};
//     const auto [data, num_cols] =
//         loadtxt<std::vector<double>>("tests/data0.csv", ',', 1, converters);
//     // print(data);
//     REQUIRE(data.size() == 195);
//     REQUIRE(almost_equal(data[0], -1.));
//     REQUIRE(almost_equal(data[data.size() - 1], 0.02));
// }

TEST_CASE("scicpp::TxtLoader to Eigen matrix") {
    const auto m1 =
        TxtLoader<double>().delimiter(',').skiprows(1).load("tests/data0.csv");
    ;
    // std::cout << m1 << '\n';
    REQUIRE(m1.rows() == 39);
    REQUIRE(m1.cols() == 5);
    REQUIRE(almost_equal(m1(0, 0), 1.0));
    const auto m2 =
        TxtLoader<int>().delimiter(',').skiprows(1).load("tests/data0.csv");
    ;
    // std::cout << m2 << '\n';
    REQUIRE(m2.rows() == 39);
    REQUIRE(m2.cols() == 5);
    REQUIRE(m2(0, 0) == 1);
}

} // namespace scicpp