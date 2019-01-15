// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "linalg.hpp"

#include <iostream>

namespace scicpp {
namespace linalg {

TEST_CASE("lstsq") {
    // SECTION("std::array 1") {
    //     Eigen::Matrix<double, 2, 3> A;
    //     A << 1., 2., 3., //
    //         4., 5., 6.;

    //     std::array b{10., 11.};
    //     const auto y = lstsq(A, b);
    //     REQUIRE(y.size() == 3);
    //     printf("%.16f, %.16f, %.16f\n", y[0], y[1], y[2]);
    //     // std::cout << y[0] << " " << y[1] << " " << y[2] << " " <<
    //     std::endl; REQUIRE_FLOAT_EQUAL(y[0], -4.555555555555557);
    // }

    SECTION("std::array") {
        Eigen::Matrix<double, 4, 2> A;
        A << 0., 1., //
            1., 1.,  //
            2., 1.,  //
            3., 1.;

        const std::array b{-1., 0.2, 0.9, 2.1};
        const auto y = lstsq(A, b);
        REQUIRE(y.size() == 2);
        // printf("%.16f, %.16f\n", y[0], y[1]);
        REQUIRE_FLOAT_EQUAL(y[0], 1.);
        REQUIRE_FLOAT_EQUAL(y[1], -0.95);
    }

    SECTION("std::vector") {
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A(4, 2);
        A << 0., 1., //
            1., 1.,  //
            2., 1.,  //
            3., 1.;

        const std::vector b{-1., 0.2, 0.9, 2.1};
        const auto y = lstsq(A, b);
        REQUIRE(y.size() == 2);
        // printf("%.16f, %.16f\n", y[0], y[1]);
        REQUIRE_FLOAT_EQUAL(y[0], 1.);
        REQUIRE_FLOAT_EQUAL(y[1], -0.95);
    }
}

} // namespace linalg
} // namespace scicpp