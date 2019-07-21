// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "numeric.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units.hpp"

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
    const auto [res, cnt] = nansum(
        std::vector{1., 2., 3., std::numeric_limits<double>::quiet_NaN()});
    REQUIRE(almost_equal(res, 6.));
    REQUIRE(cnt == 3);
    const auto v = std::vector(500000, 0.1);
    REQUIRE(std::fabs(sum(v) - double(v.size()) / 10.) < 1E-10);
}

TEST_CASE("sum physical quantities") {
    using namespace units::literals;
    REQUIRE(almost_equal(sum(std::array{1._m, 2._m, 3.141_m}), 6.141_m));
    REQUIRE(almost_equal(sum(std::vector{1._kg, 2._kg, 3._kg}), 6._kg));
}

TEST_CASE("prod") {
    REQUIRE(almost_equal(prod(std::array<double, 0>{}), 1.));
    REQUIRE(almost_equal(prod(std::array{1., 2., 3.141}), 6.282));
    REQUIRE(almost_equal(prod(std::array{1., 2., 3.}), 6.));
    REQUIRE(almost_equal(prod(std::vector{1., 2., 3.}), 6.));
    const auto [res, cnt] = nanprod(
        std::vector{1., 2., 3., std::numeric_limits<double>::quiet_NaN()});
    REQUIRE(almost_equal(res, 6.));
    REQUIRE(cnt == 3);
    const auto v = std::vector(500000, 1.);
    REQUIRE(almost_equal(prod(v), 1.));
}

TEST_CASE("cumsum") {
    REQUIRE(cumsum(std::array<double, 0>{}).empty());
    REQUIRE(almost_equal(cumsum(std::array{1., 3., 6., 10., 15., 21.}),
                         {1., 4., 10., 20., 35., 56.}));
    REQUIRE(almost_equal(cumsum(std::vector{1., 3., 6., 10., 15., 21.}),
                         {1., 4., 10., 20., 35., 56.}));
    const std::vector v{1., 3., 6., 10., 15., 21.};
    REQUIRE(almost_equal(cumsum(v), {1., 4., 10., 20., 35., 56.}));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(
        nancumsum(std::vector{1., 3., nan, 6., 10., 15., nan, 21.}),
        {1., 4., 10., 20., 35., 56.}));
}

TEST_CASE("cumsum physical quantities") {
    using namespace units::literals;
    REQUIRE(
        almost_equal(cumsum(std::array{1._m, 3._m, 6._m, 10._m, 15._m, 21._m}),
                     {1._m, 4._m, 10._m, 20._m, 35._m, 56._m}));
    REQUIRE(
        almost_equal(cumsum(std::vector{1._m, 3._m, 6._m, 10._m, 15._m, 21._m}),
                     {1._m, 4._m, 10._m, 20._m, 35._m, 56._m}));
}

TEST_CASE("cumprod") {
    REQUIRE(cumprod(std::array<double, 0>{}).empty());
    REQUIRE(almost_equal(cumprod(std::array{1., 3., 6., 10., 15., 21.}),
                         {1., 3., 18., 180., 2700., 56700.}));
    REQUIRE(almost_equal(cumprod(std::vector{1., 3., 6., 10., 15., 21.}),
                         {1., 3., 18., 180., 2700., 56700.}));
    const std::vector v{1., 3., 6., 10., 15., 21.};
    REQUIRE(almost_equal(cumprod(v), {1., 3., 18., 180., 2700., 56700.}));
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE(almost_equal(
        nancumprod(std::vector{1., nan, 3., 6., nan, 10., 15., nan, 21.}),
        {1., 3., 18., 180., 2700., 56700.}));
}

TEST_CASE("trapz") {
    REQUIRE(almost_equal(trapz(std::array<double, 0>{}, 1.), 0.));
    REQUIRE(almost_equal(trapz(std::array{1., 2., 3.}, 1.), 4.));
    REQUIRE(almost_equal(trapz(std::vector{1., 2., 3.}, 1.), 4.));
    REQUIRE(almost_equal(trapz(std::vector{1., 2., 3.}, 1.F), 4.));
}

TEST_CASE("trapz physical quantity") {
    using namespace units::literals;
    REQUIRE(almost_equal(trapz(std::array<units::length<double>, 0>{}, 1._m),
                         0._m2));
    REQUIRE(almost_equal(trapz(std::array{1._m, 2._m, 3._m}, 1._m), 4._m2));
    REQUIRE(almost_equal(trapz(std::vector{1._m, 2._m, 3._m}, 1.), 4._m));
    REQUIRE(almost_equal(trapz(std::array{1._m2, 2._m2, 3._m2}, 1._m), 4._m3));
    REQUIRE(almost_equal(trapz(std::vector{1._V, 2._V, 3._V}, 1._mA), 4._mW));
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

TEST_CASE("diff physical quantity") {
    using namespace units::literals;
    REQUIRE(almost_equal(diff(std::vector{1._m, 2._m, 4._m, 7._m, 0._m}),
                         {1._m, 2._m, 3._m, -7._m}));
    REQUIRE(almost_equal(diff(std::array{1._m, 2._m, 4._m, 7._m, 0._m}),
                         {1._m, 2._m, 3._m, -7._m}));
}

TEST_CASE("inner") {
    using namespace scicpp::operators;

    static_assert(inner(std::array{1, 2, 4, 7}, std::array{1, 2, 3, -7}) ==
                  -32);
    REQUIRE(inner(std::array{1, 2, 4, 7}, std::array{1, 2, 3, -7}) == -32);
    REQUIRE(inner(std::array{1, 2, 4, 7}, std::vector{1, 2, 3, -7}) == -32);
    REQUIRE(inner(std::vector{1, 2, 4, 7}, std::vector{1, 2, 3, -7}) == -32);
    REQUIRE(dot(std::vector{1, 2, 4, 7}, std::vector{1, 2, 3, -7}) == -32);
    REQUIRE(dot(std::vector{1, 2, 4, 7}, std::array{1, 2, 3, -7}) == -32);
    // printf("%.20f\n",
    //        inner(linspace(0., 1253., 1000000), linspace(0., 148253., 1000000)));
    REQUIRE(almost_equal<2>(
        inner(linspace(0., 1253., 1000000), linspace(0., 148253., 1000000)),
        61920367293532.47));
    REQUIRE(vdot(std::array{1, 2, 4, 7}, std::array{1, 2, 3, -7}) == -32);
    REQUIRE(vdot(std::array{1. + 0.i, 2.i, 4. + 0.i, 7.i},
                 std::array{1., 2., 3., -7.}) == 13. + 45.i);
    REQUIRE(vdot(std::array{1., 2., 3., -7.},
                 std::array{1. + 0.i, 2.i, 4. + 0.i, 7.i}) == 13. - 45.i);
    const auto p = vdot(linspace(0., 87946., 1000000) * (3.14 + 2.718i),
                        linspace(0., 156., 1000000));
    // printf("%.20f, %.20f\n", p.real(), p.imag());
    REQUIRE(almost_equal<10>(p, 14359830059918.605 - 12429942070974.137i));
}

TEST_CASE("inner physical quantity") {
    using namespace units::literals;
    REQUIRE(almost_equal(inner(std::array{1._m, 2._m, 4._m, 7._m},
                               std::array{1._m, 2._m, 3._m, -7._m}),
                         -32._m2));
    REQUIRE(almost_equal(
        inner(std::array{1._m, 2._m, 4._m, 7._m}, std::vector{1., 2., 3., -7.}),
        -32._m));
    REQUIRE(almost_equal(inner(std::array{1._V, 2._V, 4._V, 7._V},
                               std::array{1._A, 2._A, 3._A, -7._A}),
                         -32._W));
    REQUIRE(almost_equal<2>(inner(linspace(0._V, 1253._V, 1000000),
                                  linspace(0._A, 148253._A, 1000000)),
                            61920367293532.47_W));
    REQUIRE(almost_equal(dot(std::array{1._m, 2._m, 4._m, 7._m},
                             std::array{1._m, 2._m, 3._m, -7._m}),
                         -32._m2));
    REQUIRE(almost_equal(vdot(std::array{1._m, 2._m, 4._m, 7._m},
                              std::array{1._m, 2._m, 3._m, -7._m}),
                         -32._m2));
}

TEST_CASE("Logical/comparison operators") {
    using namespace operators;

    const std::array b1a{1, 0, 1};
    const std::vector b1v{1, 0, 1};
    const std::array b2a{1, 1, 0};
    const std::vector b2v{1, 1, 0};

    const std::array a1{1, 2, 3};
    const std::array a2{-1, 4, 3};
    const std::vector v1{1, 2, 3};
    const std::vector v2{-1, 4, 3};

    REQUIRE(array_equal(!b1a, {0, 1, 0}));
    REQUIRE(array_equal(!b1v, {0, 1, 0}));

    REQUIRE(array_equal(a1 == 1, {1, 0, 0}));
    REQUIRE(array_equal(v1 == 1, {1, 0, 0}));
    REQUIRE(array_equal(1 == a1, {1, 0, 0}));
    REQUIRE(array_equal(1 == v1, {1, 0, 0}));

    REQUIRE(array_equal(a2 != 4, {1, 0, 1}));
    REQUIRE(array_equal(v2 != 4, {1, 0, 1}));
    REQUIRE(array_equal(4 != a2, {1, 0, 1}));
    REQUIRE(array_equal(4 != v2, {1, 0, 1}));

    REQUIRE(array_equal(a2 < 0, {1, 0, 0}));
    REQUIRE(array_equal(v2 < 0, {1, 0, 0}));
    REQUIRE(array_equal(1 < a1, {0, 1, 1}));
    REQUIRE(array_equal(1 < v1, {0, 1, 1}));

    REQUIRE(array_equal(a2 >= 3, {0, 1, 1}));
    REQUIRE(array_equal(v2 >= 3, {0, 1, 1}));
    REQUIRE(array_equal(1 >= a1, {1, 0, 0}));
    REQUIRE(array_equal(1 >= v1, {1, 0, 0}));

    REQUIRE(array_equal(a2 <= 3, {1, 0, 1}));
    REQUIRE(array_equal(v2 <= 3, {1, 0, 1}));
    REQUIRE(array_equal(1 <= a1, {1, 1, 1}));
    REQUIRE(array_equal(1 <= v1, {1, 1, 1}));

    REQUIRE(array_equal(a2 > 3, {0, 1, 0}));
    REQUIRE(array_equal(v2 > 3, {0, 1, 0}));
    REQUIRE(array_equal(1 > a1, {0, 0, 0}));
    REQUIRE(array_equal(1 > v1, {0, 0, 0}));

    REQUIRE(array_equal(b1a && b2a, {1, 0, 0}));
    REQUIRE(array_equal(b1a || b2a, {1, 1, 1}));
}

TEST_CASE("Arithmetic operators") {
    using namespace operators;

    const std::array a{1., 2., 3.};
    const std::array b{1, 0, 1};
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
    REQUIRE(almost_equal(a * 2.i, {2.i, 4.i, 6.i}));
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
    REQUIRE(almost_equal(v1 * 2.i * v, {4.i, 16.i, 36.i}));

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

TEST_CASE("Arithmetic operators physical quantity") {
    using namespace operators;
    using namespace units::literals;

    const std::array a{1._m, 2._m, 3._m};
    const std::array t{1._s, 2._s, 3._s};
    const std::vector v{1._m, 2._m, 3._m};

    const std::array a1{2._m, 4._m, 6._m};
    const std::array b1{2., 4., 6.};
    const std::vector v1{2._m, 4._m, 6._m};

    REQUIRE(almost_equal(-a, {-1._m, -2._m, -3._m}));
    REQUIRE(almost_equal(-v, {-1._m, -2._m, -3._m}));

    REQUIRE(almost_equal(2. * a, {2._m, 4._m, 6._m}));
    REQUIRE(almost_equal(2._m * a, {2._m2, 4._m2, 6._m2}));
    REQUIRE(almost_equal(a * 2., {2._m, 4._m, 6._m}));
    REQUIRE(almost_equal(a * 2._m, {2._m2, 4._m2, 6._m2}));
    REQUIRE(almost_equal(2. * v, {2._m, 4._m, 6._m}));
    REQUIRE(almost_equal(2._m * v, {2._m2, 4._m2, 6._m2}));
    REQUIRE(almost_equal(v * 2., {2._m, 4._m, 6._m}));
    REQUIRE(almost_equal(v * 2._m, {2._m2, 4._m2, 6._m2}));
    REQUIRE(almost_equal(1._m * b1, {2._m, 4._m, 6._m}));
    REQUIRE(almost_equal(b1 * 1._m, {2._m, 4._m, 6._m}));

    REQUIRE(almost_equal(2._m + a, {3._m, 4._m, 5._m}));
    REQUIRE(almost_equal(a + 2._m, {3._m, 4._m, 5._m}));
    REQUIRE(almost_equal(2._m + v, {3._m, 4._m, 5._m}));
    REQUIRE(almost_equal(v + 2._m, {3._m, 4._m, 5._m}));

    REQUIRE(almost_equal(2._m - a, {1._m, 0._m, -1._m}));
    REQUIRE(almost_equal(a - 2._m, {-1._m, 0._m, 1._m}));
    REQUIRE(almost_equal(2._m - v, {1._m, 0._m, -1._m}));
    REQUIRE(almost_equal(v - 2._m, {-1._m, 0._m, 1._m}));

    REQUIRE(almost_equal(3._m2 / a, {3._m, 1.5_m, 1._m}));
    REQUIRE(almost_equal(3. / t, {3._Hz, 1.5_Hz, 1._Hz}));
    REQUIRE(almost_equal(a / 2., {0.5_m, 1._m, 1.5_m}));
    REQUIRE(almost_equal(a / 2._s, {0.5_m_per_s, 1._m_per_s, 1.5_m_per_s}));
    REQUIRE(almost_equal(b1 / 1._s, {2._Hz, 4._Hz, 6._Hz}));
    REQUIRE(almost_equal<1500>(6._s / b1, {3._s, 1.5_s, 1._s}));

    REQUIRE(almost_equal(2_m % a, {0_m, 0_m, 2_m}));
    REQUIRE(almost_equal(a % 2_m, {1_m, 0_m, 1_m}));

    REQUIRE(almost_equal(a1 * a, {2._m2, 8._m2, 18._m2}));
    REQUIRE(almost_equal(a * b1, {2._m, 8._m, 18._m}));
    REQUIRE(almost_equal(b1 * a, {2._m, 8._m, 18._m}));
    REQUIRE(almost_equal(v1 * v, {2._m2, 8._m2, 18._m2}));

    REQUIRE(almost_equal(a1 + a, {3._m, 6._m, 9._m}));
    REQUIRE(almost_equal(v1 + v, {3._m, 6._m, 9._m}));
    REQUIRE(almost_equal(a1 + v, {3._m, 6._m, 9._m}));

    REQUIRE(almost_equal(a1 - a, {1._m, 2._m, 3._m}));
    REQUIRE(almost_equal(v1 - v, {1._m, 2._m, 3._m}));

    REQUIRE(almost_equal(a1 / t, {2._m_per_s, 2._m_per_s, 2._m_per_s}));
    REQUIRE(almost_equal(v1 / t, {2._m_per_s, 2._m_per_s, 2._m_per_s}));

    REQUIRE(almost_equal(a1 % a, {0_m, 0_m, 0_m}));
    REQUIRE(almost_equal(v1 % v, {0_m, 0_m, 0_m}));
}

TEST_CASE("mask") {
    const std::array a{1., 2., 3.};
    const std::vector v{1., 2., 3.};

    const std::array m1{0, 1, 0};
    const std::vector m2{1, 0, 1};

    REQUIRE(almost_equal(mask(a, m2), {1., 3.}));
    REQUIRE(almost_equal(mask(v, m1), {2.}));
    REQUIRE(almost_equal(mask(std::vector{1., 2., 3.}, m1), {2.}));
}

TEST_CASE("mask physical quantity") {
    using namespace units::literals;

    const std::array a{1._m, 2._m, 3._m};
    const std::vector v{1._m, 2._m, 3._m};

    const std::array m1{0, 1, 0};
    const std::vector m2{1, 0, 1};

    REQUIRE(almost_equal(mask(a, m2), {1._m, 3._m}));
    REQUIRE(almost_equal(mask(v, m1), {2._m}));
    REQUIRE(almost_equal(mask(std::vector{1._m, 2._m, 3._m}, m1), {2._m}));
}

} // namespace scicpp
