// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "print.hpp"

#include "scicpp/core/range.hpp"
#include "scicpp/signal/windows.hpp"

#include <complex>
#include <sstream>

namespace scicpp {

TEST_CASE("Print empty array") {
    std::stringstream ss;
    const std::array<int, 0> v{};
    print(ss, v);
    REQUIRE(ss.str() == "[]\n");
}

TEST_CASE("Print empty vector") {
    std::stringstream ss;
    const std::vector<double> v(0);
    print(ss, v);
    REQUIRE(ss.str() == "[]\n");
}

TEST_CASE("Print nan") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    const std::array<double, 3> v{1.0, 2.0, nan};

    std::stringstream ss1;
    print(ss1, v);
    REQUIRE(ss1.str() == "[  1.           2.                  nan]\n");

    std::stringstream ss2;
    PrintOptions prtopts{};
    prtopts.nanstr = "NaN";
    print(ss2, v, prtopts);
    REQUIRE(ss2.str() == "[  1.           2.                  NaN]\n");
}

TEST_CASE("Print inf") {
    constexpr auto inf = std::numeric_limits<double>::infinity();
    const std::array<double, 3> v{1.0, 2.0, inf};

    std::stringstream ss1;
    print(ss1, v);
    REQUIRE(ss1.str() == "[  1.           2.                  inf]\n");

    std::stringstream ss2;
    PrintOptions prtopts{};
    prtopts.infstr = "Inf";
    print(ss2, v, prtopts);
    REQUIRE(ss2.str() == "[  1.           2.                  Inf]\n");
}

TEST_CASE("Print -inf") {
    constexpr auto inf = std::numeric_limits<double>::infinity();
    const std::array<double, 3> v{1.0, 2.0, -inf};

    std::stringstream ss1;
    print(ss1, v);
    REQUIRE(ss1.str() == "[  1.           2.                    -inf]\n");

    std::stringstream ss2;
    PrintOptions prtopts{};
    prtopts.infstr = "Inf";
    print(ss2, v, prtopts);
    REQUIRE(ss2.str() == "[  1.           2.                    -Inf]\n");
}

TEST_CASE("Print complex inf, -inf") {
    constexpr auto inf = std::numeric_limits<double>::infinity();
    const std::array<std::complex<double>, 3> v{
        inf + 1.0i, 2.0 + 3.0i, std::complex(3.0, -inf)};

    std::stringstream ss1;
    print(ss1, v);
    REQUIRE(ss1.str() == "[         inf+1.00000000j   2.00000000+3.00000000j  "
                         " 3.00000000-infj]\n");

    std::stringstream ss2;
    PrintOptions prtopts{};
    prtopts.infstr = "Inf";
    print(ss2, v, prtopts);
    REQUIRE(ss2.str() == "[         Inf+1.00000000j   2.00000000+3.00000000j  "
                         " 3.00000000-Infj]\n");
}

TEST_CASE("Print a long vector") {
    std::stringstream ss;
    const auto v = linspace(0., 10., 10000000);
    print(ss, v);
    REQUIRE(
        ss.str() ==
        "[0.00000000e+00 1.00000010e-06 2.00000020e-06 ... 9.99999800e+00 \n"
        " 9.99999900e+00 1.00000000e+01]\n");
}

TEST_CASE("Print a short array of reals") {
    std::stringstream ss;
    const auto window = signal::windows::hann<double, 16>();
    print(ss, window);
    REQUIRE(
        ss.str() ==
        "[  0.           0.04322727   0.1654347    0.3454915    0.55226423 \n"
        "   0.75         0.9045085    0.9890738    0.9890738    0.9045085  \n"
        "   0.75         0.55226423   0.3454915    0.1654347    0.04322727 \n"
        "   0.        ]\n");
}

TEST_CASE("Print a short array of complex") {
    using namespace operators;

    std::stringstream ss;
    const auto window = signal::windows::hann<double, 16>();
    const auto phi = linspace<window.size()>(0., 2. * pi<double>);
    const auto window_cplx = window * exp(1.i * phi);
    print(ss, window_cplx);
    REQUIRE(ss.str() ==
            "[  0.00000000+0.00000000j   0.03949008+0.01758212j \n"
            "   0.11069742+0.12294194j   0.10676275+0.32858195j \n"
            "  -0.05772733+0.54923887j  -0.37500000+0.64951905j \n"
            "  -0.73176275+0.53165676j  -0.96746016+0.20564001j \n"
            "  -0.96746016-0.20564001j  -0.73176275-0.53165676j \n"
            "  -0.37500000-0.64951905j  -0.05772733-0.54923887j \n"
            "   0.10676275-0.32858195j   0.11069742-0.12294194j \n"
            "   0.03949008-0.01758212j   0.00000000-0.00000000j]\n");
}

TEST_CASE("Print a short array of units") {
    using namespace units::literals;

    std::stringstream ss;
    print(ss, std::array{1._m, 2._m, 3._m, 4._m});
    REQUIRE(ss.str() ==
            "[  1.           2.           3.           4.        ]\n");
}

TEST_CASE("Print a short array of tuples") {
    std::array<std::tuple<int, bool, double>, 10> atup{};

    for (size_t i = 0; i < atup.size(); ++i) {
        atup[i] = {int(i), i % 2, 3.14 * double(i * i)};
    }

    std::stringstream ss;
    print(ss, atup);
    REQUIRE(ss.str() == "[(0, 0, 0.00000000e+00) (1, 1, 3.14000000e+00) (2, 0, "
                        "1.25600000e+01) \n"
                        " (3, 1, 2.82600000e+01) (4, 0, 5.02400000e+01) (5, 1, "
                        "7.85000000e+01) \n"
                        " (6, 0, 1.13040000e+02) (7, 1, 1.53860000e+02) (8, 0, "
                        "2.00960000e+02) \n"
                        " (9, 1, 2.54340000e+02)]\n");
}

TEST_CASE("Print a long vector of tuples") {
    std::vector<std::tuple<int, bool, double>> vtup(10000000);

    for (size_t i = 0; i < vtup.size(); ++i) {
        vtup[i] = {int(i), i % 2, 3.14 * double(i * i)};
    }

    std::stringstream ss;
    print(ss, vtup);
    REQUIRE(ss.str() ==
            "[(0, 0, 0.00000000e+00) (1, 1, 3.14000000e+00) (2, 0, "
            "1.25600000e+01) ... \n"
            " (9999997, 1, 3.13999812e+14) (9999998, 0, 3.13999874e+14) \n"
            " (9999999, 1, 3.13999937e+14)]\n");
}

TEST_CASE("Print complex") {
    std::stringstream ss;
    print(ss, 1. + 2.i);
    REQUIRE(ss.str() == "1.00000000e+00+2.00000000e+00j\n");
}

TEST_CASE("Print quantity") {
    using namespace units::literals;

    std::stringstream ss;
    print(ss, 1_m);
    REQUIRE(ss.str() == "1.00000000e+00\n");
}

TEST_CASE("Print complex quantity") {
    using namespace units::literals;

    std::stringstream ss;
    print(ss, std::complex(1_m, 2_m));
    REQUIRE(ss.str() == "1.00000000e+00+2.00000000e+00j\n");
}

} // namespace scicpp