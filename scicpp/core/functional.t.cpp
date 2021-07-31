// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "functional.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"

namespace scicpp {

TEST_CASE("map") {
    const std::vector v{1., 2., 3.};
    const std::vector vc{1. + 3.i, 2. - 2.i, 3. + 1.i};
    const std::vector a{4., 5., 6.};

    SECTION("Unary operations") {
        REQUIRE(almost_equal(map(std::negate<>(), std::array{1., 2., 3.}),
                             {-1., -2., -3.}));
        REQUIRE(almost_equal(map(std::negate<>(), std::vector{1., 2., 3.}),
                             {-1., -2., -3.}));
        REQUIRE(almost_equal(map(std::negate<>(), v), {-1., -2., -3.}));
        REQUIRE(almost_equal(map([](auto z) { return std::conj(z); }, vc),
                             {1. - 3.i, 2. + 2.i, 3. - 1.i}));
        REQUIRE(almost_equal<2>(map([](auto z) { return std::norm(z); }, vc),
                                {10., 8., 10.}));
        const auto x = map([](auto z) { return std::norm(z); },
                           std::array{1. + 3.i, 2. - 2.i, 3. + 1.i});
        REQUIRE(almost_equal<2>(x, {10., 8., 10.}));
        static_assert(
            std::is_same_v<std::decay_t<decltype(x)>, std::array<double, 3>>);
    }

    SECTION("Binary operations") {
        REQUIRE(almost_equal(
            map(std::plus<>(), std::array{1., 2., 3.}, std::array{4., 5., 6.}),
            {5., 7., 9.}));
        REQUIRE(almost_equal(map(std::plus<>(),
                                 std::array{1.i, 2.i, 3.i},
                                 std::array{4., 5., 6.}),
                             {4. + 1.i, 5. + 2.i, 6. + 3.i}));
        REQUIRE(almost_equal(map(std::plus<>(),
                                 std::array{4., 5., 6.},
                                 std::array{1.i, 2.i, 3.i}),
                             {4. + 1.i, 5. + 2.i, 6. + 3.i}));
        REQUIRE(almost_equal(map(std::plus<>(),
                                 std::vector{1., 2., 3.},
                                 std::vector{4., 5., 6.}),
                             {5., 7., 9.}));
        REQUIRE(almost_equal(
            map(std::plus<>(), std::vector{1., 2., 3.}, std::array{4., 5., 6.}),
            {5., 7., 9.}));
        REQUIRE(almost_equal(map(std::plus<>(), v, std::array{4., 5., 6.}),
                             {5., 7., 9.}));
        REQUIRE(almost_equal(
            map(std::plus<>(), std::array{1., 2., 3.}, std::vector{4., 5., 6.}),
            {5., 7., 9.}));
        REQUIRE(almost_equal(map(std::plus<>(), std::vector{1., 2., 3.}, a),
                             {5., 7., 9.}));
        REQUIRE(almost_equal(map(std::plus<>(), v, std::vector{4., 5., 6.}),
                             {5., 7., 9.}));
        REQUIRE(almost_equal(map(std::plus<>(), v, a), {5., 7., 9.}));
        REQUIRE(almost_equal(map(std::plus<>(), v, vc),
                             {2. + 3.i, 4. - 2.i, 6. + 1.i}));

        REQUIRE(almost_equal<2>(
            map([](auto z1, auto z2) { return std::norm(z1) + std::norm(z2); },
                vc,
                vc),
            {20., 16., 20.}));
        REQUIRE(almost_equal<2>(
            map([](auto z1, auto z2) { return std::norm(z1) + std::norm(z2); },
                vc,
                std::vector{1. + 3.i, 2. - 2.i, 3. + 1.i}),
            {20., 16., 20.}));
        REQUIRE(almost_equal<2>(
            map([](auto z1, auto z2) { return std::norm(z1) + std::norm(z2); },
                std::vector{1. + 3.i, 2. - 2.i, 3. + 1.i},
                vc),
            {20., 16., 20.}));

        const auto x =
            map([](auto z1, auto z2) { return std::norm(z1) + std::norm(z2); },
                std::array{1. + 3.i, 2. - 2.i, 3. + 1.i},
                std::array{1. + 3.i, 2. - 2.i, 3. + 1.i});
        REQUIRE(almost_equal<2>(x, {20., 16., 20.}));
        static_assert(
            std::is_same_v<std::decay_t<decltype(x)>, std::array<double, 3>>);
    }

    // Check const arrays are not modified
    // https://stackoverflow.com/questions/54269248/rvalue-reference-overload-difference-between-clang-and-gcc
    REQUIRE(v == std::vector{1., 2., 3.});
    REQUIRE(vc == std::vector{1. + 3.i, 2. - 2.i, 3. + 1.i});
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

    REQUIRE(almost_equal(filter(v, [](auto x) { return x > 3.; }), {4., 5.}));
    REQUIRE(almost_equal(
        filter(std::vector{1., 2., 3., 4., 5.}, [](auto x) { return x > 3.; }),
        {4., 5.}));

    REQUIRE(almost_equal(
        filter(std::array{1., 2., 3., 4., 5.}, [](auto x) { return x > 3.; }),
        {4., 5.}));

    REQUIRE(v == std::vector{1., 2., 3., 4., 5.});

    REQUIRE(almost_equal(
        filter(arange(0., 20.), filters::Trim<double>({3., 17.}, {true, true})),
        arange(3., 18.)));
}

TEST_CASE("filter_reduce") {
    const auto [res, cnt] =
        filter_reduce(std::array{1, 2, 3, 4, 5}, std::plus<>(), 0, [](auto v) {
            return v % 2 == 0;
        });

    REQUIRE(res == 6);
    REQUIRE(cnt == 2);

    REQUIRE(std::get<0>(
                filter_reduce(std::vector<int>{}, std::plus<>(), 1, [](auto v) {
                    return v % 2 == 0;
                })) == 1);
}

TEST_CASE("reduce") {
    REQUIRE(std::get<0>(reduce(std::array{1, 2, 3},
                               [](auto r, auto v) { return r + v * v; },
                               0)) == 14);
    static_assert(std::get<0>(reduce(std::array{1, 2, 3},
                                     [](auto r, auto v) { return r + v * v; },
                                     0)) == 14);
}

TEST_CASE("cumacc") {
    REQUIRE(cumacc(std::vector{-1, 1, 2, 3}, std::plus<>(), [](auto x) {
                return x > 0;
            }) == std::vector{1, 3, 6});
}

} // namespace scicpp