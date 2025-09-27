// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "numeric.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units/units.hpp"

namespace scicpp {

namespace numeric_constexpr_tests {

constexpr std::array a{1., 2., 3.};
static_assert(float_equal(sum(a), 6.));
static_assert(float_equal(prod(a), 6.));
static_assert(float_equal(trapz(a, 1.), 4.));

} // namespace numeric_constexpr_tests

//---------------------------------------------------------------------------------
// sum
//---------------------------------------------------------------------------------

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

TEST_CASE("sum with std::span") {
    SECTION("span over std::array (static extent)") {
        constexpr std::array<double, 3> a{1., 2., 3.};
        std::span<const double, 3> s{a};
        REQUIRE(almost_equal(sum(s), 6.));
    }

    SECTION("span over std::vector (dynamic extent)") {
        std::vector<double> v{1., 2., 3.};
        std::span<const double> s{v};
        REQUIRE(almost_equal(sum(s), 6.));
    }

    SECTION("subspan") {
        std::array<double, 5> a{0., 1., 2., 3., 4.};
        std::span<const double> s{a};
        auto mid = s.subspan(1, 3); // 1,2,3
        REQUIRE(almost_equal(sum(mid), 6.));
    }

    SECTION("empty span") {
        std::span<const double> s{}; // size 0
        REQUIRE(almost_equal(sum(s), 0.));
    }

    SECTION("nansum over span") {
        std::vector<double> v{
            1., 2., 3., std::numeric_limits<double>::quiet_NaN()};
        std::span<const double> s{v};
        const auto [res, cnt] = nansum(s);
        REQUIRE(almost_equal(res, 6.));
        REQUIRE(cnt == 3);
    }

    SECTION("large span") {
        const auto v = std::vector(500000, 0.1);
        std::span<const double> s{v};
        REQUIRE(std::fabs(sum(s) - double(v.size()) / 10.) < 1E-10);
    }
}

TEST_CASE("sum physical quantities") {
    using namespace units::literals;
    static_assert(float_equal(sum(std::array{1._m, 2._m, 3.141_m}), 6.141_m));
    REQUIRE(almost_equal(sum(std::vector{1._kg, 2._kg, 3._kg}), 6._kg));
}

TEST_CASE("sum physical quantities with std::span") {
    using namespace units::literals;

    SECTION("array -> span") {
        std::array a{1._m, 2._m, 3._m};
        auto s = std::span{a}; // CTAD: span<const meter_type, 3>
        REQUIRE(almost_equal(sum(s), 6._m));
    }

    SECTION("vector -> span") {
        std::vector v{1._kg, 2._kg, 3._kg};
        auto s = std::span{v}; // span<const kilogram_type>
        REQUIRE(almost_equal(sum(s), 6._kg));
    }

    SECTION("subspan with units") {
        std::array a{0._m, 1._m, 2._m, 3._m};
        auto s = std::span{a}.subspan(1, 2); // 1 m, 2 m
        REQUIRE(almost_equal(sum(s), 3._m));
    }
}

//---------------------------------------------------------------------------------
// prod
//---------------------------------------------------------------------------------

TEST_CASE("prod") {
    static_assert(float_equal(prod(std::array<double, 0>{}), 1.));
    static_assert(float_equal(prod(std::array{1., 2., 3.141}), 6.282));
    static_assert(float_equal(prod(std::array{1., 2., 3.}), 6.));

    REQUIRE(almost_equal(prod(std::vector{1., 2., 3.}), 6.));

    const auto [res, cnt] = nanprod(
        std::vector{1., 2., 3., std::numeric_limits<double>::quiet_NaN()});
    REQUIRE(almost_equal(res, 6.));
    REQUIRE(cnt == 3);

    const auto v = std::vector(500000, 1.);
    REQUIRE(almost_equal(prod(v), 1.));

    // --- span-specific checks ---
    SECTION("span over vector (dynamic extent)") {
        std::vector<double> w{1., 2., 3.};
        std::span<const double> s{w};
        REQUIRE(almost_equal(prod(s), 6.));
    }

    SECTION("span over array (static extent)") {
        constexpr std::array<double, 3> a{1., 2., 3.};
        static_assert(float_equal(prod(a), 6.));
        std::span<const double, 3> s{a};
        REQUIRE(almost_equal(prod(s), 6.));
    }

    SECTION("subspan") {
        std::array<double, 5> a{0., 1., 2., 3., 4.};
        auto s = std::span{a}.subspan(1, 3); // 1*2*3
        REQUIRE(almost_equal(prod(s), 6.));
    }

    SECTION("empty span") {
        std::span<const double> s{};
        REQUIRE(almost_equal(prod(s), 1.));
    }

    SECTION("nanprod over span") {
        std::vector<double> w{
            1., 2., 0., std::numeric_limits<double>::quiet_NaN()};
        std::span<const double> s{w};
        const auto [p, n] = nanprod(s);
        REQUIRE(almost_equal(p, 0.)); // 1*2*0 = 0
        REQUIRE(n == 3);
    }
}

//---------------------------------------------------------------------------------
// cumsum
//---------------------------------------------------------------------------------

TEST_CASE("cumsum") {
    static_assert(cumsum(std::array<double, 0>{}).empty());
    static_assert(array_equal(cumsum(std::array{1, 3, 6, 10, 15, 21}),
                              {1, 4, 10, 20, 35, 56}));
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

TEST_CASE("cumsum with std::span") {
    SECTION("non-const span (in-place)") {
        std::array<double, 6> a{1., 3., 6., 10., 15., 21.};
        auto s = std::span<double>{a};
        auto r = cumsum(s); // r is std::span<double>

        const std::array<double, 6> expected{1., 4., 10., 20., 35., 56.};

        REQUIRE(almost_equal<1>(std::span<const double>(r),
                                std::span<const double>(expected)));
    }

    SECTION("const span (returns vector copy)") {
        const std::array<double, 6> a{1., 3., 6., 10., 15., 21.};
        auto s = std::span<const double>{a};
        auto v = cumsum(s); // std::vector<double>
        REQUIRE(almost_equal(v, std::vector{1., 4., 10., 20., 35., 56.}));
    }

    SECTION("dynamic-extent span over vector") {
        std::vector<double> v{1., 3., 6., 10., 15., 21.};
        auto s = std::span<double>{v};
        (void)cumsum(s);
        REQUIRE(almost_equal(v, std::vector{1., 4., 10., 20., 35., 56.}));
    }
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

//---------------------------------------------------------------------------------
// cumprod
//---------------------------------------------------------------------------------

TEST_CASE("cumprod") {
    static_assert(cumprod(std::array<double, 0>{}).empty());
    static_assert(array_equal(cumprod(std::array{1, 3, 6, 10, 15, 21}),
                              {1, 3, 18, 180, 2700, 56700}));
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

TEST_CASE("cumprod with std::span") {
    SECTION("span over array (in-place)") {
        std::array<double, 6> a{1., 3., 6., 10., 15., 21.};
        std::span<double> s{a};

        auto r = cumprod(s); // mutates 'a' through the span

        const std::array<double, 6> expected{1., 3., 18., 180., 2700., 56700.};

        // strict same-type comparison: span<const double> vs span<const double>
        REQUIRE(almost_equal(std::span<const double>(r),
                             std::span<const double>(expected)));

        // and the underlying array is updated as well
        REQUIRE(almost_equal(std::span<const double>(a),
                             std::span<const double>(expected)));
    }

    SECTION("const span over array (returns vector copy)") {
        const std::array<double, 6> a{1., 3., 6., 10., 15., 21.};
        std::span<const double> s{a};

        auto out = cumprod(s); // expected to be std::vector<double>

        REQUIRE(
            almost_equal(out, std::vector{1., 3., 18., 180., 2700., 56700.}));
    }

    SECTION("span over vector (in-place)") {
        std::vector<double> v{1., 3., 6., 10., 15., 21.};
        std::span<double> s{v};

        auto r = cumprod(s);

        const std::vector<double> expected{1., 3., 18., 180., 2700., 56700.};

        // compare as spans for strict same-type
        REQUIRE(almost_equal(std::span<const double>(r),
                             std::span<const double>(expected)));

        // and vector mutated accordingly
        REQUIRE(almost_equal(v, expected));
    }

    SECTION("subspan (in-place on slice)") {
        std::array<double, 6> a{1., 3., 6., 10., 15., 21.};
        auto sub = std::span<double>{a}.subspan(1, 3); // {3, 6, 10}

        auto r = cumprod(sub); // only the slice is cumulatively multiplied

        const std::array<double, 3> sub_expected{3., 18., 180.};
        REQUIRE(almost_equal(std::span<const double>(r),
                             std::span<const double>(sub_expected)));

        const std::array<double, 6> full_expected{1., 3., 18., 180., 15., 21.};
        REQUIRE(almost_equal(std::span<const double>(a),
                             std::span<const double>(full_expected)));
    }

    SECTION("empty spans") {
        // non-const empty span (in-place): just check size
        std::array<double, 0> a{};
        std::span<double> s{a};
        auto r = cumprod(s);
        REQUIRE(r.empty());

        // const empty span: result vector should be empty
        std::span<const double> sc{};
        auto out = cumprod(sc);
        REQUIRE(out.empty());
    }

    SECTION("nancumprod over span<const double>") {
        const auto nan = std::numeric_limits<double>::quiet_NaN();
        std::vector<double> v{1., nan, 3., 6., nan, 10., 15., nan, 21.};
        std::span<const double> s{v};

        auto out = nancumprod(s); // std::vector<double>, NaNs filtered out
        REQUIRE(
            almost_equal(out, std::vector{1., 3., 18., 180., 2700., 56700.}));
        // (length is number of non-NaNs)
        REQUIRE(out.size() == 6);
    }
}

//---------------------------------------------------------------------------------
// trapz
//---------------------------------------------------------------------------------

TEST_CASE("trapz") {
    REQUIRE(almost_equal(trapz(std::array<double, 0>{}, 1.), 0.));
    REQUIRE(almost_equal(trapz(std::array{1., 2., 3.}, 1.), 4.));
    static_assert(float_equal(trapz(std::array{1., 2., 3.}, 1.), 4.));
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

TEST_CASE("trapz with std::span") {
    SECTION("span over array (dynamic extent)") {
        const std::array<double, 3> a{1., 2., 3.};
        std::span<const double> s{a};
        REQUIRE(almost_equal(trapz(s, 1.), 4.));
    }

    SECTION("span over vector (dynamic extent)") {
        const std::vector<double> v{1., 2., 3.};
        std::span<const double> s{v};
        REQUIRE(almost_equal(trapz(s, 1.), 4.));
        REQUIRE(
            almost_equal(trapz(s, 1.f), 4.)); // float dx → return still double
    }

    SECTION("empty span and single-element span") {
        std::span<const double> s_empty{};
        REQUIRE(almost_equal(trapz(s_empty, 1.), 0.));

        const std::array<double, 1> a1{42.};
        std::span<const double> s1{a1};
        REQUIRE(almost_equal(trapz(s1, 1.), 0.)); // N < 2 → 0
    }

    SECTION("subspan") {
        const std::array<double, 5> a{0., 1., 2., 3., 0.};
        auto s = std::span<const double>{a}.subspan(1, 3); // {1,2,3}
        REQUIRE(
            almost_equal(trapz(s, 0.5), 2.0)); // 0.5 * (1 + 2*2 + 3) * 0.5 = 2
    }
}

TEST_CASE("trapz with iterator pairs (shim)") {
    const std::vector<double> v{1., 2., 3.};
    REQUIRE(almost_equal(trapz(v.cbegin(), v.cend(), 1.), 4.));
    REQUIRE(almost_equal(trapz(v.cbegin(), v.cend(), 0.5), 2.));
}

TEST_CASE("trapz additional scalars") {
    // N < 2 edge cases
    REQUIRE(almost_equal(trapz(std::vector<double>{}, 1.), 0.));
    REQUIRE(almost_equal(trapz(std::array{42.}, 1.), 0.));

    // different dx types
    REQUIRE(almost_equal(trapz(std::array{1., 2., 3.}, 0.5), 2.));
    REQUIRE(almost_equal(trapz(std::array{1., 2., 3.}, 2), 8.));
}

TEST_CASE("trapz physical quantity - span & iterator pairs") {
    using namespace units::literals;

    SECTION("length x length > area") {
        const std::array a{1._m, 2._m, 3._m};
        std::span<const decltype(1._m)> s{a};
        REQUIRE(almost_equal(trapz(s, 1._m), 4._m2));
        REQUIRE(almost_equal(trapz(a.cbegin(), a.cend(), 1._m), 4._m2));
    }

    SECTION("area x length => volume") {
        const std::array a{1._m2, 2._m2, 3._m2};
        std::span<const decltype(1._m2)> s{a};
        REQUIRE(almost_equal(trapz(s, 1._m), 4._m3));
    }

    SECTION("volt x milliampere => milliwatt") {
        const std::vector v{1._V, 2._V, 3._V};
        std::span<const decltype(1._V)> s{v};
        REQUIRE(almost_equal(trapz(s, 1._mA), 4._mW));
        REQUIRE(almost_equal(trapz(v.cbegin(), v.cend(), 1._mA), 4._mW));
    }
}

static_assert(float_equal(trapz(std::array<double, 0>{}, 1.), 0.));
static_assert(float_equal(trapz(std::array{1., 2., 3.}, 1.), 4.));
static_assert(float_equal(trapz(std::array{1., 2., 3.}, 0.5), 2.));

//---------------------------------------------------------------------------------
// diff
//---------------------------------------------------------------------------------

TEST_CASE("diff") {
    static_assert(diff(std::array<double, 0>{}).empty());
    static_assert(diff(std::array{1.}).empty());

    static_assert(
        array_equal(diff<0>(std::array{1, 2, 4, 7, 0}), {1, 2, 4, 7, 0}));
    static_assert(array_equal(diff<2>(std::array{1, 2, 4, 7, 0}), {1, 1, -10}));

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
    // print(diff(std::vector{0., 1., 2., 3., 4.5, 5.}));
    REQUIRE(almost_equal(diff(std::vector{0., 1., 2., 3., 4.5, 5.}),
                         {1., 1., 1., 1.5, 0.5}));
    REQUIRE(almost_equal(diff(std::array{0., 1., 2., 3., 4.5, 5.}),
                         {1., 1., 1., 1.5, 0.5}));
}

TEST_CASE("diff physical quantity") {
    using namespace units::literals;
    REQUIRE(almost_equal(diff(std::vector{1._m, 2._m, 4._m, 7._m, 0._m}),
                         {1._m, 2._m, 3._m, -7._m}));
    REQUIRE(almost_equal(diff(std::array{1._m, 2._m, 4._m, 7._m, 0._m}),
                         {1._m, 2._m, 3._m, -7._m}));
}

//---------------------------------------------------------------------------------
// inner
//---------------------------------------------------------------------------------

TEST_CASE("inner") {
    using namespace scicpp::operators;

    static_assert(inner(std::array{1, 2, 4, 7}, std::array{1, 2, 3, -7}) ==
                  -32);
    REQUIRE(inner(std::array{1, 2, 4, 7}, std::array{1, 2, 3, -7}) == -32);
    REQUIRE(inner(std::array{1, 2, 4, 7}, std::vector{1, 2, 3, -7}) == -32);
    REQUIRE(inner(std::vector{1, 2, 4, 7}, std::vector{1, 2, 3, -7}) == -32);
    const auto v1 = std::vector{56, 1, 2, 4, 7, 8, 12};
    const auto v2 = std::vector{89, 1, 2, 3, -7, 3, 14};
    const auto s1 = std::span{v1}.subspan(1, 4);
    const auto s2 = std::span{v2}.subspan(1, 4);
    REQUIRE(inner(s1, s2) == -32);
    REQUIRE(dot(std::vector{1, 2, 4, 7}, std::vector{1, 2, 3, -7}) == -32);
    REQUIRE(dot(std::vector{1, 2, 4, 7}, std::array{1, 2, 3, -7}) == -32);
    REQUIRE(dot(s1, s2) == -32);
    // printf("%.20f\n",
    //        inner(linspace(0., 1253., 1000000), linspace(0., 148253., 1000000)));
    REQUIRE(almost_equal<2>(
        inner(linspace(0., 1253., 1000000), linspace(0., 148253., 1000000)),
        61920367293532.47));
    REQUIRE(vdot(std::array{1, 2, 4, 7}, std::array{1, 2, 3, -7}) == -32);
    REQUIRE(vdot(s1, s2) == -32);
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

TEST_CASE("Logical/comparison operators constexpr") {
    using namespace operators;

    constexpr std::array b1a{1, 0, 1};
    constexpr std::array b2a{1, 1, 0};
    constexpr std::array a1{1, 2, 3};
    constexpr std::array a2{-1, 4, 3};

    static_assert(array_equal(!b1a, {0, 1, 0}));
    static_assert(array_equal(a1 == 1, {1, 0, 0}));
    static_assert(array_equal(1 == a1, {1, 0, 0}));
    static_assert(array_equal(a2 != 4, {1, 0, 1}));
    static_assert(array_equal(4 != a2, {1, 0, 1}));
    static_assert(array_equal(a2 < 0, {1, 0, 0}));
    static_assert(array_equal(1 < a1, {0, 1, 1}));
    static_assert(array_equal(a2 >= 3, {0, 1, 1}));
    static_assert(array_equal(1 >= a1, {1, 0, 0}));
    static_assert(array_equal(a2 <= 3, {1, 0, 1}));
    static_assert(array_equal(1 <= a1, {1, 1, 1}));
    static_assert(array_equal(a2 > 3, {0, 1, 0}));
    static_assert(array_equal(1 > a1, {0, 0, 0}));
    static_assert(array_equal(b1a && b2a, {1, 0, 0}));
    static_assert(array_equal(b1a || b2a, {1, 1, 1}));
}

TEST_CASE("Logical/Comparison") {
    const std::array b1a{1, 0, 1};
    const std::vector b1v{1, 0, 1};
    const std::array b2a{1, 1, 0};
    const std::vector b2v{1, 1, 0};

    const std::array a1{1, 2, 3};
    const std::array a2{-1, 4, 3};
    const std::vector v1{1, 2, 3};
    const std::vector v2{-1, 4, 3};

    REQUIRE(array_equal(equal(b1a, b2a), {1, 0, 0}));
    REQUIRE(array_equal(not_equal(b1a, b2a), {0, 1, 1}));
    REQUIRE(array_equal(equal(b1v, b2v), {1, 0, 0}));
    REQUIRE(array_equal(not_equal(b1v, b2v), {0, 1, 1}));

    REQUIRE(array_equal(less(a1, a2), {0, 1, 0}));
    REQUIRE(array_equal(less(v1, v2), {0, 1, 0}));

    REQUIRE(array_equal(less_equal(a1, a2), {0, 1, 1}));
    REQUIRE(array_equal(less_equal(v1, v2), {0, 1, 1}));

    REQUIRE(array_equal(greater_equal(a1, a2), {1, 0, 1}));
    REQUIRE(array_equal(greater_equal(v1, v2), {1, 0, 1}));

    REQUIRE(array_equal(greater(a1, a2), {1, 0, 0}));
    REQUIRE(array_equal(greater(v1, v2), {1, 0, 0}));
}

TEST_CASE("Logical/Comparison constexpr") {
    constexpr std::array b1a{1, 0, 1};
    constexpr std::array b2a{1, 1, 0};
    constexpr std::array a1{1, 2, 3};
    constexpr std::array a2{-1, 4, 3};

    static_assert(array_equal(equal(b1a, b2a), {1, 0, 0}));
    static_assert(array_equal(not_equal(b1a, b2a), {0, 1, 1}));
    static_assert(array_equal(less(a1, a2), {0, 1, 0}));
    static_assert(array_equal(less_equal(a1, a2), {0, 1, 1}));
    static_assert(array_equal(greater_equal(a1, a2), {1, 0, 1}));
    static_assert(array_equal(greater(a1, a2), {1, 0, 0}));
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
    REQUIRE(array_equal(-ai, {-1, -2, -3}));
    REQUIRE(almost_equal(-v, {-1., -2., -3.}));
    REQUIRE(almost_equal(2. * a, {2., 4., 6.}));
    REQUIRE(almost_equal(a * 2., {2., 4., 6.}));
    REQUIRE(almost_equal(a * 2.i, {2.i, 4.i, 6.i}));
    REQUIRE(array_equal(2 * ai, {2, 4, 6}));
    REQUIRE(array_equal(ai * 2, {2, 4, 6}));
    REQUIRE(almost_equal(2. * v, {2., 4., 6.}));
    REQUIRE(almost_equal(v * 2., {2., 4., 6.}));
    REQUIRE(almost_equal(v * 2.i, {2.i, 4.i, 6.i}));

    REQUIRE(almost_equal(2. + a, {3., 4., 5.}));
    REQUIRE(almost_equal(2.i + a, {2.i + 1., 2.i + 2., 2.i + 3.}));
    REQUIRE(almost_equal(a + 2., {3., 4., 5.}));
    REQUIRE(array_equal(2 + ai, {3, 4, 5}));
    REQUIRE(array_equal(ai + 2, {3, 4, 5}));
    REQUIRE(almost_equal(2. + v, {3., 4., 5.}));
    REQUIRE(almost_equal(v + 2., {3., 4., 5.}));

    REQUIRE(almost_equal(2. - a, {1., 0., -1.}));
    REQUIRE(almost_equal(a - 2., {-1., 0., 1.}));
    REQUIRE(array_equal(2 - ai, {1, 0, -1}));
    REQUIRE(array_equal(ai - 2, {-1, 0, 1}));
    REQUIRE(almost_equal(2. - v, {1., 0., -1.}));
    REQUIRE(almost_equal(v - 2., {-1., 0., 1.}));

    REQUIRE(almost_equal(2. / a, {2., 1., 2. / 3.}));
    REQUIRE(almost_equal(a / 2., {0.5, 1., 3. / 2.}));
    REQUIRE(array_equal(2 / ai, {2, 1, 0}));
    REQUIRE(array_equal(ai / 2, {0, 1, 1}));
    REQUIRE(almost_equal(2. / v, {2., 1., 2. / 3.}));
    REQUIRE(almost_equal(v / 2., {0.5, 1., 3. / 2.}));
    REQUIRE(almost_equal(v / 2.i, {-0.5i, -1.i, -3.i / 2.}));

    REQUIRE(almost_equal(2. % a, {0., 0., 2.}));
    REQUIRE(almost_equal(a % 2., {1., 0., 1.}));
    REQUIRE(array_equal(2 % ai, {0, 0, 2}));
    REQUIRE(array_equal(ai % 2, {1, 0, 1}));
    REQUIRE(almost_equal(2. % v, {0., 0., 2.}));
    REQUIRE(almost_equal(v % 2., {1., 0., 1.}));

    REQUIRE(almost_equal(a1 * a, {2., 8., 18.}));
    REQUIRE(array_equal(a1i * ai, {2, 8, 18}));
    REQUIRE(almost_equal(v1 * v, {2., 8., 18.}));
    REQUIRE(almost_equal(v1 * 2.i * v, {4.i, 16.i, 36.i}));

    REQUIRE(almost_equal(a1 + a, {3., 6., 9.}));
    REQUIRE(array_equal(a1i + ai, {3, 6, 9}));
    REQUIRE(almost_equal(v1 + v, {3., 6., 9.}));

    REQUIRE(almost_equal(a1 - a, {1., 2., 3.}));
    REQUIRE(array_equal(a1i - ai, {1, 2, 3}));
    REQUIRE(almost_equal(v1 - v, {1., 2., 3.}));

    REQUIRE(almost_equal(a1 / a, {2., 2., 2.}));
    REQUIRE(array_equal(a1i / ai, {2, 2, 2}));
    REQUIRE(almost_equal(v1 / v, {2., 2., 2.}));

    REQUIRE(almost_equal(a1 % a, {0., 0., 0.}));
    REQUIRE(array_equal(a1i % ai, {0, 0, 0}));
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
    REQUIRE(almost_equal(6._s / b1, {3._s, 1.5_s, 1._s}));

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

TEST_CASE("Arithmetic operators constexpr") {
    using namespace operators;

    constexpr std::array b{1, 0, 1};
    constexpr std::array ai{1, 2, 3};
    constexpr std::array a1i{2, 4, 6};

    static_assert(array_equal(-ai, {-1, -2, -3}));
    static_assert(array_equal(2 * ai, {2, 4, 6}));
    static_assert(array_equal(ai * 2, {2, 4, 6}));
    static_assert(array_equal(2 + ai, {3, 4, 5}));
    static_assert(array_equal(ai + 2, {3, 4, 5}));
    static_assert(array_equal(2 - ai, {1, 0, -1}));
    static_assert(array_equal(ai - 2, {-1, 0, 1}));
    static_assert(array_equal(2 / ai, {2, 1, 0}));
    static_assert(array_equal(ai / 2, {0, 1, 1}));
    static_assert(array_equal(2 % ai, {0, 0, 2}));
    static_assert(array_equal(ai % 2, {1, 0, 1}));
    static_assert(array_equal(a1i * ai, {2, 8, 18}));
    static_assert(array_equal(a1i + ai, {3, 6, 9}));
    static_assert(array_equal(a1i - ai, {1, 2, 3}));
    static_assert(array_equal(a1i / ai, {2, 2, 2}));
    static_assert(array_equal(a1i % ai, {0, 0, 0}));
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

    std::vector v1{1._m, 2._m, 3._m};
    mask_array(v1, m1);
    REQUIRE(almost_equal(v1, {2_m}));
}

TEST_CASE("argmax") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    static_assert(argmax(std::array{1., 2., 3.}) == 2);
    REQUIRE(argmax(std::vector{1.}) == 0);
    REQUIRE(argmax(std::vector{1., 2., 3.}) == 2);
    REQUIRE(argmax(std::vector{1., 3., 3., 2.}) == 1);
    REQUIRE(argmax(std::array{0, 5, 2, 3, 4, 5}) == 1);
    REQUIRE(nanargmax(std::array{nan, 0., nan, 5., 2., 3., nan, 4., 5.}) == 3);
}

TEST_CASE("argmax physical quantities") {
    using namespace units::literals;
    constexpr auto nan = std::numeric_limits<units::meter<double>>::quiet_NaN();
    static_assert(argmax(std::array{1_m, 2_m, 3_m}) == 2);
    REQUIRE(argmax(std::vector{1_kg}) == 0);
    REQUIRE(argmax(std::vector{1_m, 2_m, 3_m}) == 2);
    REQUIRE(argmax(std::vector{1_m, 3_m, 3_m, 2_m}) == 1);
    REQUIRE(argmax(std::array{0_m, 5_m, 2_m, 3_m, 4_m, 5_m}) == 1);
    REQUIRE(nanargmax(
                std::array{nan, 0_m, nan, 5_m, 2_m, 3_m, nan, 4_m, 5_m}) == 3);
}

TEST_CASE("argmin") {
    constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
    static_assert(argmin(std::array{1., 2., 3.}) == 0);
    REQUIRE(argmin(std::vector{1.}) == 0);
    REQUIRE(argmin(std::vector{1., 2., 3.}) == 0);
    REQUIRE(argmin(std::vector{1., -3., -3., 2.}) == 1);
    REQUIRE(argmin(std::array{0, -5, 2, 3, 4, -5}) == 1);
    REQUIRE(nanargmin(std::array{nan, 0., -5., nan, 2., 3., 4., nan, -5.}) ==
            2);
}

TEST_CASE("argmin physical quantities") {
    using namespace units::literals;
    constexpr auto nan =
        std::numeric_limits<units::second<double>>::quiet_NaN();
    static_assert(argmin(std::array{1_s, 2_s, 3_s}) == 0);
    REQUIRE(argmin(std::vector{1_Hz}) == 0);
    REQUIRE(argmin(std::vector{1_s, 2_s, 3_s}) == 0);
    REQUIRE(argmin(std::vector{1_s, -3_s, -3_s, 2_s}) == 1);
    REQUIRE(argmin(std::array{0_s, -5_s, 2_s, 3_s, 4_s, -5_s}) == 1);
    REQUIRE(nanargmin(std::array{
                nan, 0_s, -5_s, nan, 2_s, 3_s, 4_s, nan, -5_s}) == 2);
}

TEST_CASE("argwhere, nonzero") {
    const std::array a{1., 0., -2., 3., 0., 0.};
    const std::vector v{1., 0., -2., 3., 0., 0.};
    const std::vector v0{0., 0., 0., 0.};

    REQUIRE(almost_equal(nonzero(a), {0, 2, 3}));
    REQUIRE(almost_equal(nonzero(v), {0, 2, 3}));
    REQUIRE(nonzero(v0).empty());

    REQUIRE(almost_equal(argwhere(a, [](auto x) { return x > 0; }), {0, 3}));
    REQUIRE(almost_equal(argwhere(v, [](auto x) { return x > 0; }), {0, 3}));
}

TEST_CASE("argwhere, nonzero physical quantities") {
    using namespace units::literals;
    const std::array a{1_m, 0_m, -2_m, 3_m, 0_m, 0_m};
    const std::vector v{1_s, 0_s, -2_s, 3_s, 0_s, 0_s};
    const std::vector v0{0_kg, 0_kg, 0_kg, 0_kg};

    REQUIRE(almost_equal(nonzero(a), {0, 2, 3}));
    REQUIRE(almost_equal(nonzero(v), {0, 2, 3}));
    REQUIRE(nonzero(v0).empty());

    REQUIRE(almost_equal(argwhere(a, [](auto x) { return x > 0_m; }), {0, 3}));
    REQUIRE(almost_equal(argwhere(v, [](auto x) { return x > 0_s; }), {0, 3}));
}

} // namespace scicpp
