// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "convolve.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp {
namespace signal {

namespace convolve_static_tests {

constexpr std::array a{1., 2., 3.};
constexpr std::array v{0., 1., 0.5};
constexpr auto res = convolve(a, v);
static_assert(res.size() == 5);
static_assert(float_equal(res[0], 0.));
static_assert(float_equal(res[1], 1.));
static_assert(float_equal(res[2], 2.5));
static_assert(float_equal(res[3], 4.));
static_assert(float_equal(res[4], 1.5));

} // namespace convolve_static_tests

//---------------------------------------------------------------------------------
// convolve
//---------------------------------------------------------------------------------

TEST_CASE("Convolve std::arrays") {
    SECTION("Same size") {
        const std::array a{1., 2., 3.};
        const std::array v{0., 1., 0.5};
        const auto res = convolve(a, v);
        REQUIRE(almost_equal(res, {0., 1., 2.5, 4., 1.5}));
    }

    SECTION("a smaller than v") {
        const std::array a{3.14, 2.7, 42., 78.5};
        const std::array v{1.0, 0.5, 1.0};
        const auto res = convolve(a, v);
        REQUIRE(almost_equal<2>(res, {3.14, 4.27, 46.49, 102.2, 81.25, 78.5}));
    }

    SECTION("complex values") {
        const std::array a{3.14 + 1.i, 2.7 + 3.14i, 42. + 0.i, 78.8i};
        const std::array v{1. + 0.i, 0.5i, 1. + 0.i};
        const auto res = convolve(a, v);
        // print(res);
        REQUIRE(almost_equal<8>(res,
                                {3.14 + 1.i,
                                 2.2 + 4.71i,
                                 43.57 + 2.35i,
                                 2.7 + 102.94i,
                                 2.6 + 0.i,
                                 78.8i}));
    }
}

TEST_CASE("Convolve std::vectors") {
    SECTION("Same size") {
        const std::vector a{1., 2., 3.};
        const std::vector v{0., 1., 0.5};
        const auto res = convolve(a, v);
        REQUIRE(almost_equal(res, {0., 1., 2.5, 4., 1.5}));
    }

    SECTION("a smaller than v") {
        const std::vector a{3.14, 2.7, 42., 78.5};
        const std::vector v{1.0, 0.5, 1.0};
        const auto res = convolve(a, v);
        REQUIRE(almost_equal<2>(res, {3.14, 4.27, 46.49, 102.2, 81.25, 78.5}));
    }

    SECTION("v smaller than a") {
        const std::vector a{3.14, 2.7};
        const std::vector v{1.0, 0.5, 1.0, 42., 78.5};
        const auto res = convolve(a, v);
        REQUIRE(
            almost_equal<2>(res, {3.14, 4.27, 4.49, 134.58, 359.89, 211.95}));
    }
}

//---------------------------------------------------------------------------------
// fftconvolve
//---------------------------------------------------------------------------------

TEST_CASE("fftconvolve") {
    SECTION("Same size") {
        const std::vector a{1., 2., 3.};
        const std::vector v{0., 1., 0.5};
        const auto res = fftconvolve(a, v);
        // print(res);
        REQUIRE(almost_equal<4>(res, {0., 1., 2.5, 4., 1.5}));
    }

    SECTION("a smaller than v") {
        const std::vector a{3.14, 2.7, 42., 78.5};
        const std::vector v{1., 0.5, 1.};
        const auto res = fftconvolve(a, v);
        REQUIRE(almost_equal<15>(res, {3.14, 4.27, 46.49, 102.2, 81.25, 78.5}));
    }

    SECTION("v smaller than a") {
        const std::vector a{3.14, 2.7};
        const std::vector v{1., 0.5, 1., 42., 78.5};
        const auto res = fftconvolve(a, v);
        // print(res);
        REQUIRE(
            almost_equal<100>(res, {3.14, 4.27, 4.49, 134.58, 359.89, 211.95}));
    }

    SECTION("complex values") {
        const std::vector a{3.14 + 1.i, 2.7 + 3.14i, 42. + 0.i, 78.8i};
        const std::vector v{1. + 0.i, 0.5i, 1. + 0.i};
        const auto res = fftconvolve(a, v);
        // print(res);
        REQUIRE(almost_equal<200>(res,
                                  {3.14 + 1.i,
                                   2.2 + 4.71i,
                                   43.57 + 2.35i,
                                   2.7 + 102.94i,
                                   2.6 + 0.i,
                                   78.8i}));
    }
}

//---------------------------------------------------------------------------------
// correlate
//---------------------------------------------------------------------------------

TEST_CASE("correlate") {
    SECTION("std::array real") {
        const std::array a{3.14, 2.7, 42., 78.8};
        const std::array v{1., 0.5, 1.};
        const auto res = correlate(a, v);
        // print(res);
        REQUIRE(almost_equal<2>(res, {3.14, 4.27, 46.49, 102.5, 81.4, 78.8}));
    }

    SECTION("std::array complex") {
        const std::array a{3.14 + 1.i, 2.7 + 3.14i, 42. + 0.i, 78.8i};
        const std::array v{1. + 0.i, 0.5i, 1. + 0.i};
        const auto res = correlate(a, v);
        // print(res);
        REQUIRE(almost_equal<4>(res,
                                {3.14 + 1.i,
                                 3.2 + 1.57i,
                                 46.71 - 0.35i,
                                 2.7 + 60.94i,
                                 81.4 + 0.i,
                                 78.8i}));
    }

    SECTION("std::vector real") {
        const std::vector a{3.14, 2.7, 42., 78.8};
        const std::vector v{1., 0.5, 1.};
        const auto res = correlate(a, v);
        // print(res);
        REQUIRE(almost_equal<2>(res, {3.14, 4.27, 46.49, 102.5, 81.4, 78.8}));
    }

    SECTION("std::vector real, method FFT") {
        const std::vector a{3.14, 2.7, 42., 78.8};
        const std::vector v{1., 0.5, 1.};
        const auto res = correlate<FFT>(a, v);
        // print(res);
        REQUIRE(almost_equal<25>(res, {3.14, 4.27, 46.49, 102.5, 81.4, 78.8}));
    }

    SECTION("std::vector complex") {
        const std::vector a{3.14 + 1.i, 2.7 + 3.14i, 42. + 0.i, 78.8i};
        const std::vector v{1. + 0.i, 0.5i, 1. + 0.i};
        const auto res = correlate(a, v);
        // print(res);
        REQUIRE(almost_equal<4>(res,
                                {3.14 + 1.i,
                                 3.2 + 1.57i,
                                 46.71 - 0.35i,
                                 2.7 + 60.94i,
                                 81.4 + 0.i,
                                 78.8i}));
    }
}

} // namespace signal
} // namespace scicpp