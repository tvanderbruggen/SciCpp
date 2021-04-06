// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "print.hpp"

#include "scicpp/core/range.hpp"
#include "scicpp/signal/windows.hpp"

#include <complex>
#include <sstream>

namespace scicpp {

TEST_CASE("Print a long vector") {
    std::stringstream ss;
    const auto v = linspace(0., 10., 10000000);
    fprint(ss, v);
    REQUIRE(ss.str() == "[ 0, 1e-06, 2e-06, ..., 10, 10, 10 ]\n");
}

TEST_CASE("Print a short array of reals") {
    std::stringstream ss;
    const auto window = signal::windows::hann<double, 16>();
    fprint(ss, window);
    REQUIRE(ss.str() == "[ 0, 0.0432273, 0.165435, 0.345492, 0.552264, \n"
                        "  0.75, 0.904508, 0.989074, 0.989074, 0.904508, \n"
                        "  0.75, 0.552264, 0.345492, 0.165435, 0.0432273, \n"
                        "  0 ]\n");
}

TEST_CASE("Print a short array of complex") {
    using namespace operators;

    std::stringstream ss;
    const auto window = signal::windows::hann<double, 16>();
    const auto phi = linspace<window.size()>(0., 2. * pi<double>);
    const auto window_cplx = window * exp(1.i * phi);
    fprint(ss, window_cplx);
    REQUIRE(
        ss.str() ==
        "[ 0 + 0i, 0.0394901 + 0.0175821i, 0.110697 + 0.122942i, 0.106763 "
        "+ 0.328582i, -0.0577273 + 0.549239i, \n"
        "  -0.375 + 0.649519i, -0.731763 + 0.531657i, -0.96746 + 0.20564i, "
        "-0.96746 - 0.20564i, -0.731763 - 0.531657i, \n"
        "  -0.375 - 0.649519i, -0.0577273 - 0.549239i, 0.106763 - 0.328582i, "
        "0.110697 - 0.122942i, 0.0394901 - 0.0175821i, \n"
        "  0 - 0i ]\n");
}

TEST_CASE("Print a short array of units") {
    using namespace units::literals;

    std::stringstream ss;
    fprint(ss, std::array{1._m, 2._m, 3._m, 4._m});
    REQUIRE(ss.str() == "[ 1, 2, 3, 4 ]\n");
}

TEST_CASE("Print a short array of tuples") {
    std::array<std::tuple<int, bool, double>, 10> atup{};

    for (size_t i = 0; i < atup.size(); ++i) {
        atup[i] = {int(i), i % 2, 3.14 * double(i * i)};
    }

    std::stringstream ss;
    fprint(ss, atup);
    REQUIRE(ss.str() == "[ (0, 0, 0), \n"
                        "  (1, 1, 3.14), \n"
                        "  (2, 0, 12.56), \n"
                        "  (3, 1, 28.26), \n"
                        "  (4, 0, 50.24), \n"
                        "  (5, 1, 78.5), \n"
                        "  (6, 0, 113.04), \n"
                        "  (7, 1, 153.86), \n"
                        "  (8, 0, 200.96), \n"
                        "  (9, 1, 254.34) ]\n");
}

TEST_CASE("Print a long vector of tuples") {
    std::vector<std::tuple<int, bool, double>> vtup(10000000);

    for (size_t i = 0; i < vtup.size(); ++i) {
        vtup[i] = {int(i), i % 2, 3.14 * double(i * i)};
    }

    std::stringstream ss;
    fprint(ss, vtup);
    REQUIRE(ss.str() ==
            "[ (0, 0, 0), (1, 1, 3.14), (2, 0, 12.56), ..., (9999997, 1, "
            "3.14e+14), (9999998, 0, 3.14e+14), (9999999, 1, 3.14e+14) ]\n");
}

TEST_CASE("Print complex") {
    std::stringstream ss;
    fprint(ss, 1. + 2.i);
    REQUIRE(ss.str() == "1 + 2i\n");
}

TEST_CASE("Print quantity") {
    using namespace units::literals;

    std::stringstream ss;
    fprint(ss, 1_m);
    REQUIRE(ss.str() == "1\n");
}

TEST_CASE("Print complex quantity") {
    using namespace units::literals;

    std::stringstream ss;
    fprint(ss, std::complex(1_m, 2_m));
    REQUIRE(ss.str() == "1 + 2i\n");
}

} // namespace scicpp