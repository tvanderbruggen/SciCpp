// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "spectral.hpp"

#include "scicpp/core/print.hpp"
#include "scicpp/core/random.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/core/units/units.hpp"
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

        // Don't return frequencies
        const auto p3 = Spectrum{}
                            .window(windows::hann<double>(20))
                            .welch<DENSITY, false>(std::array<double, 0>{});
        REQUIRE(p3.empty());
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

        // Don't return frequencies
        const auto p6 = Spectrum{}
                            .window(windows::gaussian(16, 2.0))
                            .welch<DENSITY, false>(x);
        REQUIRE(almost_equal<8>(p6,
                                {0.033283705527078,
                                 0.1552473685118426,
                                 0.3652273927481822,
                                 0.4944119749912214,
                                 0.5275115073459213,
                                 0.5288622631786398,
                                 0.5309738347484061,
                                 0.5290399525770682,
                                 0.2654870587529604}));
    }

    SECTION("Complex") {
        auto x = zeros<std::complex<double>>(16);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;

        auto spec = Spectrum{}.window(windows::hann<double>(8));
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

TEST_CASE("noverlap") {
    auto x = zeros<std::complex<double>>(16);
    x[0] = 1.0 + 2.0i;
    x[8] = 1.0 + 2.0i;
    auto spec =
        Spectrum{}.window(windows::hann<double>(8)).fs(10.0).noverlap(4);
    const auto [f, p] = spec.welch(x);
    // print(f);
    // print(p);
    REQUIRE(almost_equal(f, {0., 1.25, 2.5, 3.75, -5., -3.75, -2.5, -1.25}));
    REQUIRE(almost_equal<8>(p,
                            {0.04101368441119,
                             0.0412438689658457,
                             0.0552180305942083,
                             0.0576813628666607,
                             0.0573600418529239,
                             0.0576813628666607,
                             0.0552180305942083,
                             0.0412438689658457}));
    spec.noverlap(0);
    const auto [f1, p1] = spec.welch(x);
    // print(f1);
    print(p1);
    REQUIRE(almost_equal(f1, {0., 1.25, 2.5, 3.75, -5., -3.75, -2.5, -1.25}));
    REQUIRE(almost_equal<128>(p1,
                              {3.6458333333333336e-02,
                               1.2876444549389519e-02,
                               1.3566428021272650e-04,
                               8.7245037310924216e-06,
                               0.0000000000000000e+00,
                               8.7245037310924064e-06,
                               1.3566428021272650e-04,
                               1.2876444549389514e-02}));
}

TEST_CASE("periodogram") {
    auto x = zeros<std::complex<double>>(16);
    x[0] = 1.0 + 2.0i;
    x[8] = 1.0 + 2.0i;
    auto spec = Spectrum{}.window(windows::hann<double>(16)).fs(10.0);
    const auto [f, p] = spec.periodogram(x);
    // print(f);
    // print(p);
    REQUIRE(almost_equal(f,
                         {0.,
                          0.625,
                          1.25,
                          1.875,
                          2.5,
                          3.125,
                          3.75,
                          4.375,
                          -5.,
                          -4.375,
                          -3.75,
                          -3.125,
                          -2.5,
                          -1.875,
                          -1.25,
                          -0.625}));
    REQUIRE(almost_equal<1500>(p,
                               {0.0002364317230476,
                                0.0218936985942652,
                                0.0830806633107641,
                                0.0882007605538029,
                                0.0864386074072716,
                                0.0871837823196038,
                                0.0868713552374593,
                                0.0869767748617318,
                                0.0869570651175314,
                                0.0869767748617318,
                                0.0868713552374593,
                                0.0871837823196038,
                                0.0864386074072716,
                                0.0882007605538029,
                                0.083080663310764,
                                0.0218936985942652}));
}

TEST_CASE("value for iterables") {
    using namespace units::literals;

    SECTION("vector complex units") {
        auto x = zeros<std::complex<units::volt<double>>>(4);
        x[0] = std::complex(1_V, 2_V);
        x[3] = std::complex(1_V, 2_V);
        REQUIRE(almost_equal(detail::value(x), {1. + 2.i, 0., 0., 1. + 2.i}));
    }

    SECTION("array complex units") {
        auto x = zeros<4, std::complex<units::volt<double>>>();
        x[0] = std::complex(1_V, 2_V);
        x[3] = std::complex(1_V, 2_V);
        REQUIRE(almost_equal(detail::value(x), {1. + 2.i, 0., 0., 1. + 2.i}));
    }

    SECTION("array units") {
        auto x = ones<4, units::volt<double>>();
        x[0] = 0_V;
        x[3] = 2_V;
        REQUIRE(almost_equal(detail::value(x), {0., 1., 1., 2.}));
    }

    SECTION("vector complex doubles") {
        auto x = zeros<std::complex<double>>(4);
        x[0] = 1. + 2.i;
        x[3] = 1. + 2.i;
        REQUIRE(almost_equal(detail::value(x), {1. + 2.i, 0., 0., 1. + 2.i}));
    }
}

TEST_CASE("detail::element_type_t") {
    static_assert(
        std::is_same_v<detail::element_type_t<std::vector<double>>, double>);
    static_assert(std::is_same_v<
                  detail::element_type_t<std::vector<std::complex<double>>>,
                  std::complex<double>>);
    static_assert(
        std::is_same_v<detail::element_type_t<std::vector<units::volt<double>>>,
                       double>);
    static_assert(
        std::is_same_v<detail::element_type_t<
                           std::vector<std::complex<units::volt<double>>>>,
                       std::complex<double>>);
}

TEST_CASE("periodogram physical quantities") {
    using namespace units::literals;

    auto x = zeros<std::complex<units::volt<double>>>(16);
    x[0] = std::complex(1_V, 2_V);
    x[8] = std::complex(1_V, 2_V);
    auto spec = Spectrum{}.window(windows::hann<double>(16)).fs(10.0);
    const auto [f, p] = spec.periodogram(x);
    // print(f);
    // print(p);
    // TODO return proper PSD units
    REQUIRE(almost_equal(f,
                         {0.,
                          0.625,
                          1.25,
                          1.875,
                          2.5,
                          3.125,
                          3.75,
                          4.375,
                          -5.,
                          -4.375,
                          -3.75,
                          -3.125,
                          -2.5,
                          -1.875,
                          -1.25,
                          -0.625}));
    REQUIRE(almost_equal<1500>(p,
                               {0.0002364317230476,
                                0.0218936985942652,
                                0.0830806633107641,
                                0.0882007605538029,
                                0.0864386074072716,
                                0.0871837823196038,
                                0.0868713552374593,
                                0.0869767748617318,
                                0.0869570651175314,
                                0.0869767748617318,
                                0.0868713552374593,
                                0.0871837823196038,
                                0.0864386074072716,
                                0.0882007605538029,
                                0.083080663310764,
                                0.0218936985942652}));
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

        // Don't return frequencies
        const auto p4 = Spectrum{}
                            .window(windows::hann<double>(20))
                            .csd<DENSITY, false>(empty<std::complex<double>>(),
                                                 empty<std::complex<double>>());
        REQUIRE(p4.empty());
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
        REQUIRE(almost_equal<60>(
            Pxy,
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

    SECTION("Different data real different sizes") {
        const auto x = linspace(1.0, 10.0, 100);
        const auto y = linspace(10.0, 100.0, 50);

        const auto [f1, Pxy1] =
            Spectrum{}.window(windows::Hamming, 10).csd(x, y);
        REQUIRE(almost_equal(f1, {0., 0.1, 0.2, 0.3, 0.4, 0.5}));
        // print(Pxy1);
        REQUIRE(almost_equal<8000>(
            Pxy1,
            {5.4059942258228820e-17 + 0.0000000000000000e+00i,
             -8.7635981684350106e-01 + 7.2394256646839600e-02i,
             -2.6259506889322487e-01 + 6.7953763655904959e-03i,
             -3.3190773877356888e-02 + 3.0108962423992173e-04i,
             5.2777898636230183e-02 - 1.3624248523929803e-04i,
             -2.4445158559955788e-02 + 0.0000000000000000e+00i}));

        const auto [f2, Pxy2] =
            Spectrum{}.window(windows::Hamming, 10).csd(y, x);
        REQUIRE(almost_equal(f2, {0., 0.1, 0.2, 0.3, 0.4, 0.5}));
        // print(Pxy2);
        REQUIRE(almost_equal<4000>(
            Pxy2,
            {5.4059942258228820e-17 - 0.0000000000000000e+00i,
             -8.7635981684350106e-01 - 7.2394256646839600e-02i,
             -2.6259506889322487e-01 - 6.7953763655904959e-03i,
             -3.3190773877356888e-02 - 3.0108962423992173e-04i,
             5.2777898636230183e-02 + 1.3624248523929803e-04i,
             -2.4445158559955788e-02 - 0.0000000000000000e+00i}));
    }

    SECTION("Different data complex different sizes") {
        auto x = ones<std::complex<double>>(32);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;

        auto y = ones<std::complex<double>>(16);
        y[0] = 3.0 - 4.0i;
        y[8] = 1.0 + 2.0i;

        const auto [f1, Pxy1] =
            Spectrum{}.window(windows::Bartlett, 13).csd(y, x);
        REQUIRE(almost_equal<4>(f1,
                                {0.,
                                 0.0769230769230769,
                                 0.1538461538461539,
                                 0.2307692307692308,
                                 0.3076923076923077,
                                 0.3846153846153846,
                                 0.4615384615384616,
                                 -0.4615384615384616,
                                 -0.3846153846153846,
                                 -0.3076923076923077,
                                 -0.2307692307692308,
                                 -0.1538461538461539,
                                 -0.0769230769230769}));
        // print(Pxy1);
        REQUIRE(almost_equal<3500>(Pxy1,
                                   {-0.0665207640971602 - 0.0021074815595363i,
                                    0.0185925672847097 - 0.1586210297470785i,
                                    0.1738508231858531 + 0.0074374274437242i,
                                    0.1545122142491694 - 0.0103581816814315i,
                                    0.1533477377549926 + 0.010390531092891i,
                                    0.1610379640385271 + 0.001504480463977i,
                                    0.1458280372860636 + 0.0032737369473258i,
                                    0.1628768868055647 - 0.0002224288904522i,
                                    0.1469470638620797 - 0.0114619371871442i,
                                    0.1593548545737285 + 0.0068255793857207i,
                                    0.1632379438836161 - 0.002336314413126i,
                                    0.1370825218117011 + 0.0116179110243874i,
                                    0.2411693253358654 + 0.0801307664814734i}));
    }

    SECTION("Different data complex/real different sizes") {
        auto x = ones<std::complex<double>>(32);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;
        const auto y = linspace(10.0, 100.0, 50);

        const auto [f1, Pxy1] =
            Spectrum{}.window(windows::Bartlett, 13).csd(x, y);
        REQUIRE(almost_equal<4>(f1,
                                {0.,
                                 0.0769230769230769,
                                 0.1538461538461539,
                                 0.2307692307692308,
                                 0.3076923076923077,
                                 0.3846153846153846,
                                 0.4615384615384616,
                                 -0.4615384615384616,
                                 -0.3846153846153846,
                                 -0.3076923076923077,
                                 -0.2307692307692308,
                                 -0.1538461538461539,
                                 -0.0769230769230769}));
        // print(Pxy1);
        REQUIRE(almost_equal<256>(
            Pxy1,
            {4.5899691627736384e-16 - 1.0580603772799155e-15i,
             -1.5374895513987188e-01 - 2.7174310487694364e-01i,
             7.7525577007089411e-02 - 7.3191317828691510e-02i,
             -4.6070360984770402e-02 + 3.2017729284491109e-03i,
             6.4549493964843843e-02 + 1.6030670653229852e-02i,
             2.1013846663884429e-02 - 4.5034875471981255e-02i,
             -5.4063321996755460e-02 + 4.3639774581631458e-02i,
             2.5510092605947048e-02 + 2.8442942529853046e-02i,
             -8.0471846217143039e-03 - 4.0672902840938698e-02i,
             -6.6374930605366186e-02 + 6.2200889109303395e-02i,
             2.6102780546140986e-02 - 6.3420054018396312e-04i,
             -1.8162314703274152e-03 - 2.0310354286173296e-01i,
             -1.3292271576604644e+00 - 9.9103993540715474e-01i}));

        const auto [f2, Pxy2] =
            Spectrum{}.window(windows::Bartlett, 13).csd(y, x);
        REQUIRE(almost_equal<4>(f2,
                                {0.,
                                 0.0769230769230769,
                                 0.1538461538461539,
                                 0.2307692307692308,
                                 0.3076923076923077,
                                 0.3846153846153846,
                                 0.4615384615384616,
                                 -0.4615384615384616,
                                 -0.3846153846153846,
                                 -0.3076923076923077,
                                 -0.2307692307692308,
                                 -0.1538461538461539,
                                 -0.0769230769230769}));
        // print(Pxy2);
        REQUIRE(almost_equal<256>(
            Pxy2,
            {4.5899691627736384e-16 + 1.0580603772799155e-15i,
             -1.5374895513987188e-01 + 2.7174310487694364e-01i,
             7.7525577007089411e-02 + 7.3191317828691510e-02i,
             -4.6070360984770402e-02 - 3.2017729284491109e-03i,
             6.4549493964843843e-02 - 1.6030670653229852e-02i,
             2.1013846663884429e-02 + 4.5034875471981255e-02i,
             -5.4063321996755460e-02 - 4.3639774581631458e-02i,
             2.5510092605947048e-02 - 2.8442942529853046e-02i,
             -8.0471846217143039e-03 + 4.0672902840938698e-02i,
             -6.6374930605366186e-02 - 6.2200889109303395e-02i,
             2.6102780546140986e-02 + 6.3420054018396312e-04i,
             -1.8162314703274152e-03 + 2.0310354286173296e-01i,
             -1.3292271576604644e+00 + 9.9103993540715474e-01i}));
    }
}

TEST_CASE("coherence") {
    SECTION("empty") {
        const auto x = empty<double>();
        const auto [f, Cxx] = Spectrum{}.coherence(x, x);
        REQUIRE(f.empty());
        REQUIRE(Cxx.empty());
    }

    SECTION("real signals") {
        const auto x = linspace(1.0, 10.0, 50);
        const auto y = linspace(10.0, 100.0, 50);

        const auto [f, Cxx] =
            Spectrum{}.window(windows::Bartlett, 13).coherence(x, x);
        REQUIRE(almost_equal<4>(f,
                                {0.,
                                 0.0769230769230769,
                                 0.1538461538461539,
                                 0.2307692307692308,
                                 0.3076923076923077,
                                 0.3846153846153846,
                                 0.4615384615384616}));
        // print(Cxx);
        REQUIRE(almost_equal<16>(Cxx, ones<double>(f.size())));

        const auto [_, Cxy] =
            Spectrum{}.window(windows::Bartlett, 13).coherence(x, y);
        // print(Cxy);
        REQUIRE(almost_equal<12>(Cxy,
                                 {0.0249133674714442,
                                  0.9999999999999998,
                                  0.9999999999999998,
                                  1.0000000000000002,
                                  1.0000000000000002,
                                  1.,
                                  1.0000000000000002}));
    }

    SECTION("complex signals") {
        auto x = ones<std::complex<double>>(16);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;

        auto y = ones<std::complex<double>>(16);
        y[0] = 3.0 + 4.0i;
        y[8] = 1.0 + 2.0i;

        const auto [f, Cxy] =
            Spectrum{}.window(windows::Flattop, 5).coherence(x, y);
        REQUIRE(almost_equal(f, {0., 0.2, 0.4, -0.4, -0.2}));
        // print(Cxy);
        REQUIRE(almost_equal<6>(Cxy,
                                {0.9272167446777885,
                                 0.9157842754322916,
                                 0.8963246361527033,
                                 0.8963246361527033,
                                 0.9157842754322917}));
    }
}

TEST_CASE("tfestimate") {
    SECTION("empty") {
        const auto x = empty<double>();
        const auto [f, Txx] = Spectrum{}.tfestimate(x, x);
        REQUIRE(f.empty());
        REQUIRE(Txx.empty());
    }

    SECTION("real signals") {
        const auto x = linspace(1.0, 10.0, 50);
        const auto [f, Txx] =
            Spectrum{}.window(windows::Bartlett, 13).tfestimate(x, x);
        // print(f);
        REQUIRE(almost_equal<4>(f,
                                {0.,
                                 0.0769230769230769,
                                 0.1538461538461539,
                                 0.2307692307692308,
                                 0.3076923076923077,
                                 0.3846153846153846,
                                 0.4615384615384616}));
        // print(Txx);
        REQUIRE(almost_equal<16>(Txx, ones<std::complex<double>>(7)));
    }
}

TEST_CASE("welch parallel") {
    SECTION("Real") {
        auto x = zeros<double>(16);
        x[0] = 1.0;
        x[8] = 1.0;
        print(x);
        const auto [f1, p1] =
            Spectrum{}.window(windows::hann<double>(8)).nthreads(2).welch(x);
        REQUIRE(almost_equal(f1, linspace(0., 0.5, 5)));
        print(p1);
        REQUIRE(almost_equal<8>(p1,
                                {0.08202736882238,
                                 0.1649754758633829,
                                 0.220872122376833,
                                 0.2307254514666428,
                                 0.1147200837058478}));
    }
}

TEST_CASE("csd parallel") {
    SECTION("Different data real same size") {
        const auto x = linspace(1.0, 10.0, 100);
        const auto y = linspace(10.0, 100.0, 100);
        const auto [f, Pxy] =
            Spectrum{}.window(windows::Hamming, 15).nthreads(2).csd(x, y);

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
        REQUIRE(almost_equal<60>(
            Pxy,
            {-3.7062851089728351e-30 + 0.0000000000000000e+00i,
             5.5638353667040628e+00 - 3.1311659646683040e-15i,
             1.3896361537945315e-01 + 2.8720474849532764e-16i,
             1.9363830151308823e-03 + 7.8567939380376008e-18i,
             9.1683760095253690e-03 + 3.2446435506274200e-17i,
             1.0622173917597896e-02 + 5.6852044063349557e-17i,
             1.0516902037792620e-02 - 8.7090083435688860e-17i,
             1.0293880420302366e-02 + 2.1149639898068783e-17i}));
    }

    SECTION("Different data complex different sizes") {
        auto x = ones<std::complex<double>>(32);
        x[0] = 1.0 + 2.0i;
        x[8] = 1.0 + 2.0i;

        auto y = ones<std::complex<double>>(16);
        y[0] = 3.0 - 4.0i;
        y[8] = 1.0 + 2.0i;

        const auto [f1, Pxy1] =
            Spectrum{}.window(windows::Bartlett, 13).nthreads(2).csd(y, x);
        REQUIRE(almost_equal<4>(f1,
                                {0.,
                                 0.0769230769230769,
                                 0.1538461538461539,
                                 0.2307692307692308,
                                 0.3076923076923077,
                                 0.3846153846153846,
                                 0.4615384615384616,
                                 -0.4615384615384616,
                                 -0.3846153846153846,
                                 -0.3076923076923077,
                                 -0.2307692307692308,
                                 -0.1538461538461539,
                                 -0.0769230769230769}));
    }
}

} // namespace scicpp::signal