#include "arraytools.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp::signal {

TEST_CASE("Slice") {
    std::vector arr{1, 2, 3, 4, 5, 6, 7};

    SECTION("Slice 0") {
        const auto result = detail::slice(arr, -2, -50, -1);
        REQUIRE(array_equal(result, {6, 5, 4, 3, 2, 1}));
    }

    SECTION("Slice 1") {
        const auto result = detail::slice(arr, -2, -5, -2);
        REQUIRE(array_equal(result, {6, 4}));
    }

    SECTION("Slice 2") {
        const auto result = detail::slice(arr, -2, -5, 1);
        REQUIRE(array_equal(result, {}));
    }

    SECTION("Slice 3") {
        const auto result = detail::slice(arr, 2, 5, 1);
        REQUIRE(array_equal(result, {3, 4, 5}));
    }

    SECTION("Slice 4") {
        const auto result = detail::slice(arr, -5, -2, 1);
        REQUIRE(array_equal(result, {3, 4, 5}));
    }

    SECTION("Slice 5") {
        const auto result = detail::slice(arr, -4, 5, 1);
        REQUIRE(array_equal(result, {4, 5}));
    }

    SECTION("Slice 6") {
        const auto result =
            detail::slice(arr, 0, static_cast<int>(arr.size()), 1);
        REQUIRE(array_equal(result, arr));
    }

    SECTION("Slice 7") {
        const auto result = detail::slice(arr, -2, -5, -1);
        REQUIRE(array_equal(result, {6, 5, 4}));
    }

    SECTION("Slice 8") {
        const auto result = detail::slice(arr, 2, -2, 1);
        REQUIRE(array_equal(result, {3, 4, 5}));
    }
}

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
        REQUIRE(array_equal(result,
                                {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    }

    // SECTION("Axis Slice") {
    //     std::vector<std::vector<int>> arr = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } }
    //     const auto result = detail::slice(arr, 0, 1, 1, 1);
    //     scicpp::print(result);
    //     REQUIRE(almost_equal<1>(result, { {1}, {4}, {7} }));
    // }
}

} // namespace scicpp::signal
