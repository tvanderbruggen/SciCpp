// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "windows.hpp"

NONIUS_BENCHMARK("signal::windows::boxcar (1000)",
                 []() { return scicpp::signal::windows::boxcar<double>(1000); })

NONIUS_BENCHMARK("signal::windows::bartlett (1000)", []() {
    return scicpp::signal::windows::bartlett<double>(1000);
})

NONIUS_BENCHMARK("signal::windows::cosine (1000)",
                 []() { return scicpp::signal::windows::cosine<double>(1000); })

NONIUS_BENCHMARK("signal::windows::hann (1000)",
                 []() { return scicpp::signal::windows::hann<double>(1000); })

NONIUS_BENCHMARK("signal::windows::hamming (1000)", []() {
    return scicpp::signal::windows::hamming<double>(1000);
})

NONIUS_BENCHMARK("signal::windows::blackman (1000)", []() {
    return scicpp::signal::windows::blackman<double>(1000);
})

NONIUS_BENCHMARK("signal::windows::nuttall (1000)", []() {
    return scicpp::signal::windows::nuttall<double>(1000);
})

NONIUS_BENCHMARK("signal::windows::blackmanharris (1000)", []() {
    return scicpp::signal::windows::blackmanharris<double>(1000);
})

NONIUS_BENCHMARK("signal::windows::flattop (1000)", []() {
    return scicpp::signal::windows::flattop<double>(1000);
})

NONIUS_BENCHMARK("signal::windows::gaussian (1000)", []() {
    return scicpp::signal::windows::gaussian<double>(1000, 1.0);
})