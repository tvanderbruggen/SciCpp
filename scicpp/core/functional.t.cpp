// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "functional.hpp"

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("map") {
    const std::vector v{1., 2., 3.};
    const std::vector a{4., 5., 6.};

    REQUIRE(almost_equal(map(std::negate<>(), std::array{1., 2., 3.}),
                         {-1., -2., -3.}));
    REQUIRE(almost_equal(map(std::negate<>(), std::vector{1., 2., 3.}),
                         {-1., -2., -3.}));
    REQUIRE(almost_equal(map(std::negate<>(), v), {-1., -2., -3.}));
    REQUIRE(almost_equal(
        map(std::plus<>(), std::array{1., 2., 3.}, std::array{4., 5., 6.}),
        {5., 7., 9.}));
    REQUIRE(almost_equal(
        map(std::plus<>(), std::vector{1., 2., 3.}, std::vector{4., 5., 6.}),
        {5., 7., 9.}));
    REQUIRE(almost_equal(map(std::plus<>(), std::vector{1., 2., 3.}, a),
                         {5., 7., 9.}));
    REQUIRE(almost_equal(map(std::plus<>(), v, std::vector{4., 5., 6.}),
                         {5., 7., 9.}));
    REQUIRE(almost_equal(map(std::plus<>(), v, a), {5., 7., 9.}));

    // Check const arrays are not modified
    REQUIRE(v == std::vector{1., 2., 3.});
    REQUIRE(a == std::vector{4., 5., 6.});
}

TEST_CASE("vectorize") {
    auto f = vectorize([](double x) { return 2 * x; });
    REQUIRE(almost_equal(f(std::vector{1., 2., 3.}), {2., 4., 6.}));

    auto g = vectorize([](auto x) { return std::sin(x); });
    REQUIRE(almost_equal(g(std::array{1., 2., 3.}),
                         {std::sin(1.), std::sin(2.), std::sin(3.)}));

    auto h = vectorize([](double x, double y) { return x + y; });
    REQUIRE(almost_equal(h(std::vector{1., 2., 3.}, std::vector{4., 5., 6.}),
                         {5., 7., 9.}));
}

TEST_CASE("filter") {
    const std::vector v{1., 2., 3., 4., 5.};

    REQUIRE(almost_equal(filter(v, [](auto v) { return v > 3.; }), {4., 5.}));
    REQUIRE(almost_equal(
        filter(std::vector{1., 2., 3., 4., 5.}, [](auto v) { return v > 3.; }),
        {4., 5.}));

    REQUIRE(v == std::vector{1., 2., 3., 4., 5.});
}

TEST_CASE("filter_reduce") {
    static_assert(
        filter_reduce(std::array{1, 2, 3, 4, 5}, std::plus<>(), 0, [](auto v) {
            return v % 2 == 0;
        }) == 6);

    REQUIRE(
        filter_reduce(std::array{1, 2, 3, 4, 5}, std::plus<>(), 0, [](auto v) {
            return v % 2 == 0;
        }) == 6);

    REQUIRE(filter_reduce(std::vector<int>{}, std::plus<>(), 1, [](auto v) {
                return v % 2 == 0;
            }) == 1);
}

} // namespace scicpp