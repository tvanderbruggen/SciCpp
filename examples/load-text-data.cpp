// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example is inspired from:
// https://scipython.com/book/chapter-6-numpy/examples/using-numpys-loadtxt-method/
//
// It presents how data from this dataset can be loaded and manipulated using SciCpp.

#include <cstdio>
#include <scicpp/core.hpp>
#include <string>

namespace sci = scicpp;
using namespace sci::operators;

// We use type aliasing to define the data type of each column.
// When the data have physical unit, we use an unit type.
using Gender = bool; // True = M, False = F
using Height = sci::units::meter<>;
using Weight = sci::units::kilogram<>;

// Replace the missing weight by an unphysical negative value
auto parse_weight(const char *s) {
    try {
        return sci::units::kilogram<>(std::stod(s));
    } catch (...) {
        return sci::units::kilogram<>(-99.);
    }
}

// Filter valid weights
bool weight_filter(sci::units::kilogram<> w) {
    return w > sci::units::kilogram<>(0.);
}

int main() {
    const auto a = sci::TxtLoader<Gender, Height, Weight>()
                       .skiprows(10)
                       .usecols(1, 3, 4)
                       .converters({{1, [](auto x) { return x[0] == 'M'; }},
                                    {4, parse_weight}})
                       .load("examples/data1.csv");

    sci::print(a);

    // We unpack the columns into individual vectors
    const auto [genders, heights, weights] = sci::unpack(a);

    const auto m_av = sci::stats::mean(sci::mask(heights, genders));
    const auto f_av = sci::stats::mean(sci::mask(heights, !genders));
    printf("Male average: %.2f m, Female average: %.2f m\n",
           m_av.value(),
           f_av.value());

    // We use the mean function with a filter
    const auto m_wav =
        sci::stats::mean(sci::mask(weights, genders), weight_filter);
    const auto f_wav =
        sci::stats::mean(sci::mask(weights, !genders), weight_filter);
    printf("Male average: %.2f kg, Female average: %.2f kg\n",
           m_wav.value(),
           f_wav.value());
}
