// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdio>
#include <scicpp/core.hpp>

namespace sci = scicpp;

int main() {
    // Fixed size (std::array)
    const auto a = sci::linspace<100>(0., 10.);
    sci::print(a);
    printf("Mean: %f\n", sci::stats::mean(a));
    printf("Var: %f\n\n", sci::stats::var(a));

    // Dynamic size (std::vector)
    const auto v = sci::linspace(0., 10., 100000000);
    sci::print(v);
    printf("Mean: %f\n", sci::stats::mean(v));
    printf("Var: %f\n\n", sci::stats::var(v));
}
