// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "waveforms.hpp"

#include "scicpp/core.hpp"

NONIUS_BENCHMARK("signal::sweep_poly (std::vector)",
                 [](nonius::chronometer meter) {
                     const std::array poly{2., 1.25, -0.36, .025};

                     meter.measure([&]() {
                         return scicpp::signal::sweep_poly(
                             scicpp::linspace(0., 10., 1000), poly);
                     });
                 })

NONIUS_BENCHMARK("signal::sweep_poly (std::array)",
                 [](nonius::chronometer meter) {
                     const std::array poly{2., 1.25, -0.36, .025};

                     meter.measure([&]() {
                         return scicpp::signal::sweep_poly(
                             scicpp::linspace<1000>(0., 10.), poly);
                     });
                 })