// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// This example is inspired from:
// https://scipython.com/book/chapter-6-numpy/examples/using-numpys-loadtxt-method/
//
// We present how data from this dataset can be loaded using SciCpp.

#include <algorithm>
#include <scicpp/core.hpp>

namespace sci = scicpp;

auto is_male(const char *x) {
    const auto end = x + sizeof(x);
    return std::find(x, end, 'F') == end;
}

int main() {
    // We use type aliasing to define the data type of each column.
    using gender = bool; // True = M, False = F
    using height = double;

    const auto a = sci::TxtLoader<gender, height>()
                       .skiprows(10)
                       .usecols(1, 3)
                       .converters({{1, is_male}})
                       .load("examples/data1.csv");

    sci::print(a);
    sci::print(sci::utils::get_elements<height>(a));
}
