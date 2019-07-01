// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example is inspired from:
// https://scipython.com/book/chapter-6-numpy/examples/using-numpys-loadtxt-method/
//
// We present how data from this dataset can be loaded and manipulated using SciCpp.

#include <cstdio>
#include <scicpp/core.hpp>

namespace sci = scicpp;
using namespace sci::operators;

int main() {
    // We use type aliasing to define the data type of each column.
    using gender = bool; // True = M, False = F
    using height = double;

    const auto a = sci::TxtLoader<gender, height>()
                       .skiprows(10)
                       .usecols(1, 3)
                       .converters({{1, [](auto x) { return x[0] == 'M'; }}})
                       .load("examples/data1.csv");

    sci::print(a);

    const auto [genders, heights] = sci::unpack(a);
    const auto m_av = sci::stats::mean(sci::mask(heights, genders));
    const auto f_av = sci::stats::mean(sci::mask(heights, !genders));
    printf("Male average: %.2f m, Female average: %.2f m\n", m_av, f_av);
}
