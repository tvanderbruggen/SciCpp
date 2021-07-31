// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "fft.hpp"

#include "scicpp/random.hpp"
#include "scicpp/signal/windows.hpp"

//---------------------------------------------------------------------------------
// FFTs
//---------------------------------------------------------------------------------

NONIUS_BENCHMARK("signal::rfft (1000)", [](nonius::chronometer meter) {
    const auto v = scicpp::random::rand<double>(1000);

    meter.measure([&v]() { return scicpp::signal::rfft(v); });
})

NONIUS_BENCHMARK("signal::ifft (1000)", [](nonius::chronometer meter) {
    const auto v = scicpp::random::rand<double>(1000);

    meter.measure([&v]() { return scicpp::signal::ifft(v); });
})

//---------------------------------------------------------------------------------
// Power spectrum
//---------------------------------------------------------------------------------

NONIUS_BENCHMARK("signal::power_spectrum_density (1000)",
                 [](nonius::chronometer meter) {
                     const auto v = scicpp::random::rand<double>(1000);

                     meter.measure([&v]() {
                         return scicpp::signal::power_spectrum_density(
                             v, 1.0, scicpp::signal::windows::Window::Hann);
                     });
                 })