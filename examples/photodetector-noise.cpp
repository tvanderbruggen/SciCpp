// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

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
    constexpr auto Cpd = 1_pF;      // Photodiode capacitance

    // The photocurrent noise density introduced
    // by the photon shot-noise is:
    const auto In_ph = sci::units::sqrt(2. * sci::phys_cst::e * S * Popt);

    printf("Shot-noise at %.3e W is %.3e A/rtHz\n", Popt.eval(), In_ph.eval());

    // Resistor thermal noise
    constexpr auto Rf = 12_kOhm;
    constexpr auto T = 300_K;
    const auto In_th = sci::units::sqrt(4. * sci::phys_cst::k * T / Rf);

    // ------- Amplifier noise (OPA847)

    // Here we use nA/rtHz instead of pA/rtHz because when we square the
    // current noise latter on, the integer ratio would overflow
    // (1 / 1000'000'000'000) x (1 / 1000'000'000'000) = (1 / 1000'000'000'000'000'000'000'000).

    constexpr auto In_amp = 3.5E-3_nA_per_rtHz;  // Current noise density
    constexpr auto Vn_amp = 8.55E-3_uV_per_rtHz; // Voltage noise density
    constexpr auto Camp = 1.2_pF + 2.5_pF;       // Amplifier input capacitance

    const auto f = sci::linspace<100>(0_GHz, 0.2_GHz);
    const auto In_amp_v = 2. * sci::pi<double> * (Cpd + Camp) * f * Vn_amp;

    // Total detector noise (does not include shot-noise)
    const auto In_det =
        sci::sqrt(In_amp * In_amp + In_th * In_th + In_amp_v * In_amp_v / 3.);

    printf("Detector noise is %.3e A/rtHz\n", In_det[0].eval());

    // Detector noise equivalent power (NEP)
    const auto NEP = In_det / S;

    printf("Detector NEP is %.3e W/rtHz \n", NEP[0].eval());

    // Some dimensional analysis checks
    static_assert(
        sci::units::is_same_dimension<decltype(In_ph), decltype(In_th)>);
    static_assert(
        sci::units::is_same_dimension<decltype(In_ph), decltype(In_amp)>);
}