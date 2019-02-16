// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <Eigen/Dense>
#include <array>
#include <scicpp/core.hpp>
#include <scicpp/linalg.hpp>

int main() {
    Eigen::Matrix<double, 4, 2> A;
    A << 0., 1., //
        1., 1.,  //
        2., 1.,  //
        3., 1.;

    const std::array b{-1., 0.2, 0.9, 2.1};
    const auto x = scicpp::linalg::lstsq(A, b);
    scicpp::print(x);
}