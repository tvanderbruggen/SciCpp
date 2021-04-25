// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// Estimate the zeros of a noisy sinusoid using a polynomial fit

#include <array>
#include <cmath>
#include <scicpp/core.hpp>
#include <scicpp/polynomials.hpp>

namespace sci = scicpp;
using namespace sci::operators;
using namespace sci::units::literals;

int main() {
    const auto x = sci::linspace<100>(-0.1, 2.1) * sci::pi<double>;
    const auto y = sci::sin(x * 1_rad) + 0.1 * sci::random::rand<double, 100>();

    const auto P = sci::polynomial::polyfit<3>(x, y);
    const auto zeros = sci::polynomial::polyroots(P);

    printf("Zeros: Expect 0, pi, 2 pi\n");
    sci::print(zeros);
}