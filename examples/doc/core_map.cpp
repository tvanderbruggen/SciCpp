// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <array>
#include <complex>
#include <functional>
#include <scicpp/core.hpp>
#include <vector>

int main() {
    using namespace std::literals; // For std::complex notation

    // Unary operations

    // Negate all elements of an array
    const auto neg = scicpp::map(std::negate<>(), std::array{1., 2., 3.});
    scicpp::print(neg);

    // Conjuguate all elements of a vector of complex numbers
    const auto cj = scicpp::map([](auto z) { return std::conj(z); },
                                std::vector{1. + 2.i, 2. - 3.i});
    scicpp::print(cj);

    // Binary operations

    // Sum two arrays
    const auto s = scicpp::map(
        std::plus<>(), std::array{1., 2., 3.}, std::array{4., 5., 6.});
    scicpp::print(s);

    // Compute the product of sinus between two vectors
    const auto p =
        scicpp::map([](auto x, auto y) { return std::sin(x) * std::sin(y); },
                    std::vector{1., 2., 3.},
                    std::vector{4., 5., 6.});
    scicpp::print(p);
}