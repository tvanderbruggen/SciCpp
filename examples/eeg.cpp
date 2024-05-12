// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example is inspired from:
// https://raphaelvallat.com/bandpower.html

#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

namespace sci = scicpp;
namespace plt = sci::plots;

int main() {
    using namespace sci::operators;
    using namespace sci::units::literals;

    const auto eeg_signal =
        sci::TxtLoader<sci::units::microvolt<>>().skiprows(1).load_vector(
            "examples/eeg-data.txt");
    sci::print(eeg_signal);

    // Sampling frequency and time vector
    constexpr auto fs = 100_Hz;
    const auto time = sci::arange(0.0, double(eeg_signal.size())) / fs;
    sci::print(time);

    auto plot = plt::plot(time, eeg_signal);
    plot.color("black");
    plot.ylabel("Voltage (uV)");
    plot.xlabel("Time (s)");
    plot.fontSize(12);
    plot.label("N3 sleep EEG data (F3)");
    plot.show(1200, 400);
}