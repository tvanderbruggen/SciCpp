// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <cstdio>
#include <scicpp/polynomials.hpp>

int main() {
    scicpp::polynomial::Polynomial P1{1., 2., 3.};
    scicpp::polynomial::Polynomial P2{9., 5., 1.};
    auto P = 2. * P1 * P2;

    // Print the resulting polynomial degree
    printf("deg(P) = %lu\n", P.degree());

    // Evaluate the polynomial for x = 3.14
    printf("P(3.14) = %f\n", P(3.14));
}