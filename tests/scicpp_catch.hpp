#ifndef SCICPP_CATCH
#define SCICPP_CATCH

// clang-format off
#include "catch.hpp"

#include <limits>

// https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison
#include "FloatingPoint.hpp" 

#include <scicpp/core/maths.hpp>
// clang-format on

constexpr auto eps = std::numeric_limits<double>::epsilon();
constexpr auto inf = std::numeric_limits<double>::infinity();

template <typename T>
constexpr bool float_equal(const T f1, const T f2) {
    return scicpp::fabs(f1 - f2) <= eps;
}

template <typename T>
bool almost_equals(T left, T right) {
    const FloatingPoint<T> lhs(left), rhs(right);
    return lhs.AlmostEquals(rhs);
}

#define FLOAT_EQUAL(f1, f2) (almost_equals((f1), (f2)))
#define CPLX_EQUAL(c1, c2)                                                     \
    (FLOAT_EQUAL((c1).real(), (c2).real()) &&                                  \
     FLOAT_EQUAL((c1).imag(), (c2).imag()))

#define REQUIRE_FLOAT_EQUAL(a, b) REQUIRE(FLOAT_EQUAL((a), (b)))
#define REQUIRE_CPLX_EQUAL(a, b) REQUIRE(CPLX_EQUAL((a), (b)))

#endif // SCICPP_CATCH
