// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

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

TEST_CASE("move_subvector") {
    REQUIRE(move_subvector(std::vector{1, 2, 3}, 2) == std::vector{1, 2});
    REQUIRE(move_subvector(std::vector{1, 2, 3}, 0) == std::vector<int>{});
    REQUIRE(move_subvector(std::vector{1, 2, 3}, 4) == std::vector{1, 2, 3});
}

TEST_CASE("prime_factors") {
    constexpr auto factors = prime_factors<84628766448>().values();
    static_assert(factors[0].first == 2 && factors[0].second == 4);
    static_assert(factors[1].first == 3 && factors[1].second == 2);
    static_assert(factors[2].first == 31 && factors[2].second == 1);
    static_assert(factors[3].first == 59 && factors[3].second == 1);
    static_assert(factors[4].first == 321323 && factors[4].second == 1);
    print(factors);
}

} // namespace scicpp::utils