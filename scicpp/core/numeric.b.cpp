// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "numeric.hpp"

#include "scicpp/random.hpp"

#include <algorithm>

NONIUS_BENCHMARK("Sum std::array", [](nonius::chronometer meter) {
    auto a = scicpp::random::rand<double, 100>();

    meter.measure([&a]() { return scicpp::sum(a); });
})

NONIUS_BENCHMARK("Sum std::vector", [](nonius::chronometer meter) {
    std::vector<double> v(100);
    std::fill(v.begin(), v.end(), 10.0);

    meter.measure([&v]() { return scicpp::sum(v); });
})