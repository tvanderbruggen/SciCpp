// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example loads and explore wave height data provided by:
// https://www.qld.gov.au/environment/coasts-waterways/beach/monitoring

#include <cstdio>
#include <scicpp/core.hpp>

namespace sci = scicpp;
using namespace sci::operators;
using namespace sci::units::literals;

// The data fields are defined here:
// https://www.qld.gov.au/environment/coasts-waterways/beach/monitoring/waves-glossary#wave-height
//
// From these definitions, we define the data types for each column
using Hsig = sci::units::meter<>; // Significant wave height
using Hmax = sci::units::meter<>; // Highest single wave height
using Tz = sci::units::second<>; // Average of the zero up-crossing wave periods
using Tp = sci::units::second<>; // Period of waves producing the most energy.
using SST = sci::units::celsius<>; // Sea surface temperature

int main() {
    const auto [hsig, hmax, tz, tp, sst] =
        sci::TxtLoader<Hsig, Hmax, Tz, Tp, SST>()
            .delimiter(',')
            .skiprows(1)
            .usecols(1, 2, 3, 4, 6)
            .load<sci::io::unpack>(
                "examples/townsville_2019-01-01t00_00-2019-06-30t23_30.csv");

    // Some simple stats:
    printf("Highest wave is %.2f m\n", sci::stats::amax(hmax).value());

    // In the dataset invalid values are represented by -99.9
    // We create a mask to filter out these values:
    // TODO implement comparison operators for arrays
    const auto valid_mask = (hsig >= 0_m) && (hmax >= 0_m) && (tz >= 0_s) &&
                            (tp >= 0_s) && (sst >= 0_degC);

    // Wave power (per meter of wavefront)
    // https://en.wikipedia.org/wiki/Wave_power
    constexpr auto power_coeff = 500_W / 1_m3 / 1_s;
    sci::units::print(power_coeff);
    const auto P = sci::mask(power_coeff * hsig * hsig * tz, valid_mask);
    printf("Average wave power %.2f W/m\n", sci::stats::mean(P).value());
}