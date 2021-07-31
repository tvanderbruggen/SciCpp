// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdio>
#include <scicpp/core.hpp>

int main() {
    const std::array a{-1., 1., 2., 3.};

    // Compute the mean of the array
    auto m = scicpp::stats::median(a);

    // Compute the mean of the positive values of the array
    auto m_pos = scicpp::stats::median(a, [](auto x) { return x >= 0; });

    printf("m = %f, m_pos = %f\n", m, m_pos);
}
