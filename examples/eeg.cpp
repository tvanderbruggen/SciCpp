// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example is inspired from:
// https://raphaelvallat.com/bandpower.html

#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

namespace sci = scicpp;
namespace plt = sci::plots;
namespace sig = sci::signal;

int main() {
    using namespace sci::operators;
    using namespace sci::units::literals;

    // ---- Loading data
    const auto eeg_signal =
        sci::TxtLoader<sci::units::microvolt<>>().skiprows(1).load_vector(
            "examples/eeg-data.txt");
    sci::print(eeg_signal);

    // Sampling frequency and time vector
    constexpr auto fs = 100_Hz;
    const auto time = sci::arange(0.0, double(eeg_signal.size())) / fs;
    sci::print(time);

    auto plot1 = plt::plot(time, eeg_signal);
    plot1.color("black");
    plot1.ylabel("Voltage (uV)");
    plot1.xlabel("Time (s)");
    plot1.fontSize(12);
    plot1.label("N3 sleep EEG data (F3)");
    plot1.show(1200, 400);

    // ---- Welch's periodogram
    // Configure the spectrum analyzer
    auto spec =
        sig::Spectrum{}.fs(fs).window(sig::windows::Hann, time.size() / 6);

    auto plot2 = plt::psd(spec, eeg_signal);
    plot2.size(1000, 500);
    plot2.xlabel("FREQUENCY (Hz)");
    plot2.ylabel("PSD (dB / Hz)");
    plot2.show();
}