// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "io.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/units/units.hpp"
#include "scicpp/linalg/utils.hpp"

#include <iostream>
#include <tuple>

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

TEST_CASE("scicpp::savetxt single std::array") {
    const auto arr = std::array{
        0., 3.14, 12.56, 28.26, 50.24, 78.5, 113.04, 153.86, 200.96, 254.34};
    const auto fname = "tmp/tests/test.csv";
    savetxt(fname, arr, ',', '\n');
    const auto data = linalg::to_std_container(TxtLoader<double>().load(fname));
    REQUIRE(almost_equal(data, std::vector(arr.cbegin(), arr.cend())));
}

TEST_CASE("scicpp::savetxt single std::vector") {
    const auto vec = std::vector{
        0., 3.14, 12.56, 28.26, 50.24, 78.5, 113.04, 153.86, 200.96, 254.34};
    const auto fname = "tmp/tests/test.csv";
    savetxt(fname, vec, ',', '\n');
    const auto data = linalg::to_std_container(TxtLoader<double>().load(fname));
    REQUIRE(almost_equal(data, vec));
}

TEST_CASE("scicpp::savetxt tuple std::vector") {
    std::tuple<std::vector<int>, std::vector<bool>, std::vector<double>> t;

    for (size_t i = 0; i < 10; ++i) {
        std::get<0>(t).push_back(int(i));
        std::get<1>(t).push_back(i % 2);
        std::get<2>(t).push_back(3.14 * double(i * i));
    }

    const auto fname = "tmp/tests/test.csv";
    savetxt(fname, t, ',', '\n');
    const auto [v0, v1, v2] =
        TxtLoader<int, bool, double>().delimiter(',').load(fname);
    REQUIRE(almost_equal(std::get<0>(t), v0));
    REQUIRE(almost_equal(std::get<1>(t), v1));
    REQUIRE(almost_equal(std::get<2>(t), v2));
}

template <typename Array>
auto to_vector(const Array &arr) {
    return std::vector(arr.cbegin(), arr.cend());
}

TEST_CASE("scicpp::savetxt tuple std::array") {
    std::
        tuple<std::array<int, 10>, std::array<bool, 10>, std::array<double, 10>>
            t;

    for (size_t i = 0; i < 10; ++i) {
        std::get<0>(t)[i] = int(i);
        std::get<1>(t)[i] = i % 2;
        std::get<2>(t)[i] = 3.14 * double(i * i);
    }

    const auto fname = "tmp/tests/test.csv";
    savetxt(fname, t, ' ', '\n');
    const auto [v0, v1, v2] =
        TxtLoader<int, bool, double>().delimiter(' ').load(fname);
    REQUIRE(to_vector(std::get<0>(t)) == v0);
    REQUIRE(to_vector(std::get<1>(t)) == v1);
    REQUIRE(almost_equal(to_vector(std::get<2>(t)), v2));
}

TEST_CASE("scicpp::TxtSaver tuple std::array") {
    using namespace operators;

    std::tuple<std::array<int, 10>,
               std::array<bool, 10>,
               std::array<double, 10>,
               std::array<const char *, 10>,
               std::array<std::complex<double>, 10>>
        t;

    for (size_t i = 0; i < 10; ++i) {
        std::get<0>(t)[i] = int(i);
        std::get<1>(t)[i] = i % 2;
        std::get<2>(t)[i] = 3.14 * double(i * i);
        std::get<3>(t)[i] = (i % 2) ? "Yes" : "No";
        std::get<4>(t)[i] = std::complex(
            3.14 * double(i * i), 2.718 * double(i) * ((i % 2) ? 1.0 : -1.0));
    }

    const auto fname = "tmp/tests/test.csv";
    TxtSaver().delimiter(' ').newline('\n').save(fname, t);
    const auto [v0, v1, v2, v3, v4] =
        TxtLoader<int, bool, double, std::string, std::complex<double>>()
            .delimiter(' ')
            .load(fname);
    REQUIRE(to_vector(std::get<0>(t)) == v0);
    REQUIRE(to_vector(std::get<1>(t)) == v1);
    REQUIRE(almost_equal(to_vector(std::get<2>(t)), v2));
    REQUIRE(std::vector<std::string>(std::get<3>(t).cbegin(),
                                     std::get<3>(t).cend()) == v3);
    // print(v4);
    REQUIRE(to_vector(std::get<4>(t)) == v4);
}

} // namespace scicpp