// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "manips.hpp"

#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("concatenate") {
    SECTION("std::array / std::array") {
        const auto a1 = std::array{1.0, 2.0, 3.0};
        const auto a2 = std::array{4, 5, 6};
        const auto res = concatenate(a1, a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}));
    }

    // SECTION("std::array<quantity> / std::array<quantity>") {
    //     // TODO

    //     using namespace units::literals;

    //     const auto a1 = std::array{1_m, 2_m, 3_m};
    //     const auto a2 = std::array{4_mm, 5_mm, 6_mm};
    //     const auto res = concatenate(a1, a2);
    //     REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}));
    // }

    SECTION("std::array / std::vector") {
        const auto a1 = std::array{1.0, 2.0, 3.0};
        const auto a2 = std::vector{4, 5, 6, 7, 8};
        const auto res = concatenate(a1, a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}));
    }

    SECTION("std::vector / std::array") {
        const auto a1 = std::vector{1.0, 2.0, 3.0};
        const auto a2 = std::array{4, 5, 6, 7, 8};
        const auto res = concatenate(a1, a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}));
    }

    SECTION("std::vector / std::vector") {
        const auto a1 = std::vector{1.0, 2.0, 3.0};
        const auto a2 = std::vector{4, 5, 6, 7, 8};
        const auto res = concatenate(a1, a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}));
    }

    SECTION("std::vector&& / std::array same type") {
        auto a1 = std::vector{1.0, 2.0, 3.0};
        const auto a2 = std::array{4.0, 5.0, 6.0, 7.0, 8.0};
        const auto res = concatenate(std::move(a1), a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}));
    }

    SECTION("std::vector&& / std::vector convertible types") {
        auto a1 = std::vector{1.0, 2.0, 3.0};
        const auto a2 = std::vector{4, 5, 6, 7, 8};
        const auto res = concatenate(std::move(a1), a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}));
    }

    SECTION("std::vector / std::vector&& same type") {
        const auto a1 = std::vector{1, 2, 3};
        auto a2 = std::vector{4, 5, 6, 7, 8};
        const auto res = concatenate(a1, std::move(a2));
        // print(res);
        // print(res.size());
        REQUIRE(array_equal(res, {1, 2, 3, 4, 5, 6, 7, 8}));
    }

    SECTION("std::vector / std::vector&& convertible types") {
        const auto a1 = std::vector{1.0, 2.0, 3.0};
        auto a2 = std::vector{4, 5, 6, 7, 8};
        const auto res = concatenate(a1, std::move(a2));
        // print(res);
        // print(res.size());
        REQUIRE(array_equal(res, {1, 2, 3, 4, 5, 6, 7, 8}));
    }

    SECTION("Operator |") {
        using namespace operators;

        const auto a1 = std::vector{1, 2, 3};
        const auto a2 = std::vector{4, 5};
        const auto a3 = std::vector{6, 7, 8};
        const auto res = a1 | a2 | a3;
        REQUIRE(array_equal(res, {1, 2, 3, 4, 5, 6, 7, 8}));
    }

    SECTION("Many std::vectors") {
        const auto a1 = std::vector{1, 2, 3};
        const auto a2 = std::vector{4, 5};
        const auto a3 = std::vector{6, 7, 8};
        const auto res = concatenate(a1, a2, a3);
        REQUIRE(array_equal(res, {1, 2, 3, 4, 5, 6, 7, 8}));
    }
}

} // namespace scicpp
