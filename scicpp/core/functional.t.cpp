// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "functional.hpp"

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("map") {
    REQUIRE(almost_equal(map(std::negate<>(), std::array{1., 2., 3.}),
                         {-1., -2., -3.}));
    REQUIRE(almost_equal(map(std::negate<>(), std::vector{1., 2., 3.}),
                         {-1., -2., -3.}));
    REQUIRE(almost_equal(
        map(std::plus<>(), std::array{1., 2., 3.}, std::array{4., 5., 6.}),
        {5., 7., 9.}));
    REQUIRE(almost_equal(
        map(std::plus<>(), std::vector{1., 2., 3.}, std::vector{4., 5., 6.}),
        {5., 7., 9.}));
}

} // namespace scicpp