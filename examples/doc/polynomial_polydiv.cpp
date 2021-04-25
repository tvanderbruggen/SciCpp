// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <scicpp/core.hpp>
#include <scicpp/polynomials.hpp>

int main() {
    const std::vector u{1., 2., 3.};
    const std::vector v{3., 2., 1.};
    const auto [q, r] = scicpp::polynomial::polydiv(u, v);

    scicpp::print(q); // [ 3.0 ]
    scicpp::print(r); // [ -8.0, -4.0 ]
}