// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "fft.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/random.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/signal/windows.hpp"

namespace scicpp::signal {

TEST_CASE("Forward complex FFT") {
    SECTION("Complex input vector") {
        std::vector x{1. + 3.i, 2. + 2.i, 3. + 1.i};
        REQUIRE(almost_equal<2>(fft(x),
                                {6. + 6.i,
                                 -0.6339745962155614 + 2.3660254037844388i,
                                 -2.3660254037844388 + 0.6339745962155614i}));
        REQUIRE(almost_equal(fft(full(1, 1. + 1.i)), {1. + 1.i}));
    }

    SECTION("Real input vector") {
        std::vector x{1., 2., 3.};
        REQUIRE(almost_equal<2>(fft(x),
                                {6. + 0.i,
                                 -1.5 + 0.8660254037844386i,
                                 -1.5 - 0.8660254037844386i}));
        REQUIRE(almost_equal(fft(ones<double>(1)), {1. + 0.i}));
        REQUIRE(almost_equal(fft(zeros<double>(1)), {0. + 0.i}));
    }
}

TEST_CASE("Forward real FFT") {
    SECTION("Odd length") {
        std::vector x{1., 2., 3.};
        REQUIRE(
            almost_equal<2>(rfft(x), {6. + 0.i, -1.5 + 0.8660254037844386i}));
        REQUIRE(almost_equal(rfft(ones<double>(1)), {1. + 0.i}));
        REQUIRE(almost_equal(rfft(zeros<double>(1)), {0. + 0.i}));
    }

    SECTION("Even length") {
        auto x = zeros<double>(16);
        x[0] = 1.0;
        x[8] = 1.0;
        // print(rfft(x));
        REQUIRE(almost_equal(rfft(x),
                             {2. + 0.i,
                              0. + 0.i,
                              2. - 0.i,
                              0. + 0.i,
                              2. + 0.i,
                              0. + 0.i,
                              2. + 0.i,
                              0. + 0.i,
                              2. + 0.i}));
    }
}

TEST_CASE("Inverse real FFT") {
    SECTION("Test1") {
        std::vector y{1. + 0.i, -1.i, -1. + 0.i, 1.i};
        REQUIRE(almost_equal<15>(irfft(y),
                                 {-0.16666666666666666,
                                  0.6220084679281461,
                                  0.6220084679281461,
                                  -0.16666666666666666,
                                  0.04465819873852046,
                                  0.04465819873852046}));
    }

    SECTION("Test2") {
        // np.fft.irfft([1, -1j, -1])
        std::vector y{1. + 0.i, -1.i, -1. + 0.i};
        REQUIRE(almost_equal(irfft(y), {0., 1., 0., 0.}));
    }

    SECTION("Test3") {
        // np.fft.irfft([1, -1j, -1], n=3)
        std::vector y{1. + 0.i, -1.i, -1. + 0.i};

        REQUIRE(almost_equal<4>(
            irfft(y, 3),
            {0.3333333333333333, 0.910683602522959, -0.2440169358562924}));
    }
}

TEST_CASE("Inverse complex FFT") {
    SECTION("Complex input vector") {
        std::vector y{1. + 3.i, 2. + 2.i, 3. + 1.i};
        REQUIRE(almost_equal<5>(ifft(y),
                                {2. + 2.i,
                                 -0.7886751345948131 + 0.21132486540518697i,
                                 -0.21132486540518697 + 0.7886751345948131i}));
        // print(ifft(y, 5));
        REQUIRE(almost_equal<300>(ifft(y, 5),
                                  {1.2 + 1.2i,
                                   -0.6597830558515456 + 1.2948971567685348i,
                                   0.016900601217030943 + 0.002676792264907668i,
                                   0.10670619653294805 + 0.6737164099851134i,
                                   0.3361762581015666 - 0.17129035901855583i}));
    }

    SECTION("Real input vector") {
        std::vector y{1., 2., 3.};
        // print(ifft(y));
        REQUIRE(almost_equal<5>(
            ifft(y),
            {2. + 0.i, -0.5 - 0.288675134594813i, -0.5 + 0.288675134594813i}));
        // print(ifft(y, 6));
        REQUIRE(almost_equal<10>(ifft(y, 6),
                                 {1. + 0.i,
                                  0.08333333333333334 + 0.7216878364870323i,
                                  -0.25 - 0.14433756729740643i,
                                  0.3333333333333333 - 1.4802973661668753e-16i,
                                  -0.25 + 0.14433756729740643i,
                                  0.08333333333333334 - 0.721687836487032i}));
    }
}

TEST_CASE("fftfreq") {
    REQUIRE(almost_equal(
        fftfreq<4>(3.14),
        {0., 0.07961783439490445, -0.1592356687898089, -0.07961783439490445}));
    REQUIRE(almost_equal(fftfreq<5>(3.14),
                         {0.,
                          0.06369426751592357,
                          0.12738853503184713,
                          -0.12738853503184713,
                          -0.06369426751592357}));
    REQUIRE(almost_equal(
        fftfreq(4, 3.14),
        {0., 0.07961783439490445, -0.1592356687898089, -0.07961783439490445}));
    REQUIRE(almost_equal(fftfreq(5, 3.14),
                         {0.,
                          0.06369426751592357,
                          0.12738853503184713,
                          -0.12738853503184713,
                          -0.06369426751592357}));
}

TEST_CASE("rfftfreq") {
    REQUIRE(almost_equal(rfftfreq<10>(2.0), {0., 0.05, 0.1, 0.15, 0.2, 0.25}));
    REQUIRE(almost_equal(rfftfreq<15>(2.0),
                         {0.,
                          0.03333333333333333,
                          0.06666666666666666,
                          0.1,
                          0.13333333333333333,
                          0.16666666666666666,
                          0.2,
                          0.23333333333333333}));
    REQUIRE(almost_equal(rfftfreq(10, 2.0), {0., 0.05, 0.1, 0.15, 0.2, 0.25}));
    REQUIRE(almost_equal(rfftfreq(15, 2.0),
                         {0.,
                          0.03333333333333333,
                          0.06666666666666666,
                          0.1,
                          0.13333333333333333,
                          0.16666666666666666,
                          0.2,
                          0.23333333333333333}));
}

TEST_CASE("fftshift") {
    SECTION("std::array") {
        SECTION("Even size") {
            REQUIRE(almost_equal(fftshift(fftfreq<4>(3.14)),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));

            const auto a = fftfreq<4>(3.14);
            REQUIRE(almost_equal(fftshift(a),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));
        }

        SECTION("Odd size") {
            REQUIRE(almost_equal(fftshift(fftfreq<5>(3.14)),
                                 {-0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357,
                                  0.12738853503184713}));

            const auto a = fftfreq<5>(3.14);
            REQUIRE(almost_equal(fftshift(a),
                                 {-0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357,
                                  0.12738853503184713}));
        }
    }

    SECTION("std::vector") {
        SECTION("Even size") {
            REQUIRE(almost_equal(fftshift(fftfreq(4, 3.14)),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));

            const auto a = fftfreq(4, 3.14);
            REQUIRE(almost_equal(fftshift(a),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));
        }

        SECTION("Odd size") {
            REQUIRE(almost_equal(fftshift(fftfreq(5, 3.14)),
                                 {-0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357,
                                  0.12738853503184713}));

            const auto a = fftfreq(5, 3.14);
            REQUIRE(almost_equal(fftshift(a),
                                 {-0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357,
                                  0.12738853503184713}));
        }
    }
}

TEST_CASE("ifftshift") {
    SECTION("std::array") {
        SECTION("Even size") {
            REQUIRE(almost_equal(ifftshift(fftfreq<4>(3.14)),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));

            const auto a = fftfreq<4>(3.14);
            REQUIRE(almost_equal(ifftshift(a),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));
        }
        SECTION("Odd size") {
            REQUIRE(almost_equal(ifftshift(fftfreq<5>(3.14)),
                                 {0.12738853503184713,
                                  -0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357}));

            const auto a = fftfreq<5>(3.14);
            REQUIRE(almost_equal(ifftshift(a),
                                 {0.12738853503184713,
                                  -0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357}));
        }
    }

    SECTION("std::vector") {
        SECTION("Even size") {
            REQUIRE(almost_equal(ifftshift(fftfreq(4, 3.14)),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));

            const auto a = fftfreq(4, 3.14);
            REQUIRE(almost_equal(ifftshift(a),
                                 {-0.1592356687898089,
                                  -0.07961783439490445,
                                  0.,
                                  0.07961783439490445}));
        }

        SECTION("Odd size") {
            REQUIRE(almost_equal(ifftshift(fftfreq(5, 3.14)),
                                 {0.12738853503184713,
                                  -0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357}));

            const auto a = fftfreq(5, 3.14);
            REQUIRE(almost_equal(ifftshift(a),
                                 {0.12738853503184713,
                                  -0.12738853503184713,
                                  -0.06369426751592357,
                                  0.,
                                  0.06369426751592357}));
        }
    }
}

TEST_CASE("next_fast_len") {
    REQUIRE(next_fast_len(852U) == 864U);
    REQUIRE(next_fast_len(78954651U) == 79626240U);
}

TEST_CASE("zero_padding") {
    const std::vector v{1., 2., 3., 4., 5.};
    REQUIRE(almost_equal(zero_padding(v, 3), {1., 2., 3.}));
    REQUIRE(almost_equal(zero_padding(v, 7.), {1., 2., 3., 4., 5., 0., 0.}));

    const std::array a{1., 2., 3., 4., 5.};
    REQUIRE(almost_equal(zero_padding(a, 3), {1., 2., 3.}));
    REQUIRE(almost_equal(zero_padding(a, 7.), {1., 2., 3., 4., 5., 0., 0.}));

    REQUIRE(almost_equal(zero_padding(std::vector{1., 2., 3., 4., 5.}, 3),
                         {1., 2., 3.}));
    REQUIRE(almost_equal(zero_padding(std::vector{1., 2., 3., 4., 5.}, 7.),
                         {1., 2., 3., 4., 5., 0., 0.}));
}

TEST_CASE("power_spectrum_density") {
    // Generate a Gaussian white-noise with sigma = 1
    const auto noise = random::randn<double>(10000);
    const auto psd = power_spectrum_density(noise, 1., windows::Window::Boxcar);
    // The expected PSD for a white noise is 2 * sigma ^ 2
    REQUIRE(std::fabs(stats::mean(psd) - 2.) / 2. < 1. / std::sqrt(10000));
}

} // namespace scicpp::signal
