// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "spectral.hpp"

#include "scicpp/core/print.hpp"
#include "scicpp/core/random.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/signal/windows.hpp"

#include <cmath>

namespace scicpp::signal {

TEST_CASE("welch") {
    SECTION("Empty") {
        const auto [f1, p1] =
            Spectrum().window(windows::hann<double>(20)).welch(empty<double>());
        REQUIRE(f1.empty());
        REQUIRE(p1.empty());

        const auto [f2, p2] = Spectrum()
                                  .window(windows::hann<double>(20))
                                  .welch(std::array<double, 0>{});
        REQUIRE(f1.empty());
        REQUIRE(p1.empty());
    }

    SECTION("Even size") {
        // >>> x = np.linspace(1,10,100)
        // >>> sp.signal.welch(x, window=sp.signal.windows.hann(20))
        //returns
        // f = [0.  , 0.05, 0.1 , 0.15, 0.2 , 0.25, 0.3 , 0.35, 0.4 , 0.45, 0.5 ]
        // Pxx = [1.1717462900635261e-30, 1.0856685702984454e+00,
        //        1.0217229970700109e-01, 5.1690953307982762e-03,
        //        6.7054096389153556e-04, 1.2711280749082613e-04,
        //        2.8351467354801672e-05, 6.5075192982108344e-06,
        //        1.3856758812935259e-06, 2.7844868643021482e-07,
        //        5.6211853247788487e-08]

        const auto x = linspace(1.0, 10.0, 100);
        const auto [f, Pxx] =
            Spectrum().window(windows::hann<double>(20)).welch(x);

        REQUIRE(almost_equal(
            f, {0., 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5}));
        // print(Pxx);
        REQUIRE(almost_equal<1300>(Pxx,
                                   {1.1717462900635261e-30,
                                    1.0856685702984454e+00,
                                    1.0217229970700109e-01,
                                    5.1690953307982762e-03,
                                    6.7054096389153556e-04,
                                    1.2711280749082613e-04,
                                    2.8351467354801672e-05,
                                    6.5075192982108344e-06,
                                    1.3856758812935259e-06,
                                    2.7844868643021482e-07,
                                    5.6211853247788487e-08}));
    }

    SECTION("Real") {
        auto x = zeros<double>(16);
        x[0] = 1.0;
        x[8] = 1.0;
        const auto [f1, p1] =
            Spectrum().window(windows::hann<double>(8)).welch(x);
        REQUIRE(almost_equal(f1, linspace(0., 0.5, 5)));
        // print(p1);
        REQUIRE(almost_equal<8>(p1,
                                {0.08202736882238,
                                 0.1649754758633829,
                                 0.220872122376833,
                                 0.2307254514666428,
                                 0.1147200837058478}));
        const auto [f2, p2] =
            Spectrum().window(windows::gaussian(8, 2.0)).welch(x);
        // print(p2);
        REQUIRE(almost_equal(f2, linspace(0., 0.5, 5)));
        REQUIRE(almost_equal<8>(p2,
                                {0.0406358487483491,
                                 0.1803216798910316,
                                 0.1968282108542216,
                                 0.1947573237389626,
                                 0.097508612646513}));
        const auto [f3, p3] =
            Spectrum().window(windows::gaussian(9, 2.0)).welch(x);
        // print(p3);
        REQUIRE(almost_equal(f3,
                             {0.,
                              0.1111111111111111,
                              0.2222222222222222,
                              0.3333333333333333,
                              0.4444444444444444}));
        REQUIRE(almost_equal<8>(p3,
                                {0.110595751939577,
                                 0.2859911027014507,
                                 0.2303790376095823,
                                 0.2262491500241708,
                                 0.2202554469147203}));
        const auto [f4, p4] =
            Spectrum().window(windows::gaussian(16, 2.0)).welch(x);
        // print(p4);
        REQUIRE(almost_equal(
            f4, {0., 0.0625, 0.125, 0.1875, 0.25, 0.3125, 0.375, 0.4375, 0.5}));
        REQUIRE(almost_equal<8>(p4,
                                {0.033283705527078,
                                 0.1552473685118426,
                                 0.3652273927481822,
                                 0.4944119749912214,
                                 0.5275115073459213,
                                 0.5288622631786398,
                                 0.5309738347484061,
                                 0.5290399525770682,
                                 0.2654870587529604}));
        const auto [f5, p5] =
            Spectrum().window(windows::gaussian(1, 2.0)).welch(x);
        // print(p5);
        REQUIRE(almost_equal(f5, {0.}));
        REQUIRE(almost_equal(p5, {0.}));
    }

    SECTION("Complex") {
        auto x = zeros<std::complex<double>>(16);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;
        const auto [f, p] =
            Spectrum().window(windows::hann<double>(8.0)).welch(x);
        REQUIRE(almost_equal(
            f, {0., 0.125, 0.25, 0.375, -0.5, -0.375, -0.25, -0.125}));
        REQUIRE(almost_equal<4>(p,
                                {0.4101368441119002,
                                 0.4124386896584574,
                                 0.5521803059420826,
                                 0.5768136286666069,
                                 0.5736004185292388,
                                 0.576813628666607,
                                 0.5521803059420826,
                                 0.4124386896584573}));
    }

    SECTION("White noise") {
        // Generate a Gaussian white-noise with sigma = 1
        const auto noise = random::randn<double>(10000);
        const auto [f, psd] =
            Spectrum().window(windows::boxcar<double>(2000)).welch(noise);
        // The expected PSD for a white noise is 2 * sigma ^ 2
        // print(stats::mean(psd));
        REQUIRE(std::fabs(stats::mean(psd) - 2.) / 2. < 1. / std::sqrt(10000));
    }
}

} // namespace scicpp::signal