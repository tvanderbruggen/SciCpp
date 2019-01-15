// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "random.hpp"

NONIUS_BENCHMARK("random::rand (std::array, 1000)",
                 []() { return scicpp::random::rand<double, 1000>(); })

NONIUS_BENCHMARK("random::rand (std::vector, 1000)",
                 []() { return scicpp::random::rand<double>(1000); })

NONIUS_BENCHMARK("random::randn (std::array, 1000)",
                 []() { return scicpp::random::randn<double, 1000>(); })

NONIUS_BENCHMARK("random::randn (std::vector, 1000)",
                 []() { return scicpp::random::randn<double>(1000); })