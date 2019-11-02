// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "maths.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/units/units.hpp"

namespace scicpp {

TEST_CASE("scicpp::fabs") {
    constexpr auto inf = std::numeric_limits<double>::infinity();

    static_assert(float_equal(scicpp::fabs(-3.141516), 3.141516));
    static_assert(float_equal(scicpp::fabs(2.71828), 2.71828));

    REQUIRE(std::fpclassify(1. / scicpp::fabs(-0.)) == FP_INFINITE);
    REQUIRE(std::fpclassify(1. / scicpp::fabs(+0.)) == FP_INFINITE);
    REQUIRE(std::fpclassify(scicpp::fabs(-inf)) == FP_INFINITE);
    REQUIRE(std::fpclassify(scicpp::fabs(inf)) == FP_INFINITE);

    REQUIRE(
        almost_equal(scicpp::fabs(std::array{-3.141516, 2.71828, 42., -1.4142}),
                     {3.141516, 2.71828, 42., 1.4142}));
    REQUIRE(almost_equal(
        scicpp::fabs(std::vector{-3.141516, 2.71828, 42., -1.4142}),
        {3.141516, 2.71828, 42., 1.4142}));
}

TEST_CASE("Trigonometric functions") {
    using namespace units::literals;

    REQUIRE(almost_equal<2>(sin(std::array{0_deg, 90_deg, 180_deg, 270_deg}),
                            {0., 1., 0., -1.}));
    REQUIRE(almost_equal(sin(std::array{1_rad, 2_rad, 3_rad}),
                         {std::sin(1.), std::sin(2.), std::sin(3.)}));
    REQUIRE(almost_equal(sin(std::array{1.i, 2.i, 3.i}),
                         {std::sin(1.i), std::sin(2.i), std::sin(3.i)}));
    REQUIRE(almost_equal<2>(cos(std::array{0_deg, 90_deg, 180_deg, 270_deg}),
                            {1., 0., -1., 0.}));
    REQUIRE(almost_equal(cos(std::array{1_rad, 2_rad, 3_rad}),
                         {std::cos(1.), std::cos(2.), std::cos(3.)}));
    REQUIRE(almost_equal(tan(std::array{1_rad, 2_rad, 3_rad}),
                         {std::tan(1.), std::tan(2.), std::tan(3.)}));
    REQUIRE(almost_equal(arcsin(std::vector{0.1, 0.2, 0.3}),
                         {std::asin(0.1) * 1_rad,
                          std::asin(0.2) * 1_rad,
                          std::asin(0.3) * 1_rad}));
    REQUIRE(almost_equal(arccos(std::array{0.1, 0.2, 0.3}),
                         {std::acos(0.1) * 1_rad,
                          std::acos(0.2) * 1_rad,
                          std::acos(0.3) * 1_rad}));
    REQUIRE(almost_equal(
        arctan(std::vector{1., 2., 3.}),
        {std::atan(1.) * 1_rad, std::atan(2.) * 1_rad, std::atan(3.) * 1_rad}));
    REQUIRE(
        almost_equal(arctan2(std::array{1., 2., 3.}, std::array{3., 2., 1.}),
                     {std::atan2(1., 3.) * 1_rad,
                      std::atan2(2., 2.) * 1_rad,
                      std::atan2(3., 1.) * 1_rad}));
    REQUIRE(almost_equal(
        hypot(std::array{1., 2., 3.}, std::array{3., 2., 1.}),
        {std::hypot(1., 3.), std::hypot(2., 2.), std::hypot(3., 1.)}));
    REQUIRE(almost_equal(
        hypot(std::array{1_m, 2_m, 3_m}, std::array{3_m, 2_m, 1_m}),
        {units::hypot(1_m, 3_m),
         units::hypot(2_m, 2_m),
         units::hypot(3_m, 1_m)}));
}

TEST_CASE("Hyperbolic functions") {
    REQUIRE(almost_equal<2>(sinh(std::array{1., 2., 3.}),
                            {std::sinh(1.), std::sinh(2.), std::sinh(3.)}));
    REQUIRE(almost_equal<2>(sinh(std::array{1.i, 2.i, 3.i}),
                            {std::sinh(1.i), std::sinh(2.i), std::sinh(3.i)}));
    REQUIRE(almost_equal<2>(cosh(std::vector{1., 2., 3.}),
                            {std::cosh(1.), std::cosh(2.), std::cosh(3.)}));
    REQUIRE(almost_equal<2>(tanh(std::array{1., 2., 3.}),
                            {std::tanh(1.), std::tanh(2.), std::tanh(3.)}));
    REQUIRE(
        almost_equal<2>(arcsinh(std::vector{0.1, 0.2, 0.3}),
                        {std::asinh(0.1), std::asinh(0.2), std::asinh(0.3)}));
    REQUIRE(
        almost_equal<2>(arccosh(std::array{0.1, 0.2, 0.3}),
                        {std::acosh(0.1), std::acosh(0.2), std::acosh(0.3)}));
    REQUIRE(
        almost_equal<2>(arctanh(std::vector{0.1, 0.2, 0.3}),
                        {std::atanh(0.1), std::atanh(0.2), std::atanh(0.3)}));
}

TEST_CASE("Exponents and logarithms") {
    REQUIRE(almost_equal(exp(std::vector{1., 2., 3.}),
                         {std::exp(1.), std::exp(2.), std::exp(3.)}));
    REQUIRE(almost_equal(expm1(std::array{1., 2., 3.}),
                         {std::expm1(1.), std::expm1(2.), std::expm1(3.)}));
    REQUIRE(almost_equal(exp2(std::vector{1., 2., 3.}),
                         {std::exp2(1.), std::exp2(2.), std::exp2(3.)}));
    REQUIRE(almost_equal(log(std::array{1., 2., 3.}),
                         {std::log(1.), std::log(2.), std::log(3.)}));
    REQUIRE(almost_equal(log2(std::vector{1., 2., 3.}),
                         {std::log2(1.), std::log2(2.), std::log2(3.)}));
    REQUIRE(almost_equal<2>(log1p(std::array{1., 2., 3.}),
                            {std::log1p(1.), std::log1p(2.), std::log1p(3.)}));
}

TEST_CASE("Rounding") {
    using namespace units::literals;

    REQUIRE(almost_equal(around(std::vector{1.2, 2.5, 3.67}), {1., 3., 4.}));
    REQUIRE(almost_equal(around(std::vector{1.2_m, 2.5_m, 3.67_m}),
                         {1_m, 3_m, 4_m}));
    REQUIRE(almost_equal(floor(std::array{1., 2., 3.}),
                         {std::floor(1.), std::floor(2.), std::floor(3.)}));
    REQUIRE(almost_equal(floor(std::array{1_m, 2_m, 3_m}), {1_m, 2_m, 3_m}));
    REQUIRE(almost_equal(ceil(std::array{1., 2., 3.}),
                         {std::ceil(1.), std::ceil(2.), std::ceil(3.)}));
    REQUIRE(
        almost_equal(ceil(std::array{1.1_m, 2.1_m, 3.1_m}), {2_m, 3_m, 4_m}));
    REQUIRE(almost_equal(trunc(std::array{1., 2., 3.}),
                         {std::trunc(1.), std::trunc(2.), std::trunc(3.)}));
    REQUIRE(almost_equal(trunc(std::array{1_m, 2_m, 3_m}), {1_m, 2_m, 3_m}));
    REQUIRE(almost_equal(rint(std::array{1., 2., 3.}),
                         {std::rint(1.), std::rint(2.), std::rint(3.)}));
    REQUIRE(almost_equal(rint(std::array{1_m, 2_m, 3_m}), {1_m, 2_m, 3_m}));
}

TEST_CASE("Complex numbers") {
    using namespace units::literals;

    const std::vector v{1. - 1.i, -42. + 3.i, -64. + 42.i};

    REQUIRE(almost_equal(real(v), {1., -42., -64.}));
    REQUIRE(almost_equal(imag(v), {-1., 3., 42.}));
    REQUIRE(almost_equal(
        angle(v),
        {std::arg(1. - 1.i), std::arg(-42. + 3.i), std::arg(-64. + 42.i)}));
    REQUIRE(almost_equal(conj(v), {1. + 1.i, -42. - 3.i, -64. - 42.i}));
    REQUIRE(almost_equal(
        norm(v),
        {std::norm(1. - 1.i), std::norm(-42. + 3.i), std::norm(-64. + 42.i)}));
    REQUIRE(almost_equal(
        polar(std::vector{1., 2., 3.}, std::vector{3_rad, 2_rad, 1_rad}),
        {std::polar(1., 3.), std::polar(2., 2.), std::polar(3., 1.)}));
}

TEST_CASE("Rational routines") {
    REQUIRE(almost_equal(gcd(std::vector{1, 2, 3}, std::vector{3, 2, 1}),
                         {std::gcd(1, 3), std::gcd(2, 2), std::gcd(3, 1)}));
    REQUIRE(almost_equal(lcm(std::vector{1, 2, 3}, std::vector{3, 2, 1}),
                         {std::lcm(1, 3), std::lcm(2, 2), std::lcm(3, 1)}));
}

TEST_CASE("Miscellaneous") {
    using namespace units::literals;

    REQUIRE(almost_equal(absolute(std::array{1., -42., -64.}), {1., 42., 64.}));
    REQUIRE(almost_equal(absolute(std::array{1_m3, -42_m3, -64_m3}),
                         {1_m3, 42_m3, 64_m3}));
    REQUIRE(almost_equal(
        absolute(std::vector{1. - 1.i, -42. + 3.i, -64. + 42.i}),
        {std::abs(1. - 1.i), std::abs(-42. + 3.i), std::abs(-64. + 42.i)}));
    REQUIRE(almost_equal(sqrt(std::vector{1., 2., 3.}),
                         {std::sqrt(1.), std::sqrt(2.), std::sqrt(3.)}));
    REQUIRE(almost_equal(
        sqrt(std::vector{1_uH, 2_uH, 3_uH}),
        {units::sqrt(1_uH), units::sqrt(2_uH), units::sqrt(3_uH)}));
    REQUIRE(almost_equal<2>(cbrt(std::vector{1., 2., 3.}),
                            {std::cbrt(1.), std::cbrt(2.), std::cbrt(3.)}));
    REQUIRE(almost_equal<2>(
        cbrt(std::vector{1_mF, 2_mF, 3_mF}),
        {units::cbrt(1_mF), units::cbrt(2_mF), units::cbrt(3_mF)}));
}

} // namespace scicpp
