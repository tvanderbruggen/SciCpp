// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "io.hpp"

NONIUS_BENCHMARK("Load csv file", [](nonius::chronometer meter) {
    // From examples/wave-height.cpp

    meter.measure([] {
        namespace sci = scicpp;

        using Hsig = sci::units::meter<>;  // Significant wave height
        using Hmax = sci::units::meter<>;  // Highest single wave height
        using Tz = sci::units::second<>;   // Average of the wave periods
        using Tp = sci::units::second<>;   // Period of most energetic waves
        using SST = sci::units::celsius<>; // Sea surface temperature

        auto [hsig, hmax, tz, tp, sst] =
            sci::TxtLoader<Hsig, Hmax, Tz, Tp, SST>()
                .delimiter(',')
                .skiprows(1)
                .usecols(1, 2, 3, 4, 6)
                .load<sci::io::unpack>(
                    "examples/"
                    "townsville_2019-01-01t00_00-2019-06-30t23_30.csv");
    });
})