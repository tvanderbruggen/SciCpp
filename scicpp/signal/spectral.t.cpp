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

// Use Spectrum{} instead of Spectrum() for initialization
// because of a bug with GCC:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81486

TEST_CASE("welch") {
    SECTION("Empty") {
        const auto [f1, p1] =
            Spectrum{}.window(windows::hann<double>(20)).welch(empty<double>());
        REQUIRE(f1.empty());
        REQUIRE(p1.empty());

        const auto [f2, p2] = Spectrum{}
                                  .window(windows::hann<double>(20))
                                  .welch(std::array<double, 0>{});
        REQUIRE(f2.empty());
        REQUIRE(p2.empty());
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
            Spectrum{}.window(windows::hann<double>(20)).welch(x);

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
            Spectrum{}.window(windows::hann<double>(8)).welch(x);
        REQUIRE(almost_equal(f1, linspace(0., 0.5, 5)));
        // print(p1);
        REQUIRE(almost_equal<8>(p1,
                                {0.08202736882238,
                                 0.1649754758633829,
                                 0.220872122376833,
                                 0.2307254514666428,
                                 0.1147200837058478}));
        const auto [f2, p2] =
            Spectrum{}.window(windows::gaussian(8, 2.0)).welch(x);
        // print(p2);
        REQUIRE(almost_equal(f2, linspace(0., 0.5, 5)));
        REQUIRE(almost_equal<8>(p2,
                                {0.0406358487483491,
                                 0.1803216798910316,
                                 0.1968282108542216,
                                 0.1947573237389626,
                                 0.097508612646513}));
        const auto [f3, p3] =
            Spectrum{}.window(windows::gaussian(9, 2.0)).welch(x);
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
            Spectrum{}.window(windows::gaussian(16, 2.0)).welch(x);
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
            Spectrum{}.window(windows::gaussian(1, 2.0)).welch(x);
        // print(p5);
        REQUIRE(almost_equal(f5, {0.}));
        REQUIRE(almost_equal(p5, {0.}));
    }

    SECTION("Complex") {
        auto x = zeros<std::complex<double>>(16);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;

        auto spec = Spectrum{}.window(windows::hann<double>(8.0));
        const auto [f, p] = spec.welch(x);
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
        const auto [f1, p1] = spec.welch<SpectrumScaling::SPECTRUM>(x);
        // print(p1);
        REQUIRE(almost_equal(
            f1, {0., 0.125, 0.25, 0.375, -0.5, -0.375, -0.25, -0.125}));
        REQUIRE(almost_equal<6>(p1,
                                {0.0878864665954072,
                                 0.0883797192125266,
                                 0.1183243512733034,
                                 0.1236029204285586,
                                 0.1229143753991226,
                                 0.1236029204285586,
                                 0.1183243512733034,
                                 0.0883797192125266}));
    }

    SECTION("White noise") {
        // Generate a Gaussian white-noise with sigma = 1
        const auto noise = random::randn<double>(10000);
        const auto [f, psd] =
            Spectrum{}.window(windows::boxcar<double>(2000)).welch(noise);
        // The expected PSD for a white noise is 2 * sigma ^ 2
        // print(stats::mean(psd));
        REQUIRE(std::fabs(stats::mean(psd) - 2.) / 2. < 1. / std::sqrt(10000));
    }
}

TEST_CASE("csd") {
    SECTION("Empty") {
        const auto [f1, p1] = Spectrum{}
                                  .window(windows::hann<double>(20))
                                  .csd(empty<double>(), empty<double>());
        REQUIRE(f1.empty());
        REQUIRE(p1.empty());

        const auto [f2, p2] =
            Spectrum{}
                .window(windows::hann<double>(20))
                .csd(std::array<double, 0>{}, std::array<double, 0>{});
        REQUIRE(f2.empty());
        REQUIRE(p2.empty());

        const auto [f3, p3] = Spectrum{}
                                  .window(windows::hann<double>(20))
                                  .csd(empty<std::complex<double>>(),
                                       empty<std::complex<double>>());
        REQUIRE(f3.empty());
        REQUIRE(p3.empty());
    }

    SECTION("Same data real") {
        const auto x = linspace(1.0, 10.0, 100);
        const auto [f, Pxx] = Spectrum{}.window(windows::Hann, 20).csd(x, x);

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

    SECTION("Different data real same size") {
        const auto x = linspace(1.0, 10.0, 100);
        const auto y = linspace(10.0, 100.0, 100);
        const auto [f, Pxy] = Spectrum{}.window(windows::Hamming, 15).csd(x, y);

        // print(f);
        REQUIRE(almost_equal<4>(f,
                                {0.,
                                 0.0666666666666667,
                                 0.1333333333333333,
                                 0.2,
                                 0.2666666666666667,
                                 0.3333333333333333,
                                 0.4,
                                 0.4666666666666667}));
        // print(Pxy);
        REQUIRE(
            almost_equal(Pxy,
                         {-3.7062851089728351e-30 + 0.0000000000000000e+00i,
                          5.5638353667040628e+00 - 3.1311659646683040e-15i,
                          1.3896361537945315e-01 + 2.8720474849532764e-16i,
                          1.9363830151308823e-03 + 7.8567939380376008e-18i,
                          9.1683760095253690e-03 + 3.2446435506274200e-17i,
                          1.0622173917597896e-02 + 5.6852044063349557e-17i,
                          1.0516902037792620e-02 - 8.7090083435688860e-17i,
                          1.0293880420302366e-02 + 2.1149639898068783e-17i}));
    }

    SECTION("Different data complex same size") {
        auto x = ones<std::complex<double>>(16);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;

        auto y = ones<std::complex<double>>(16);
        y[0] = 3.0 - 4.0i;
        y[8] = 1.0 + 2.0i;

        const auto [f, Pxy] = Spectrum{}.window(windows::Hamming, 5).csd(x, y);

        REQUIRE(almost_equal(f, {0., 0.2, 0.4, -0.4, -0.2}));
        // print(Pxy);
        REQUIRE(almost_equal<120>(Pxy,
                                  {0.0212130325814537 - 0.0848521303258146i,
                                   0.2412993147862689 - 0.0598938467147468i,
                                   0.5759137177951847 - 0.003695125716331i,
                                   0.5759137177951847 - 0.003695125716331i,
                                   0.2412993147862689 - 0.0598938467147468i}));
    }
}

} // namespace scicpp::signal