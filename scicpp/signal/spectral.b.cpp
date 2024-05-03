// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "spectral.hpp"

#include "scicpp/core.hpp"

NONIUS_BENCHMARK("signal::spectrum 1 thread", [](nonius::chronometer meter) {
    auto x = scicpp::zeros<double>(1048576);
    x[0] = 1.0;
    x[8] = 1.0;
    auto spec = scicpp::signal::Spectrum{}.window(
        scicpp::signal::windows::hann<double>(65536));
    meter.measure([&]() { return spec.welch(x); });
})

NONIUS_BENCHMARK("signal::spectrum 2 threads", [](nonius::chronometer meter) {
    auto x = scicpp::zeros<double>(1048576);
    x[0] = 1.0;
    x[8] = 1.0;
    auto spec = scicpp::signal::Spectrum{}.window(
        scicpp::signal::windows::hann<double>(65536)).nthreads(2);
    meter.measure([&]() { return spec.welch(x); });
})

NONIUS_BENCHMARK("signal::spectrum 4 threads", [](nonius::chronometer meter) {
    auto x = scicpp::zeros<double>(1048576);
    x[0] = 1.0;
    x[8] = 1.0;
    auto spec = scicpp::signal::Spectrum{}.window(
        scicpp::signal::windows::hann<double>(65536)).nthreads(4);
    meter.measure([&]() { return spec.welch(x); });
})

NONIUS_BENCHMARK("signal::spectrum 8 threads", [](nonius::chronometer meter) {
    auto x = scicpp::zeros<double>(1048576);
    x[0] = 1.0;
    x[8] = 1.0;
    auto spec = scicpp::signal::Spectrum{}.window(
        scicpp::signal::windows::hann<double>(65536)).nthreads(8);
    meter.measure([&]() { return spec.welch(x); });
})