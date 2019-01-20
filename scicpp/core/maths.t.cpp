#include "maths.hpp"

#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {

TEST_CASE("fabs") {
    static_assert(float_equal(fabs(-3.141516), 3.141516));
    static_assert(float_equal(fabs(2.71828), 2.71828));

    REQUIRE(std::fpclassify(1. / fabs(-0.)) == FP_INFINITE);
    REQUIRE(std::fpclassify(1. / fabs(+0.)) == FP_INFINITE);
    REQUIRE(std::fpclassify(fabs(-inf)) == FP_INFINITE);
    REQUIRE(std::fpclassify(fabs(inf)) == FP_INFINITE);

    REQUIRE(almost_equal(fabs(std::array{-3.141516, 2.71828, 42., -1.4142}),
                         {3.141516, 2.71828, 42., 1.4142}));
    REQUIRE(almost_equal(fabs(std::vector{-3.141516, 2.71828, 42., -1.4142}),
                         {3.141516, 2.71828, 42., 1.4142}));
}

TEST_CASE("Trigonometric functions") {
    REQUIRE(almost_equal(sin(std::array{1., 2., 3.}),
                         {std::sin(1.), std::sin(2.), std::sin(3.)}));
    REQUIRE(almost_equal(sin(std::array{1.i, 2.i, 3.i}),
                         {std::sin(1.i), std::sin(2.i), std::sin(3.i)}));
    REQUIRE(almost_equal(cos(std::vector{1., 2., 3.}),
                         {std::cos(1.), std::cos(2.), std::cos(3.)}));
    REQUIRE(almost_equal(tan(std::array{1., 2., 3.}),
                         {std::tan(1.), std::tan(2.), std::tan(3.)}));
    REQUIRE(almost_equal(arcsin(std::vector{0.1, 0.2, 0.3}),
                         {std::asin(0.1), std::asin(0.2), std::asin(0.3)}));
    REQUIRE(almost_equal(arccos(std::array{0.1, 0.2, 0.3}),
                         {std::acos(0.1), std::acos(0.2), std::acos(0.3)}));
    REQUIRE(almost_equal(arctan(std::vector{1., 2., 3.}),
                         {std::atan(1.), std::atan(2.), std::atan(3.)}));
    REQUIRE(almost_equal(
        arctan2(std::array{1., 2., 3.}, std::array{3., 2., 1.}),
        {std::atan2(1., 3.), std::atan2(2., 2.), std::atan2(3., 1.)}));
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

} // namespace scicpp
