// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "numeric.hpp"

#include "scicpp/core/print.hpp"

namespace scicpp {

namespace numeric_constexpr_tests {

constexpr std::array a{1., 2., 3.};
static_assert(float_equal(sum(a), 6.));
static_assert(float_equal(prod(a), 6.));
static_assert(float_equal(trapz(a, 1.), 4.));

} // namespace numeric_constexpr_tests

TEST_CASE("sum") {
    REQUIRE(almost_equal(sum(std::array<double, 0>{}), 0.));
    REQUIRE(almost_equal(sum(std::array{1., 2., 3.141}), 6.141));
    REQUIRE(almost_equal(sum(std::array{1., 2., 3.}), 6.));
    REQUIRE(almost_equal(sum(std::vector{1., 2., 3.}), 6.));
    REQUIRE(
        almost_equal(nansum(std::vector{
                         1., 2., 3., std::numeric_limits<double>::quiet_NaN()}),
                     6.));
}

TEST_CASE("prod") {
    REQUIRE(almost_equal(prod(std::array<double, 0>{}), 1.));
    REQUIRE(almost_equal(prod(std::array{1., 2., 3.141}), 6.282));
    REQUIRE(almost_equal(prod(std::array{1., 2., 3.}), 6.));
    REQUIRE(almost_equal(prod(std::vector{1., 2., 3.}), 6.));
    REQUIRE(
        almost_equal(nanprod(std::vector{
                         1., 2., 3., std::numeric_limits<double>::quiet_NaN()}),
                     6.));
}

TEST_CASE("cumsum") {
    REQUIRE(cumsum(std::array<double, 0>{}).empty());
    REQUIRE(almost_equal(cumsum(std::array{1., 3., 6., 10., 15., 21.}),
                         {1., 4., 10., 20., 35., 56.}));
    REQUIRE(almost_equal(cumsum(std::vector{1., 3., 6., 10., 15., 21.}),
                         {1., 4., 10., 20., 35., 56.}));
}

TEST_CASE("trapz") {
    REQUIRE(almost_equal(trapz(std::array<double, 0>{}, 1.), 0.));
    REQUIRE(almost_equal(trapz(std::array{1., 2., 3.}, 1.), 4.));
    REQUIRE(almost_equal(trapz(std::vector{1., 2., 3.}, 1.), 4.));
}

TEST_CASE("diff") {
    REQUIRE(diff(std::array<double, 0>{}).empty());
    REQUIRE(diff(std::array{1.}).empty());
    REQUIRE(almost_equal(diff<0>(std::array{1., 2., 4., 7., 0.}),
                         {1., 2., 4., 7., 0.}));
    REQUIRE(
        almost_equal(diff(std::array{1., 2., 4., 7., 0.}), {1., 2., 3., -7.}));
    REQUIRE(
        almost_equal(diff<2>(std::array{1., 2., 4., 7., 0.}), {1., 1., -10.}));
    REQUIRE(diff(std::vector<double>{}).empty());
    REQUIRE(diff(std::vector{1.}).empty());
    // print(diff(std::vector{1., 2., 4., 7., 0.}));
    REQUIRE(
        almost_equal(diff(std::vector{1., 2., 4., 7., 0.}), {1., 2., 3., -7.}));
    // print(diff(std::vector{1., 2., 4., 7., 0.}, 2));
    REQUIRE(
        almost_equal(diff(std::vector{1., 2., 4., 7., 0.}, 2), {1., 1., -10.}));
    const std::vector v{1., 2., 4., 7., 0.};
    REQUIRE(almost_equal(diff(v, 2), {1., 1., -10.}));
}

TEST_CASE("Arithmetic operators") {
    using namespace operators;

    const std::array a{1., 2., 3.};
    const std::array ai{1, 2, 3};
    const std::vector v{1., 2., 3.};

    const std::array a1{2., 4., 6.};
    const std::array a1i{2, 4, 6};
    const std::vector v1{2., 4., 6.};

    REQUIRE(almost_equal(-a, {-1., -2., -3.}));
    REQUIRE(almost_equal(-ai, {-1, -2, -3}));
    REQUIRE(almost_equal(-v, {-1., -2., -3.}));
    REQUIRE(almost_equal(2. * a, {2., 4., 6.}));
    REQUIRE(almost_equal(a * 2., {2., 4., 6.}));
    REQUIRE(almost_equal(2 * ai, {2, 4, 6}));
    REQUIRE(almost_equal(ai * 2, {2, 4, 6}));
    REQUIRE(almost_equal(2. * v, {2., 4., 6.}));
    REQUIRE(almost_equal(v * 2., {2., 4., 6.}));
    REQUIRE(almost_equal(v * 2.i, {2.i, 4.i, 6.i}));

    REQUIRE(almost_equal(2. + a, {3., 4., 5.}));
    REQUIRE(almost_equal(2.i + a, {2.i + 1., 2.i + 2., 2.i + 3.}));
    REQUIRE(almost_equal(a + 2., {3., 4., 5.}));
    REQUIRE(almost_equal(2 + ai, {3, 4, 5}));
    REQUIRE(almost_equal(ai + 2, {3, 4, 5}));
    REQUIRE(almost_equal(2. + v, {3., 4., 5.}));
    REQUIRE(almost_equal(v + 2., {3., 4., 5.}));

    REQUIRE(almost_equal(2. - a, {1., 0., -1.}));
    REQUIRE(almost_equal(a - 2., {-1., 0., 1.}));
    REQUIRE(almost_equal(2 - ai, {1, 0, -1}));
    REQUIRE(almost_equal(ai - 2, {-1, 0, 1}));
    REQUIRE(almost_equal(2. - v, {1., 0., -1.}));
    REQUIRE(almost_equal(v - 2., {-1., 0., 1.}));

    REQUIRE(almost_equal(2. / a, {2., 1., 2. / 3.}));
    REQUIRE(almost_equal(a / 2., {0.5, 1., 3. / 2.}));
    REQUIRE(almost_equal(2 / ai, {2, 1, 0}));
    REQUIRE(almost_equal(ai / 2, {0, 1, 1}));
    REQUIRE(almost_equal(2. / v, {2., 1., 2. / 3.}));
    REQUIRE(almost_equal(v / 2., {0.5, 1., 3. / 2.}));
    REQUIRE(almost_equal(v / 2.i, {-0.5i, -1.i, -3.i / 2.}));

    REQUIRE(almost_equal(2. % a, {0., 0., 2.}));
    REQUIRE(almost_equal(a % 2., {1., 0., 1.}));
    REQUIRE(almost_equal(2 % ai, {0, 0, 2}));
    REQUIRE(almost_equal(ai % 2, {1, 0, 1}));
    REQUIRE(almost_equal(2. % v, {0., 0., 2.}));
    REQUIRE(almost_equal(v % 2., {1., 0., 1.}));

    REQUIRE(almost_equal(a1 * a, {2., 8., 18.}));
    REQUIRE(almost_equal(a1i * ai, {2, 8, 18}));
    REQUIRE(almost_equal(v1 * v, {2., 8., 18.}));

    REQUIRE(almost_equal(a1 + a, {3., 6., 9.}));
    REQUIRE(almost_equal(a1i + ai, {3, 6, 9}));
    REQUIRE(almost_equal(v1 + v, {3., 6., 9.}));

    REQUIRE(almost_equal(a1 - a, {1., 2., 3.}));
    REQUIRE(almost_equal(a1i - ai, {1, 2, 3}));
    REQUIRE(almost_equal(v1 - v, {1., 2., 3.}));

    REQUIRE(almost_equal(a1 / a, {2., 2., 2.}));
    REQUIRE(almost_equal(a1i / ai, {2, 2, 2}));
    REQUIRE(almost_equal(v1 / v, {2., 2., 2.}));

    REQUIRE(almost_equal(a1 % a, {0., 0., 0.}));
    REQUIRE(almost_equal(a1i % ai, {0, 0, 0}));
    REQUIRE(almost_equal(v1 % v, {0., 0., 0.}));
}

TEST_CASE("almost_equal real") {
    SECTION("NaNs") {
        constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
        REQUIRE(almost_equal(nan, nan));
    }

    SECTION("Inf") {
        constexpr auto inf = std::numeric_limits<double>::infinity();
        REQUIRE(almost_equal(inf, inf));
        REQUIRE(!almost_equal(inf, -inf));
    }

    SECTION("near zero") {
        REQUIRE(almost_equal(0., 0.00000000000000011102));
        REQUIRE(almost_equal(0.00000000000000011102, 0.));
        REQUIRE(almost_equal(0.00000000000000011102, 0.00000000000000011102));
    }

    SECTION("std::array") {
        const std::array f1{1., 2., 3.141};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }

    SECTION("std::vector") {
        const std::vector f1{1., 2., 3.141};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));

        const std::vector f3{1., 2.};
        REQUIRE(!almost_equal(f1, f3));

        const std::vector f4{1., 2.1, 3.141};
        REQUIRE(!almost_equal(f1, f4));
    }
}

TEST_CASE("almost_equal complex") {
    SECTION("scalar") {
        constexpr auto eps = std::numeric_limits<double>::epsilon();
        const auto z1 = 3.141 + 42.i;
        const auto z2 = 3.141 + 2. * eps + 42.i;
        REQUIRE(almost_equal(z1, z1));
        REQUIRE(!almost_equal(z1, z2));
        REQUIRE(almost_equal<2>(z1, z2));
    }

    SECTION("std::array") {
        const std::array f1{1. + 2.i, 3.141 + 42.i};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }

    SECTION("std::vector") {
        const std::vector f1{1. + 2.i, 3.141 + 42.i};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }
}

} // namespace scicpp
