// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "print.hpp"

#include "scicpp/signal/windows.hpp"

#include <sstream>

NONIUS_BENCHMARK("Print an array of 500 pts to std::stringstream",
                 [](nonius::chronometer meter) {
                     const auto window =
                         scicpp::signal::windows::hann<double, 500>();
                     std::ostringstream ss;

                     meter.measure([&] { scicpp::print(ss, window); });
                 })

NONIUS_BENCHMARK("Print an array of 1000 pts to std::stringstream",
                 [](nonius::chronometer meter) {
                     const auto window =
                         scicpp::signal::windows::hann<double, 1000>();
                     std::ostringstream ss;

                     meter.measure([&] { scicpp::print(ss, window); });
                 })

NONIUS_BENCHMARK("Print an array of 2000 pts to std::stringstream",
                 [](nonius::chronometer meter) {
                     const auto window =
                         scicpp::signal::windows::hann<double, 2000>();
                     std::ostringstream ss;

                     meter.measure([&] { scicpp::print(ss, window); });
                 })