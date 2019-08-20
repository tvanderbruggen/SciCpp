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

    printf("Shot-noise at %.3e W is %.3e A/rtHz\n", Popt.eval(), In_ph.eval());

    // Resistor thermal noise
    constexpr auto Rf = 12_kOhm;
    constexpr auto T = 300_K;
    const auto In_th = sci::units::sqrt(4. * sci::phys_cst::k * T / Rf);

    // ------- Amplifier noise (OPA847)
    constexpr auto In_amp = 3.5_pA_per_rtHz;
    constexpr auto Vn_amp = 0.85_nV_per_rtHz;

    const auto f = sci::linspace<100>(0_MHz, 200_MHz);

    // Total detector noise (does not include shot-noise)

    // Some dimensional analysis checks
    static_assert(
        sci::units::is_same_dimension<decltype(In_ph), decltype(In_th)>);
    static_assert(
        sci::units::is_same_dimension<decltype(In_ph), decltype(In_amp)>);
}