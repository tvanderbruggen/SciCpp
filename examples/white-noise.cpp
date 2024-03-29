// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdio>
#include <scicpp/core.hpp>
#include <scicpp/signal.hpp>

namespace sci = scicpp;

int main() {
    // Generate a Gaussian white-noise
    const auto noise = sci::random::randn<double>(10000);
    printf("Noise: mean = %f, std = %f\n",
           sci::stats::mean(noise),
           sci::stats::std(noise));

    const auto [f, psd] = sci::signal::Spectrum{}
                              .window(sci::signal::windows::Flattop, 2000)
                              .welch(noise);

    // The expected PSD for a white noise is 2 * sigma ^ 2
    printf("PSD = %f\n", sci::stats::mean(psd));
}
