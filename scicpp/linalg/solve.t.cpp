// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "solve.hpp"

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp::linalg {

TEST_CASE("lstsq") {
    SECTION("std::array") {
        Eigen::Matrix<double, 4, 2> A;
        A << 0., 1., //
            1., 1.,  //
            2., 1.,  //
            3., 1.;

        const std::array b{-1., 0.2, 0.9, 2.1};
        REQUIRE(almost_equal<4>(lstsq(A, b), {1., -0.95}));
    }

    SECTION("std::vector") {
        Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> A(4, 2);
        A << 0., 1., //
            1., 1.,  //
            2., 1.,  //
            3., 1.;

        const std::vector b{-1., 0.2, 0.9, 2.1};
        REQUIRE(almost_equal<4>(lstsq(A, b), {1., -0.95}));
    }
}

} // namespace scicpp::linalg