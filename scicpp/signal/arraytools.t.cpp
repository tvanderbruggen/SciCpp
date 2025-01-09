// SPDX-License-Identifier: MIT

#include "arraytools.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

#include <array>
#include <vector>

namespace scicpp::signal {

TEST_CASE("Eye") {
    // SECTION("Eye 0") {
    //     const auto result = detail::eye<int>(3, 0, -1);
    //     REQUIRE(almost_equal<1>(result, {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}}));
    // }
    // SECTION("Eye 1") {
    //     const auto result = detail::eye<int>(3, 0, 1);
    //     REQUIRE(almost_equal<1>(result, {{0, 1, 0}, {0, 0, 1}, {0, 0, 0}}));
    // }
    // SECTION("Eye 2") {
    //     const auto result = detail::eye<int>(3, 2, 1);
    //     REQUIRE(almost_equal<1>(result, {{0, 1}, {0, 0}, {0, 0}}));
    // }
    // SECTION("Eye 3") {
    //     const auto result = detail::eye<int>(2);
    //     REQUIRE(almost_equal<1>(result, {{1, 0}, {0, 1}}));
    // }
}

TEST_CASE("odd_ext") {
    SECTION("std::vector") {
        std::vector arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(odd_ext(arr, 0), arr));
        REQUIRE(array_equal(odd_ext(arr, 1), {0, 1, 2, 3, 4, 5, 6, 7, 8}));
        REQUIRE(
            array_equal(odd_ext(arr, 2), {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(array_equal(odd_ext(arr, 3),
                            {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }

    SECTION("std::vector units") {
        using namespace units::literals;
        std::vector arr{1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J};
        REQUIRE(almost_equal(odd_ext(arr, 0), arr));
        REQUIRE(almost_equal(odd_ext(arr, 1),
                             {0_J, 1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J, 8_J}));
    }

    SECTION("std::array") {
        std::array arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(odd_ext(arr, 0), {1, 2, 3, 4, 5, 6, 7}));
        REQUIRE(array_equal(odd_ext(arr, 1), {0, 1, 2, 3, 4, 5, 6, 7, 8}));
        REQUIRE(
            array_equal(odd_ext(arr, 2), {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        REQUIRE(array_equal(odd_ext(arr, 3),
                            {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }
}

TEST_CASE("even_ext") {
    SECTION("std::vector") {
        std::vector arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(even_ext(arr, 0), arr));
        REQUIRE(array_equal(even_ext(arr, 1), {2, 1, 2, 3, 4, 5, 6, 7, 6}));
        REQUIRE(
            array_equal(even_ext(arr, 2), {3, 2, 1, 2, 3, 4, 5, 6, 7, 6, 5}));
    }

    SECTION("std::vector units") {
        using namespace units::literals;
        std::vector arr{1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J};
        REQUIRE(almost_equal(even_ext(arr, 0), arr));
        REQUIRE(almost_equal(even_ext(arr, 1),
                             {2_J, 1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J, 6_J}));
        REQUIRE(almost_equal(
            even_ext(arr, 2),
            {3_J, 2_J, 1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J, 6_J, 5_J}));
    }

    SECTION("std::array") {
        std::array arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(even_ext(arr, 0), {1, 2, 3, 4, 5, 6, 7}));
        REQUIRE(array_equal(even_ext(arr, 1), {2, 1, 2, 3, 4, 5, 6, 7, 6}));
        REQUIRE(
            array_equal(even_ext(arr, 2), {3, 2, 1, 2, 3, 4, 5, 6, 7, 6, 5}));
    }
}

TEST_CASE("const_ext") {
    SECTION("std::vector") {
        std::vector arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(const_ext(arr, 0), {1, 2, 3, 4, 5, 6, 7}));
        REQUIRE(array_equal(const_ext(arr, 1), {1, 1, 2, 3, 4, 5, 6, 7, 7}));
        REQUIRE(
            array_equal(const_ext(arr, 2), {1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7}));
    }

    SECTION("std::vector units") {
        using namespace units::literals;
        std::vector arr{1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J};
        REQUIRE(almost_equal(const_ext(arr, 0),
                             {1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J}));
        REQUIRE(almost_equal(const_ext(arr, 1),
                             {1_J, 1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J, 7_J}));
        REQUIRE(almost_equal(
            const_ext(arr, 2),
            {1_J, 1_J, 1_J, 2_J, 3_J, 4_J, 5_J, 6_J, 7_J, 7_J, 7_J}));
    }

    SECTION("std::array") {
        std::array arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(const_ext(arr, 0), {1, 2, 3, 4, 5, 6, 7}));
        REQUIRE(array_equal(const_ext(arr, 1), {1, 1, 2, 3, 4, 5, 6, 7, 7}));
        REQUIRE(
            array_equal(const_ext(arr, 2), {1, 1, 1, 2, 3, 4, 5, 6, 7, 7, 7}));
    }
}

TEST_CASE("zero_ext") {
    SECTION("std::vector") {
        std::vector arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(zero_ext(arr, 0), {1, 2, 3, 4, 5, 6, 7}));
        REQUIRE(array_equal(zero_ext(arr, 1), {0, 1, 2, 3, 4, 5, 6, 7, 0}));
        REQUIRE(
            array_equal(zero_ext(arr, 2), {0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0}));
        REQUIRE(array_equal(zero_ext(empty<int>(), 2), {0, 0, 0, 0}));
    }

    SECTION("std::vector units") {
        using namespace units::literals;
        std::vector arr{1_m, 2_m, 3_m, 4_m, 5_m, 6_m, 7_m};
        REQUIRE(almost_equal(zero_ext(arr, 0),
                             {1_m, 2_m, 3_m, 4_m, 5_m, 6_m, 7_m}));
        REQUIRE(almost_equal(zero_ext(arr, 1),
                             {0_m, 1_m, 2_m, 3_m, 4_m, 5_m, 6_m, 7_m, 0_m}));
        REQUIRE(almost_equal(
            zero_ext(arr, 2),
            {0_m, 0_m, 1_m, 2_m, 3_m, 4_m, 5_m, 6_m, 7_m, 0_m, 0_m}));
    }

    SECTION("std::array") {
        std::array arr{1, 2, 3, 4, 5, 6, 7};
        REQUIRE(array_equal(zero_ext(arr, 0), {1, 2, 3, 4, 5, 6, 7}));
        REQUIRE(array_equal(zero_ext(arr, 1), {0, 1, 2, 3, 4, 5, 6, 7, 0}));
        REQUIRE(
            array_equal(zero_ext(arr, 2), {0, 0, 1, 2, 3, 4, 5, 6, 7, 0, 0}));
    }
}

} // namespace scicpp::signal
