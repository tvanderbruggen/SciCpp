// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdlib>
#include <scicpp/core.hpp>

int main() {
    const auto v1 = scicpp::fromstring("42, 89, 66", ',');
    scicpp::print(v1); // [42.0, 89.0, 66.0]

    const auto v2 = scicpp::fromstring("42 89 66", ' ');
    scicpp::print(v2); // [42.0, 89.0, 66.0]

    const auto v3 = scicpp::fromstring<int>(
        "42 89 66",
        ' ',
        {{0, [](auto x) { return -std::atoi(x); }},
         {1, [](auto x) { return 10 - std::atoi(x); }}});
    scicpp::print(v3); // [-42, -79, 66]
}