// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example is inspired from:
// https://scipython.com/book/chapter-6-numpy/examples/using-numpys-loadtxt-method/
//
// It presents how data from this dataset can be loaded and manipulated using SciCpp.

#include <cstdio>
#include <scicpp/core.hpp>
#include <string>
#include <utility>

namespace sci = scicpp;
using namespace sci::operators;
using namespace sci::units::literals;

// We use type aliasing to define the data type of each column.
// When the data have physical unit, we use an unit type.
using Gender = bool; // True = M, False = F
using Height = sci::units::meter<>;
using Weight = sci::units::kilogram<>;
using BloodPressure = std::pair<sci::units::mmHg<>, sci::units::mmHg<>>;

// Replace the missing weight by an unphysical negative value
auto parse_weight(const char *s) {
    try {
        return Weight(std::stod(s));
    } catch (...) {
        return -99_kg;
    }
}

auto parse_blood_pressure(const char *s) {
    double systolic, diastolic;

    if (std::sscanf(s, "%lf/%lf", &systolic, &diastolic) > 0) {
        return std::make_pair(sci::units::mmHg<>(systolic),
                              sci::units::mmHg<>(diastolic));
    } else { // Invalid input
        return std::make_pair(-99_mmHg, -99_mmHg);
    }
}

int main() {
    const auto a = sci::TxtLoader<Gender, Height, Weight, BloodPressure>()
                       .skiprows(10)
                       .usecols(1, 3, 4, 5)
                       .converters({{1, [](auto x) { return x[0] == 'M'; }},
                                    {4, parse_weight},
                                    {5, parse_blood_pressure}})
                       .load("examples/data1.csv");

    // sci::print(a);

    // We unpack the columns into individual vectors
    const auto [genders, heights, weights, blood_pressures] = sci::unpack(a);

    const auto m_av = sci::stats::mean(sci::mask(heights, genders));
    const auto f_av = sci::stats::mean(sci::mask(heights, !genders));
    printf("Male average: %.2f m, Female average: %.2f m\n",
           m_av.value(),
           f_av.value());

    // We use the mean function with a filter
    const auto weight_filter = [](auto w) { return w > 0_kg; };
    const auto m_wav =
        sci::stats::mean(sci::mask(weights, genders), weight_filter);
    const auto f_wav =
        sci::stats::mean(sci::mask(weights, !genders), weight_filter);
    printf("Male average: %.2f kg, Female average: %.2f kg\n",
           m_wav.value(),
           f_wav.value());

    const auto [systolic, diastolic] = sci::unpack(blood_pressures);
    const auto blood_pressure_filter = [](auto p) { return p > 0_mmHg; };
    const auto m_syst_av =
        sci::stats::mean(sci::mask(systolic, genders), blood_pressure_filter);
    const auto f_syst_av =
        sci::stats::mean(sci::mask(systolic, !genders), blood_pressure_filter);
    const auto m_diast_av =
        sci::stats::mean(sci::mask(diastolic, genders), blood_pressure_filter);
    const auto f_diast_av =
        sci::stats::mean(sci::mask(diastolic, !genders), blood_pressure_filter);

    printf("Male average: %.2f/%.2f mmHg, Female average: %.2f/%.2f mmHg\n",
           m_syst_av.value(), m_diast_av.value(),
           f_syst_av.value(), f_diast_av.value());
}
