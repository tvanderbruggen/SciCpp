#include "maths.hpp"

#include <cmath>
#include <cstdio>

namespace scicpp {
namespace maths {

//---------------------------------------------------------------------------------
// fabs
//---------------------------------------------------------------------------------

namespace constexpr_tests {
static_assert(float_equal(fabs(-3.141516), 3.141516));
static_assert(float_equal(fabs(2.71828), 2.71828));
} // namespace constexpr_tests

TEST_CASE("fabs zero and inf") {
    REQUIRE(std::fpclassify(1. / fabs(-0.)) == FP_INFINITE);
    REQUIRE(std::fpclassify(1. / fabs(+0.)) == FP_INFINITE);
    REQUIRE(std::fpclassify(fabs(-inf)) == FP_INFINITE);
    REQUIRE(std::fpclassify(fabs(inf)) == FP_INFINITE);
}

TEST_CASE("fabs std::array") {
    const std::array<double, 4> a = {{-3.141516, 2.71828, 42.0, -1.4142}};
    const auto abs_a = fabs(a);
    REQUIRE_FLOAT_EQUAL(abs_a[0], 3.141516);
    REQUIRE_FLOAT_EQUAL(abs_a[1], 2.71828);
    REQUIRE_FLOAT_EQUAL(abs_a[2], 42.0);
    REQUIRE_FLOAT_EQUAL(abs_a[3], 1.4142);
}

TEST_CASE("fabs std::vector") {
    std::vector<double> v(4);
    v[0] = -3.141516;
    v[1] = 2.71828;
    v[2] = 42.0;
    v[3] = -1.4142;
    const auto abs_v = fabs(v);
    REQUIRE_FLOAT_EQUAL(abs_v[0], 3.141516);
    REQUIRE_FLOAT_EQUAL(abs_v[1], 2.71828);
    REQUIRE_FLOAT_EQUAL(abs_v[2], 42.0);
    REQUIRE_FLOAT_EQUAL(abs_v[3], 1.4142);
}

} // namespace maths
} // namespace scicpp
