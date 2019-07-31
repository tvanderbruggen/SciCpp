// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "io.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/units.hpp"

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
    const ConvertersDict converters = {
        {1, [](auto x) { return std::max(std::atof(x), 1.0); }},
        {2, [](auto x) { return std::min(std::atof(x), 10.0); }}};
    REQUIRE(almost_equal(fromstring("42, 0.1, 66", ',', converters),
                         {42., 1., 10.}));
}

TEST_CASE("scicpp::loadtxt to tuple") {
    const auto data = TxtLoader<int, double, double, double, double>()
                          .delimiter(',')
                          .skiprows(1)
                          .load<io::pack>("tests/data0.csv");
    // print(data);
    REQUIRE(data.size() == 38);
    REQUIRE(std::get<0>(data[0]) == 1);
    REQUIRE(std::get<0>(data[24]) == 26);
    REQUIRE(almost_equal(std::get<4>(data[0]), 0.76));
}

TEST_CASE("scicpp::loadtxt to tuple and unpack") {
    const auto [a, b, c] = TxtLoader<int, double, double>()
                               .delimiter(',')
                               .usecols(0, 1, 4)
                               .skiprows(1)
                               .load("tests/data0.csv");
    // print(data);
    REQUIRE(a.size() == 38);
    REQUIRE(a[0] == 1);
    REQUIRE(a[24] == 26);
    REQUIRE(almost_equal(c[0], 0.76));
}

TEST_CASE("scicpp::loadtxt to tuple with converters") {
    const auto data =
        TxtLoader<int, bool, int, double, double>()
            .delimiter(',')
            .skiprows(1)
            .converters({{1, [](auto x) { return std::atof(x) > 25.0; }},
                         {2, [](auto x) { return 10 + std::atoi(x); }}})
            .load<io::pack>("tests/data0.csv");
    // print(data);
    REQUIRE(data.size() == 38);
    REQUIRE(std::get<1>(data[0]) == 0);
    REQUIRE(std::get<1>(data[25]) == 1);
    REQUIRE(std::get<2>(data[25]) == 10);
    REQUIRE(almost_equal(std::get<4>(data[0]), 0.76));
}

TEST_CASE("scicpp::loadtxt to tuple with converters and physical quantities") {
    using namespace units::literals;

    const auto data =
        TxtLoader<int, bool, int, units::length<double>, units::mass<double>>()
            .delimiter(',')
            .skiprows(1)
            .converters({{1, [](auto x) { return std::atof(x) > 25.0; }},
                         {2, [](auto x) { return 10 + std::atoi(x); }}})
            .load<io::pack>("tests/data0.csv");
    // print(data);
    REQUIRE(data.size() == 38);
    REQUIRE(std::get<1>(data[0]) == 0);
    REQUIRE(std::get<1>(data[25]) == 1);
    REQUIRE(std::get<2>(data[25]) == 10);
    REQUIRE(almost_equal(std::get<3>(data[0]), 19.78_m));
    REQUIRE(almost_equal(std::get<4>(data[0]), 0.76_kg));
}

TEST_CASE("scicpp::TxtLoader to Eigen matrix") {
    const auto m1 =
        TxtLoader<double>().delimiter(',').skiprows(1).load("tests/data0.csv");
    // std::cout << m1 << '\n';
    REQUIRE(m1.rows() == 38);
    REQUIRE(m1.cols() == 5);
    REQUIRE(almost_equal(m1(0, 0), 1.0));
    const auto m2 =
        TxtLoader<int>().delimiter(',').skiprows(1).load("tests/data0.csv");
    // std::cout << m2 << '\n';
    REQUIRE(m2.rows() == 38);
    REQUIRE(m2.cols() == 5);
    REQUIRE(m2(0, 0) == 1);
}

TEST_CASE("scicpp::TxtLoader to Eigen matrix with converters") {
    const auto m = TxtLoader<double>()
                       .delimiter(',')
                       .skiprows(1)
                       .converters({{0, [](auto x) { return -std::atof(x); }}})
                       .load("tests/data0.csv");

    REQUIRE(m.size() == 190);
    REQUIRE(almost_equal(m(0), -1.));
    REQUIRE(almost_equal(m(m.size() - 1), 0.02));
}

TEST_CASE("scicpp::TxtLoader to Eigen matrix with usecols and converters") {
    const auto m = TxtLoader<double>()
                       .delimiter(',')
                       .skiprows(1)
                       .usecols({0, 2})
                       .converters({{0, [](auto x) { return -std::atof(x); }}})
                       .load("tests/data0.csv");

    // std::cout << m << '\n';
    REQUIRE(m.size() == 2 * 38);
    REQUIRE(almost_equal(m(0), -1.));
    REQUIRE(almost_equal(m(m.size() - 1), 0.25));
}

TEST_CASE("scicpp::loadtxt to tuple with usecols and converters") {
    const auto data =
        TxtLoader<bool, int, double>()
            .delimiter(',')
            .skiprows(1)
            .usecols({1, 2, 4})
            .converters({{1, [](auto x) { return std::atof(x) > 25.0; }},
                         {2, [](auto x) { return 10 + std::atoi(x); }}})
            .load<io::pack>("tests/data0.csv");
    // print(data);
    REQUIRE(data.size() == 38);
    REQUIRE(std::get<0>(data[0]) == 0);
    REQUIRE(std::get<1>(data[25]) == 10);
    REQUIRE(almost_equal(std::get<2>(data[37]), 0.02));
}

TEST_CASE("scicpp::loadtxt to tuple with usecols, converters and filters") {
    const auto data =
        TxtLoader<bool, int, double>()
            .delimiter(',')
            .skiprows(1)
            .usecols({1, 2, 4})
            .converters({{1, [](auto x) { return std::atof(x) > 25.0; }},
                         {2, [](auto x) { return 10 + std::atoi(x); }}})
            .filters({{1, [](auto x) { return io::cast<bool>(x); }},
                      {2, [](auto x) { return io::cast<int>(x) < 12; }}})
            .load<io::pack>("tests/data0.csv");

    // print(data);
    REQUIRE(data.size() == 26);
    REQUIRE(std::get<0>(data[0]) == 1);
    REQUIRE(std::get<1>(data[25]) == 10);
    REQUIRE(almost_equal(std::get<2>(data[25]), 0.02));
}

TEST_CASE("scicpp::loadtxt to tuple with usecols list and converters") {
    const auto data =
        TxtLoader<bool, int, double>()
            .delimiter(',')
            .skiprows(1)
            .usecols(1, 2, 4)
            .converters({{1, [](auto x) { return std::atof(x) > 25.0; }},
                         {2, [](auto x) { return 10 + std::atoi(x); }}})
            .max_rows(10)
            .load<io::pack>("tests/data0.csv");
    // print(data);
    REQUIRE(data.size() == 10);
    REQUIRE(std::get<0>(data[0]) == 0);
    REQUIRE(std::get<1>(data[9]) == 10);
    REQUIRE(almost_equal(std::get<2>(data[9]), 0.8));
}

} // namespace scicpp