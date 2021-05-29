// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "windows.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"

namespace scicpp::signal::windows {

//---------------------------------------------------------------------------------
// Polynomial windows
//---------------------------------------------------------------------------------

TEST_CASE("Boxcar") {
    REQUIRE(almost_equal(boxcar<double>(4), {1., 1., 1., 1.}));
    REQUIRE(almost_equal(boxcar<double, 4>(), {1., 1., 1., 1.}));
}

TEST_CASE("Bartlett") {
    SECTION("Odd size") {
        REQUIRE(almost_equal<2>(bartlett<double>(5), {0., 0.5, 1., 0.5, 0.}));
        REQUIRE(almost_equal<2>(bartlett<double, 5>(), {0., 0.5, 1., 0.5, 0.}));
    }

    SECTION("Even size") {
        REQUIRE(
            almost_equal<2>(bartlett<double>(4), {0., 2. / 3., 2. / 3., 0.}));
        REQUIRE(
            almost_equal<2>(bartlett<double, 4>(), {0., 2. / 3., 2. / 3., 0.}));
    }
}

//---------------------------------------------------------------------------------
// Cosine window
//---------------------------------------------------------------------------------

TEST_CASE("Cosine") {
    SECTION("Odd size") {
        REQUIRE(almost_equal<4>(cosine<double>(5),
                                {0.3090169943749474,
                                 0.8090169943749475,
                                 1.,
                                 0.8090169943749475,
                                 0.3090169943749474}));
        REQUIRE(almost_equal<4>(cosine<double, 5>(),
                                {0.3090169943749474,
                                 0.8090169943749475,
                                 1.,
                                 0.8090169943749475,
                                 0.3090169943749474}));
    }

    SECTION("Even size") {
        REQUIRE(almost_equal<4>(cosine<double>(4),
                                {0.3826834323650898,
                                 0.9238795325112867,
                                 0.9238795325112867,
                                 0.3826834323650898}));
        REQUIRE(almost_equal<4>(cosine<double, 4>(),
                                {0.3826834323650898,
                                 0.9238795325112867,
                                 0.9238795325112867,
                                 0.3826834323650898}));
    }
}

//---------------------------------------------------------------------------------
// Cosine sum windows
//---------------------------------------------------------------------------------

TEST_CASE("Hann") {
    REQUIRE(almost_equal<4>(hann<double>(4), {0., 0.75, 0.75, 0.}));
    REQUIRE(almost_equal<4>(hann<double, 4>(), {0., 0.75, 0.75, 0.}));
}

TEST_CASE("Hamming") {
    REQUIRE(almost_equal<8>(hamming<double>(4), {0.08, 0.77, 0.77, 0.08}));
    REQUIRE(almost_equal<8>(hamming<double, 4>(), {0.08, 0.77, 0.77, 0.08}));
}

TEST_CASE("Blackman") {
    REQUIRE(almost_equal<4>(blackman<double>(4), {0., 0.63, 0.63, 0.}));
    REQUIRE(almost_equal<4>(blackman<double, 4>(), {0., 0.63, 0.63, 0.}));
}

TEST_CASE("Nuttall") {
    REQUIRE(almost_equal<4>(nuttall<double>(4),
                            {0.0003628000000000381,
                             0.5292298000000004166,
                             0.5292298000000004166,
                             0.0003628000000000381}));
    REQUIRE(almost_equal<4>(nuttall<double, 4>(),
                            {0.0003628000000000381,
                             0.5292298000000004166,
                             0.5292298000000004166,
                             0.0003628000000000381}));
}

TEST_CASE("Blackman-Harris") {
    REQUIRE(almost_equal<4>(blackmanharris<double>(4),
                            {6.0000000000001025E-5,
                             0.5205750000000002320,
                             0.5205750000000002320,
                             6.0000000000001025E-5}));
    REQUIRE(almost_equal<4>(blackmanharris<double, 4>(),
                            {6.0000000000001025E-5,
                             0.5205750000000002320,
                             0.5205750000000002320,
                             6.0000000000001025E-5}));
}

TEST_CASE("Flat-top") {
    REQUIRE(almost_equal(flattop<double>(4),
                         {-0.0004210510000000013,
                          0.1982105300000003567,
                          0.1982105300000003567,
                          -0.0004210510000000013}));
    REQUIRE(almost_equal(flattop<double, 4>(),
                         {-0.0004210510000000013,
                          0.1982105300000003567,
                          0.1982105300000003567,
                          -0.0004210510000000013}));
}

TEST_CASE("Gaussian") {
    SECTION("Empty") {
        REQUIRE(gaussian(0, 14.0).empty());
        REQUIRE(gaussian<double, 0>(14.0).empty());
    }

    SECTION("Odd size") {
        REQUIRE(almost_equal(gaussian<double>(5, 2.),
                             {0.6065306597126334,
                              0.8824969025845955,
                              1.,
                              0.8824969025845955,
                              0.6065306597126334}));

        REQUIRE(almost_equal(gaussian<double, 5>(2.),
                             {0.6065306597126334,
                              0.8824969025845955,
                              1.,
                              0.8824969025845955,
                              0.6065306597126334}));
    }

    SECTION("Even size") {
        REQUIRE(almost_equal(gaussian(4, 3.14),
                             {0.8921669573788574,
                              0.9874020281709599,
                              0.9874020281709599,
                              0.8921669573788574}));
        REQUIRE(almost_equal(gaussian<double, 4>(3.14),
                             {0.8921669573788574,
                              0.9874020281709599,
                              0.9874020281709599,
                              0.8921669573788574}));
    }
}

TEST_CASE("Kaiser") {
    SECTION("Empty") {
        REQUIRE(kaiser(0, 14.0).empty());
        REQUIRE(kaiser<double, 0>(14.0).empty());
    }

    SECTION("Odd size") {
        // print(kaiser(5, 14.0));
        REQUIRE(almost_equal<8>(kaiser(5, 14.0),
                                {7.7268668352703676e-06,
                                 1.6493218754795197e-01,
                                 1.0000000000000000e+00,
                                 1.6493218754795197e-01,
                                 7.7268668352703676e-06}));
        REQUIRE(almost_equal<8>(kaiser<double, 5>(14.0),
                                {7.7268668352703676e-06,
                                 1.6493218754795197e-01,
                                 1.0000000000000000e+00,
                                 1.6493218754795197e-01,
                                 7.7268668352703676e-06}));
    }

    SECTION("Even size") {
        // print(kaiser(4, 20.0));
        REQUIRE(almost_equal<16>(kaiser(4, 20.0),
                                 {2.2957746293894510e-08,
                                  3.2825222610412064e-01,
                                  3.2825222610412064e-01,
                                  2.2957746293894510e-08}));
        REQUIRE(almost_equal<16>(kaiser<double, 4>(20.0),
                                 {2.2957746293894510e-08,
                                  3.2825222610412064e-01,
                                  3.2825222610412064e-01,
                                  2.2957746293894510e-08}));
        REQUIRE(almost_equal<16>(kaiser(4, -20.0),
                                 {2.2957746293894510e-08,
                                  3.2825222610412064e-01,
                                  3.2825222610412064e-01,
                                  2.2957746293894510e-08}));
        REQUIRE(almost_equal(kaiser(4, 0.0), ones<double>(4)));
    }
}

TEST_CASE("get_window") {
    SECTION("std::array") {
        REQUIRE(almost_equal<4>(get_window<Hann, 4>(), {0., 0.75, 0.75, 0.}));
        REQUIRE(almost_equal(get_window<Nuttall, 4>(),
                             {0.0003628000000000381,
                              0.5292298000000004166,
                              0.5292298000000004166,
                              0.0003628000000000381}));
    }

    SECTION("std::vector") {
        REQUIRE(almost_equal<4>(get_window(Hann, 4), {0., 0.75, 0.75, 0.}));
        REQUIRE(almost_equal(get_window(Nuttall, 4),
                             {0.0003628000000000381,
                              0.5292298000000004166,
                              0.5292298000000004166,
                              0.0003628000000000381}));
    }
}

} // namespace scicpp::signal::windows
