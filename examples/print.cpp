// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <array>
#include <complex>
#include <scicpp/core.hpp>
#include <scicpp/signal.hpp>
#include <tuple>
#include <vector>

namespace sci = scicpp;
using namespace sci::operators;
using namespace sci::units::literals;
using namespace std::literals;

int main() {
    // ---- Print a long vector
    sci::print(sci::linspace(0., 10., 10000000));
    sci::print(sci::linspace(1., 10000000., 10000000));

    // ---- Print a short array of reals
    sci::print(sci::linspace(1.0, 456.156, 100));
    sci::print(sci::linspace(1.0, 456156.0, 100));

    const auto window = sci::signal::windows::hann<double, 16>();
    sci::print(window);

    // ---- Print a short array of complex
    const auto phi = sci::linspace<window.size()>(0., 2. * M_PI);
    sci::print(sci::exp(1.i * phi));
    const auto window_cplx = window * sci::exp(1.i * phi);
    sci::print(window_cplx);

    // ---- Print a short array of units
    sci::print(std::array{1._m, 2._m, 3._m, 4._m});

    // ---- Print a short array of tuples
    std::array<std::tuple<int, bool, double>, 10> atup{};

    for (size_t i = 0; i < atup.size(); ++i) {
        atup[i] = {int(i), i % 2, 3.14 * double(i * i)};
    }

    sci::print(atup);

    // ---- Print a long vector of tuples
    std::vector<std::tuple<int, bool, double>> vtup(10000000);

    for (size_t i = 0; i < vtup.size(); ++i) {
        vtup[i] = {int(i), i % 2, 3.14 * double(i * i)};
    }

    sci::print(vtup);

    // ---- Print a complex
    sci::print(1. + 2.i);
}