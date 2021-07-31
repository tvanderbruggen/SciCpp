// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "numeric.hpp"

#include "scicpp/core/random.hpp"

// NONIUS_BENCHMARK("Sum std::array", [](nonius::chronometer meter) {
//     const auto a = scicpp::random::rand<double, 100>();
//     meter.measure([&a]() { return scicpp::sum(a); });
// })

NONIUS_BENCHMARK("Sum std::vector", [](nonius::chronometer meter) {
    const auto v = scicpp::random::rand<double>(1000000);
    meter.measure([&v]() { return scicpp::sum(v); });
})