// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "utils.hpp"

#include "scicpp/core/numeric.hpp"

namespace scicpp::utils {

TEST_CASE("set_array") {
    const std::vector v{1, 2, 3};
    const std::array a{1, 2, 3};

    REQUIRE(set_array(v) == std::vector<int>(v.size()));
    REQUIRE(set_array(a) == std::array<int, a.size()>{});
}

TEST_CASE("move_sub_vector") {
    REQUIRE(move_sub_vector(std::vector{1, 2, 3}, 2) == std::vector{1, 2});
    REQUIRE(move_sub_vector(std::vector{1, 2, 3}, 0) == std::vector<int>{});
    REQUIRE(move_sub_vector(std::vector{1, 2, 3}, 4) == std::vector{1, 2, 3});
}

} // namespace scicpp::utils