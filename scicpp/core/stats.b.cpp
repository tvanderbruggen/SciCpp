// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "stats.hpp"

#include "scicpp/core/random.hpp"

// NONIUS_BENCHMARK("Var std::array", [](nonius::chronometer meter) {
//     const auto a = scicpp::random::rand<double, 100>();
//     meter.measure([&a]() { return scicpp::stats::var(a); });
// })

NONIUS_BENCHMARK("Var std::vector", [](nonius::chronometer meter) {
    const auto v = scicpp::random::rand<double>(1000000);
    meter.measure([&v]() { return scicpp::stats::var(v); });
})