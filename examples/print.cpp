// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <array>
#include <scicpp/core.hpp>
#include <scicpp/signal.hpp>
#include <tuple>
#include <vector>

namespace sci = scicpp;
using namespace sci::operators;
using namespace std::literals;

int main() {
    // Print a long vector
    const auto v = sci::linspace(0., 10., 100000000);
    sci::print(v);

    // Print a short array of reals
    constexpr std::array HFT90D{1., 1.942604, 1.340318, 0.440811, 0.043097};
    const auto window =
        sci::signal::windows::general_cosine<double, 16>(HFT90D);
    sci::print(window);

    // Print a short array of complex
    const auto window_cplx =
        window * sci::exp(1.i * sci::linspace<window.size()>(0., 2. * M_PI));
    sci::print(window_cplx);

    // Print a short array of tuples
    std::array<std::tuple<int, bool, double>, 10> atup{};

    for (size_t i = 0; i < atup.size(); ++i) {
        atup[i] = std::make_tuple(int(i), i % 2, 3.14 * i * i);
    }

    sci::print(atup);

    // Print a long vector of tuples
    std::vector<std::tuple<int, bool, double>> vtup(10000000);

    for (size_t i = 0; i < vtup.size(); ++i) {
        vtup[i] = std::make_tuple(int(i), i % 2, 3.14 * i * i);
    }

    sci::print(vtup);
}