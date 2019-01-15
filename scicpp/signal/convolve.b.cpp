// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "convolve.hpp"

#include "scicpp/core.hpp"

NONIUS_BENCHMARK("signal::convolve (double, 1000 x 100)",
                 [](nonius::chronometer meter) {
                     const auto a = scicpp::random::rand<double>(1000);
                     const auto v = scicpp::random::rand<double>(100);

                     meter.measure(
                         [&]() { return scicpp::signal::convolve(a, v); });
                 })

NONIUS_BENCHMARK("signal::fftconvolve (double, 1000 x 100)",
                 [](nonius::chronometer meter) {
                     const auto a = scicpp::random::rand<double>(1000);
                     const auto v = scicpp::random::rand<double>(100);

                     meter.measure(
                         [&]() { return scicpp::signal::fftconvolve(a, v); });
                 })

NONIUS_BENCHMARK("signal::convolve (double, 10000 x 1000)",
                 [](nonius::chronometer meter) {
                     const auto a = scicpp::random::rand<double>(10000);
                     const auto v = scicpp::random::rand<double>(1000);

                     meter.measure(
                         [&]() { return scicpp::signal::convolve(a, v); });
                 })

NONIUS_BENCHMARK("signal::fftconvolve (double, 10000 x 1000)",
                 [](nonius::chronometer meter) {
                     const auto a = scicpp::random::rand<double>(10000);
                     const auto v = scicpp::random::rand<double>(1000);

                     meter.measure(
                         [&]() { return scicpp::signal::fftconvolve(a, v); });
                 })

// NONIUS_BENCHMARK("signal::convolve (float, 10000 x 1000)",
//                  [](nonius::chronometer meter) {
//                      const auto a = scicpp::random::rand<float>(10000);
//                      const auto v = scicpp::random::rand<float>(1000);

//                      meter.measure(
//                          [&]() { return scicpp::signal::convolve(a, v); });
//                  })

// NONIUS_BENCHMARK("signal::fftconvolve (float, 10000 x 1000)",
//                  [](nonius::chronometer meter) {
//                      const auto a = scicpp::random::rand<float>(10000);
//                      const auto v = scicpp::random::rand<float>(1000);

//                      meter.measure(
//                          [&]() { return scicpp::signal::fftconvolve(a, v); });
//                  })