// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdio>
#include <scicpp/core.hpp>

int main() {
    const std::array a{-1., 1., 2., 3.};

    // Compute the variance of the array
    const auto v = scicpp::stats::var(a);

    // Compute the variance of the positive values of the array
    const auto v_pos = scicpp::stats::var(a, [](auto x) { return x >= 0; });

    printf("v = %f, v_pos = %f\n", v, v_pos);
}
