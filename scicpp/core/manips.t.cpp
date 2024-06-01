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

    SECTION("std::array<quantity> / std::array<quantity>") {
        using namespace units::literals;
        const auto a1 = std::array{1_m, 2_m, 3_m};
        const auto a2 = std::array{4_mm, 5_mm, 6_mm};
        const auto res = concatenate(a1, a2);
        // print(res);
        REQUIRE(almost_equal(res, {1_m, 2_m, 3_m, 4_mm, 5_mm, 6_mm}));
    }

    SECTION("std::array / std::vector") {
        const auto a1 = std::array{1.0, 2.0, 3.0};
        const auto a2 = std::vector{4, 5, 6, 7, 8};
        const auto res = concatenate(a1, a2);
        REQUIRE(almost_equal(res, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}));
    }

    SECTION("std::array<quantity> / std::vector<quantity>") {
        using namespace units::literals;
        const auto a1 = std::array{1_g, 2_g, 3_g};
        const auto a2 = std::vector{4_kg, 5_kg, 6_kg, 7_kg, 8_kg};
        const auto res = concatenate(a1, a2);
        // print(res);
        REQUIRE(
            almost_equal(res, {1_g, 2_g, 3_g, 4_kg, 5_kg, 6_kg, 7_kg, 8_kg}));
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

    SECTION("std::vector&& / std::array quantities") {
        using namespace units::literals;
        auto a1 = std::vector{1_mA, 2_mA, 3_mA};
        const auto a2 = std::array{4_uA, 5_uA, 6_uA, 7_uA, 8_uA};
        const auto res = concatenate(std::move(a1), a2);
        REQUIRE(almost_equal(res,
                             {1_mA, 2_mA, 3_mA, 4_uA, 5_uA, 6_uA, 7_uA, 8_uA}));
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

    SECTION("std::vector / std::vector&& quantity") {
        using namespace units::literals;
        const auto a1 = std::vector{1_hPa, 2_hPa, 3_hPa};
        auto a2 = std::vector{4_Pa, 5_Pa, 6_Pa, 7_Pa, 8_Pa};
        const auto res = concatenate(a1, std::move(a2));
        // print(res);
        REQUIRE(almost_equal(
            res, {1_hPa, 2_hPa, 3_hPa, 4_Pa, 5_Pa, 6_Pa, 7_Pa, 8_Pa}));
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

TEST_CASE("flip") {
    SECTION("empty") {
        REQUIRE(flip(empty<float>()).empty());
    }

    SECTION("const std::array") {
        const auto a = std::array{1, 2, 3};
        REQUIRE(array_equal(flip(a), {3, 2, 1}));
    }

    SECTION("std::array&&") {
        auto a = std::array{1, 2, 3};
        REQUIRE(array_equal(flip(std::move(a)), {3, 2, 1}));
    }

    SECTION("std::vector&&") {
        auto v = std::vector{1, 2, 3};
        REQUIRE(array_equal(flip(std::move(v)), {3, 2, 1}));
    }
}

} // namespace scicpp
