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
    REQUIRE(almost_equal(boxcar<double, Periodic>(4), {1., 1., 1., 1.}));
    REQUIRE(almost_equal(boxcar<double, 4, Periodic>(), {1., 1., 1., 1.}));
}

TEST_CASE("Bartlett") {
    SECTION("Empty") {
        REQUIRE(bartlett<double>(0).empty());
        REQUIRE(bartlett<double, 0>().empty());
    }

    SECTION("Odd size") {
        REQUIRE(almost_equal<2>(bartlett<double>(5), {0., 0.5, 1., 0.5, 0.}));
        REQUIRE(almost_equal<2>(bartlett<double, 5>(), {0., 0.5, 1., 0.5, 0.}));
        REQUIRE(almost_equal<4>(bartlett<double, Periodic>(5),
                                {0., 0.4, 0.8, 0.8, 0.4}));
        REQUIRE(almost_equal<4>(bartlett<double, 5, Periodic>(),
                                {0., 0.4, 0.8, 0.8, 0.4}));
    }

    SECTION("Even size") {
        REQUIRE(
            almost_equal<2>(bartlett<double>(4), {0., 2. / 3., 2. / 3., 0.}));
        REQUIRE(
            almost_equal<2>(bartlett<double, 4>(), {0., 2. / 3., 2. / 3., 0.}));
        print(bartlett<double, 4, Periodic>());
        REQUIRE(almost_equal<4>(bartlett<double, 4, Periodic>(),
                                {0., 0.5, 1., 0.5}));
    }
}

//---------------------------------------------------------------------------------
// Cosine window
//---------------------------------------------------------------------------------

TEST_CASE("Cosine") {
    SECTION("Empty") {
        REQUIRE(cosine<double>(0).empty());
        REQUIRE(cosine<double, 0>().empty());
    }

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
        REQUIRE(almost_equal<16>(cosine<double, Periodic>(5),
                                 {0.2588190451025207,
                                  0.7071067811865475,
                                  0.9659258262890682,
                                  0.9659258262890683,
                                  0.7071067811865476}));
        REQUIRE(almost_equal<16>(cosine<double, 5, Periodic>(),
                                 {0.2588190451025207,
                                  0.7071067811865475,
                                  0.9659258262890682,
                                  0.9659258262890683,
                                  0.7071067811865476}));
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
        REQUIRE(almost_equal<16>(
            cosine<double, Periodic>(4),
            {0.3090169943749474, 0.8090169943749475, 1., 0.8090169943749475}));
        REQUIRE(almost_equal<16>(
            cosine<double, 4, Periodic>(),
            {0.3090169943749474, 0.8090169943749475, 1., 0.8090169943749475}));
    }
}

//---------------------------------------------------------------------------------
// Cosine sum windows
//---------------------------------------------------------------------------------

TEST_CASE("Hann") {
    REQUIRE(hann<double>(0).empty());
    REQUIRE(hann<double, 0>().empty());
    REQUIRE(almost_equal<4>(hann<double>(4), {0., 0.75, 0.75, 0.}));
    REQUIRE(almost_equal<4>(hann<double, 4>(), {0., 0.75, 0.75, 0.}));
    REQUIRE(almost_equal<4>(hann<double, Periodic>(4), {0., 0.5, 1., 0.5}));
    REQUIRE(almost_equal<4>(hann<double, 4, Periodic>(), {0., 0.5, 1., 0.5}));
    REQUIRE(almost_equal<4>(hann<double, 5, Periodic>(),
                            {0.,
                             0.3454915028125263,
                             0.9045084971874737,
                             0.9045084971874737,
                             0.3454915028125263}));
    REQUIRE(almost_equal<4>(hann<double, Periodic>(5),
                            {0.,
                             0.3454915028125263,
                             0.9045084971874737,
                             0.9045084971874737,
                             0.3454915028125263}));
}

TEST_CASE("Hamming") {
    REQUIRE(hamming<double>(0).empty());
    REQUIRE(hamming<double, 0>().empty());
    REQUIRE(almost_equal<8>(hamming<double>(4), {0.08, 0.77, 0.77, 0.08}));
    REQUIRE(almost_equal<8>(hamming<double, 4>(), {0.08, 0.77, 0.77, 0.08}));
    REQUIRE(almost_equal<8>(hamming<double, 4, Periodic>(),
                            {0.08, 0.54, 1., 0.54}));
    REQUIRE(
        almost_equal<8>(hamming<double, Periodic>(4), {0.08, 0.54, 1., 0.54}));
    REQUIRE(almost_equal<8>(hamming<double, Periodic>(5),
                            {0.08,
                             0.3978521825875243,
                             0.9121478174124759,
                             0.9121478174124759,
                             0.3978521825875243}));
}

TEST_CASE("Blackman") {
    REQUIRE(blackman<double>(0).empty());
    REQUIRE(blackman<double, 0>().empty());
    REQUIRE(almost_equal<4>(blackman<double>(4), {0., 0.63, 0.63, 0.}));
    REQUIRE(almost_equal<4>(blackman<double, 4>(), {0., 0.63, 0.63, 0.}));
    REQUIRE(almost_equal<4>(blackman<double, 4, Periodic>(),
                            {-1.3877787807814457e-17,
                             3.4000000000000002e-01,
                             9.9999999999999989e-01,
                             3.4000000000000002e-01}));
    REQUIRE(almost_equal<4>(blackman<double, Periodic>(4),
                            {-1.3877787807814457e-17,
                             3.4000000000000002e-01,
                             9.9999999999999989e-01,
                             3.4000000000000002e-01}));
    REQUIRE(almost_equal<4>(blackman<double, 5, Periodic>(),
                            {-1.3877787807814457e-17,
                             2.0077014326253051e-01,
                             8.4922985673746942e-01,
                             8.4922985673746942e-01,
                             2.0077014326253051e-01}));
}

TEST_CASE("Nuttall") {
    REQUIRE(nuttall<double>(0).empty());
    REQUIRE(nuttall<double, 0>().empty());
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
    REQUIRE(almost_equal<4>(nuttall<double, 4, Periodic>(),
                            {3.6280000000003809e-04,
                             2.2698240000000006e-01,
                             1.0000000000000000e+00,
                             2.2698240000000006e-01}));
    REQUIRE(almost_equal<4>(nuttall<double, Periodic>(4),
                            {3.6280000000003809e-04,
                             2.2698240000000006e-01,
                             1.0000000000000000e+00,
                             2.2698240000000006e-01}));
    REQUIRE(almost_equal<4>(nuttall<double, 5, Periodic>(),
                            {3.6280000000003809e-04,
                             1.1051525304987181e-01,
                             7.9825809695012828e-01,
                             7.9825809695012828e-01,
                             1.1051525304987181e-01}));
}

TEST_CASE("Blackman-Harris") {
    REQUIRE(blackmanharris<double>(0).empty());
    REQUIRE(blackmanharris<double, 0>().empty());
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
    REQUIRE(almost_equal<4>(blackmanharris<double, 5, Periodic>(),
                            {6.0000000000001025e-05,
                             1.0301148934566379e-01,
                             7.9383351065433627e-01,
                             7.9383351065433627e-01,
                             1.0301148934566379e-01}));
}

TEST_CASE("Flat-top") {
    REQUIRE(flattop<double>(0).empty());
    REQUIRE(flattop<double, 0>().empty());
    REQUIRE(almost_equal(flattop<double>(4),
                         {-0.0004210510000000013,
                          0.1982105300000003567,
                          0.1982105300000003567,
                          -0.0004210510000000013}));
    REQUIRE(almost_equal<4>(flattop<double, 5, Periodic>(),
                            {-4.210510000000013e-04,
                             -6.771425207621193e-02,
                             6.068721525762120e-01,
                             6.068721525762120e-01,
                             -6.771425207621193e-02}));
}

//---------------------------------------------------------------------------------
// Gaussian
//---------------------------------------------------------------------------------

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

        REQUIRE(almost_equal<4>(gaussian<double, 5, Periodic>(2.),
                                {0.4578333617716143,
                                 0.7548396019890073,
                                 0.9692332344763441,
                                 0.9692332344763441,
                                 0.7548396019890073}));
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
        REQUIRE(almost_equal(gaussian(4, -3.14),
                             {0.8921669573788574,
                              0.9874020281709599,
                              0.9874020281709599,
                              0.8921669573788574}));
    }
}

//---------------------------------------------------------------------------------
// General Gaussian
//---------------------------------------------------------------------------------

TEST_CASE("General Gaussian") {
    SECTION("Empty") {
        REQUIRE(general_gaussian(0, 14.0, 0.5).empty());
        REQUIRE(general_gaussian<double, 0>(14.0, 0.5).empty());
    }

    SECTION("Odd size") {
        // print(general_gaussian(5, 14.0, 0.5));
        REQUIRE(
            almost_equal(general_gaussian(5, 14.0, 0.5), {0., 0., 1., 0., 0.}));
        REQUIRE(almost_equal(general_gaussian(5, 0.5, 14.0),
                             {0.9310627797040227,
                              0.96491594437237,
                              1.,
                              0.96491594437237,
                              0.9310627797040227}));
        REQUIRE(almost_equal(general_gaussian<double, 5, Periodic>(0.5, 14.0),
                             {0.9145842264619389,
                              0.9478382358546584,
                              0.9823013510997377,
                              0.9823013510997377,
                              0.9478382358546584}));
    }

    SECTION("Even size") {
        REQUIRE(almost_equal(general_gaussian(4, 0.5, 14.0),
                             {0.9478382358546584,
                              0.9823013510997377,
                              0.9823013510997377,
                              0.9478382358546584}));
        REQUIRE(almost_equal(general_gaussian<double, 6, Periodic>(0.5, 14.0),
                             {0.898397321348071,
                              0.9310627797040227,
                              0.96491594437237,
                              1.,
                              0.96491594437237,
                              0.9310627797040227}));
    }
}

//---------------------------------------------------------------------------------
// Kaiser
//---------------------------------------------------------------------------------

TEST_CASE("Kaiser") {
    SECTION("Empty") {
        REQUIRE(kaiser(0, 14.0).empty());
        REQUIRE(kaiser<double, 0>(14.0).empty());
    }

    SECTION("Odd size") {
        // print(kaiser(5, 14.0));
        REQUIRE(almost_equal<12>(kaiser(5, 14.0),
                                 {7.7268668352703676e-06,
                                  1.6493218754795197e-01,
                                  1.0000000000000000e+00,
                                  1.6493218754795197e-01,
                                  7.7268668352703676e-06}));
        REQUIRE(almost_equal<12>(kaiser<double, 5>(14.0),
                                 {7.7268668352703676e-06,
                                  1.6493218754795197e-01,
                                  1.0000000000000000e+00,
                                  1.6493218754795197e-01,
                                  7.7268668352703676e-06}));
        REQUIRE(almost_equal<16>(kaiser<double, 5, Periodic>(14.0),
                                 {7.726866835270368e-06,
                                  6.815374319396057e-02,
                                  7.615093990873935e-01,
                                  7.615093990873935e-01,
                                  6.815374319396057e-02}));
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

//---------------------------------------------------------------------------------
// get_window
//---------------------------------------------------------------------------------

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
