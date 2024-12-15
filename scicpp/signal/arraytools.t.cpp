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
    std::vector arr{1, 2, 3, 4, 5, 6, 7};

    SECTION("Odd Ext 0") {
        const auto result = odd_ext<int>(arr, 0);
        REQUIRE(array_equal(result, {1, 2, 3, 4, 5, 6, 7}));
    }

    SECTION("Odd Ext 1") {
        const auto result = odd_ext<int>(arr, 1);
        REQUIRE(array_equal(result, {0, 1, 2, 3, 4, 5, 6, 7, 8}));
    }

    SECTION("Odd Ext 2") {
        const auto result = odd_ext<int>(arr, 2);
        REQUIRE(array_equal(result, {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
    }

    SECTION("Odd Ext 3") {
        const auto result = odd_ext<int>(arr, 3);
        REQUIRE(
            array_equal(result, {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }

    // SECTION("Axis Slice") {
    //     std::vector<std::vector<int>> arr = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } }
    //     const auto result = detail::slice(arr, 0, 1, 1, 1);
    //     scicpp::print(result);
    //     REQUIRE(almost_equal<1>(result, { {1}, {4}, {7} }));
    // }
}

} // namespace scicpp::signal
