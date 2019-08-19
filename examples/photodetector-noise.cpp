// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example presents some simple estimates of a photodetector
// (photodiode + amplifier noise)

#include <cstdio>
#include <scicpp/core.hpp>

namespace sci = scicpp;
using namespace sci::operators;
using namespace sci::units::literals;

int main() {
    // ------- Photon shot-noise
    constexpr auto S = 0.9_A / 1_W; // Photodiode sensitivity
    constexpr auto Popt = 100_uW;   // Incident optical power

    // The photocurrent noise density introduced
    // by the photon shot-noise is:
    const auto In_ph = sci::units::sqrt(2. * sci::phys_cst::e * S * Popt);

    printf("Current shot-noise for %.3e W is %.3e A/rtHz\n",
           Popt.eval(),
           In_ph.eval());
}