// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "utils.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp::utils {

TEST_CASE("set_array") {
    const std::vector v{1, 2, 3};
    const std::array a{1, 2, 3};

    REQUIRE(set_array(v) == std::vector<int>(v.size()));
    REQUIRE(set_array(a) == std::array<int, a.size()>{});
    REQUIRE(set_array<float>(a) == std::array<float, a.size()>{});
    REQUIRE(set_array<float>(v) == std::vector<float>(v.size()));
}

TEST_CASE("subvector") {
    REQUIRE(subvector(std::vector{1, 2, 3}, 2) == std::vector{1, 2});
    REQUIRE(subvector(std::vector{1, 2, 3}, 0) == std::vector<int>{});
    REQUIRE(subvector(std::vector{1, 2, 3}, 4) == std::vector{1, 2, 3});
    const auto v = std::vector{1, 2, 3};
    REQUIRE(subvector(v, 2) == std::vector{1, 2});
    REQUIRE(subvector(v, 4) == std::vector{1, 2, 3});
}

} // namespace scicpp::utils