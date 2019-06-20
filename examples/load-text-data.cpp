// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdio>
#include <scicpp/core.hpp>

namespace sci = scicpp;

int main() {
    // const auto data =
    //     TxtLoader<int, bool, int, double, double>()
    //         .delimiter(',')
    //         .skiprows(1)
    //         .converters({{1, [](auto x) { return std::atof(x) > 25.0; }},
    //                      {2, [](auto x) { return 10 + std::atoi(x); }}})
    //         .load("examples/data1.csv");
}
