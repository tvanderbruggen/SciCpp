// SPDX-License-Identifier: MIT

#include "filtering.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/signal/convolve.hpp"

namespace scicpp::signal {

TEST_CASE("lfilter_zi") {
    SECTION("std::vector 1") {
        const auto a = std::vector{1., 2., 3.};
        const auto b = std::vector{4., 5., 6.};
        const auto zi = lfilter_zi(b, a);
        // print(zi);
        REQUIRE(almost_equal<10>(zi, {-1.5, -1.5}));
    }

    SECTION("std::vector 2") {
        const auto a = std::vector{0., 0., 0., 1., 2., 3.};
        const auto b = std::vector{4., 5., 6.};
        const auto zi = lfilter_zi(b, a);
        // print(zi);
        REQUIRE(almost_equal<10>(zi, {-1.5, -1.5}));
    }

    SECTION("std::vector 3") {
        const auto a = std::vector{1., 10.};
        const auto b = std::vector{10., 20., 30.};
        const auto zi = lfilter_zi(b, a);
        // print(zi);
        REQUIRE(almost_equal<1>(zi, {-4.545454545454545454545454, 30.}));
    }

    SECTION("std::vector 4") {
        const auto a = std::vector{0., 1., 10., 100., 1000.};
        const auto b = std::vector{10., 20., 30.};
        const auto zi = lfilter_zi(b, a);
        // print(zi);
        REQUIRE(almost_equal<10>(
            zi, {-9.945994599459945, -29.40594059405941, -54.005400540054005}));
    }

    SECTION("std::array") {
        const auto a = std::array{1., 10., 100., 1000.};
        const auto b = std::array{10., 20., 30.};
        const auto zi = lfilter_zi(b, a);
        static_assert(zi.size() == 3);
        // print(zi);
        REQUIRE(almost_equal<10>(
            zi, {-9.945994599459945, -29.40594059405941, -54.005400540054005}));
    }
}

TEST_CASE("lfilter") {
    SECTION("std::vector - len(a) == 1 (1)") {
        const auto a = std::vector{1.};
        const auto b = std::vector{10., 20., 30.};
        const auto x = linspace(0., 100., 20);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<2>(y,
                                {0.,
                                 52.631578947368425,
                                 210.5263157894737,
                                 526.3157894736842,
                                 842.1052631578948,
                                 1157.8947368421054,
                                 1473.6842105263158,
                                 1789.4736842105265,
                                 2105.263157894737,
                                 2421.0526315789475,
                                 2736.842105263158,
                                 3052.6315789473683,
                                 3368.421052631579,
                                 3684.2105263157896,
                                 4000.0000000000005,
                                 4315.789473684211,
                                 4631.578947368421,
                                 4947.368421052632,
                                 5263.1578947368425,
                                 5578.947368421053}));
    }

    SECTION("std::vector - len(a) == 1 (2)") {
        const auto a = std::vector{8.};
        const auto b = std::vector{10., 20., 30.};
        const auto x = linspace(1., 10., 5);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<2>(y, {1.25, 6.5625, 18.75, 35.625, 52.5}));
    }

    SECTION("std::vector 1") {
        const auto a = std::vector{1., 10., 100.};
        const auto b = std::vector{8.};
        const auto x = linspace(1., 10., 5);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<2>(
            y, {8., -5.400e+01, -2.160e+02, 7.622e+03, -5.454e+04}));
    }

    SECTION("std::vector 2") {
        const auto a = std::vector{1., 10., 100., 1000.};
        const auto b = std::vector{10., 20., 30.};
        const auto x = linspace(0.0, 100.0, 20);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<2>(y,
                                {0.,
                                 5.2631578947368425e+01,
                                 -3.1578947368421052e+02,
                                 -1.5789473684210541e+03,
                                 -4.4210526315789357e+03,
                                 5.1905263157894736e+05,
                                 -3.1679999999999995e+06,
                                 -1.5802421052631602e+07,
                                 -4.4226315789473481e+07,
                                 5.1905076842105255e+09,
                                 -3.1680021473684204e+10,
                                 -1.5802423484210544e+11,
                                 -4.4226318505262958e+11,
                                 5.1905076812105258e+13,
                                 -3.1680021476968419e+14,
                                 -1.5802423484567385e+15,
                                 -4.4226318505648220e+15,
                                 5.1905076812101120e+17,
                                 -3.1680021476968858e+18,
                                 -1.5802423484567446e+19}));
    }

    SECTION("std::vector 3") {
        const auto a = std::vector{1., 10., 100.};
        const auto b = std::vector{8., 16.};
        const auto x = linspace(1., 10., 5);
        const auto zi = ones<double>(2);
        const auto [y, zf] = lfilter(b, a, x, zi);
        // print(y);
        // print(zf);
        REQUIRE(almost_equal<2>(
            y, {9., -4.7000e+01, -3.3400e+02, 8.1900e+03, -4.8296e+04}));
        REQUIRE(almost_equal<2>(zf, {-335880., 4829600.}));
    }

    SECTION("std::vector 4") {
        const auto a = std::vector{10.};
        const auto b = std::vector{8., 16.};
        const auto x = linspace(1., 10., 5);
        const auto zi = ones<double>(1);
        const auto [y, zf] = lfilter(b, a, x, zi);
        // print(y);
        // print(zf);
        REQUIRE(almost_equal<2>(y, {1.8, 4.2, 9.600000000000001, 15., 20.4}));
        REQUIRE(almost_equal<2>(zf, {16.}));
    }

    SECTION("std::vector 5") {
        const auto a = std::vector{10.};
        const auto b = std::vector{8., 16., 32.};
        const auto x = linspace(1., 10., 5);
        const auto zi = std::vector{2., 4.};
        const auto [y, zf] = lfilter(b, a, x, zi);
        // print(y);
        // print(zf);
        REQUIRE(almost_equal<2>(y, {2.8, 8.2, 12.8, 25.400000000000002, 38.}));
        REQUIRE(almost_equal<2>(zf, {40.8, 32.}));
    }

    SECTION("std::array - len(a) == 1 (2)") {
        const auto a = std::array{8.};
        const auto b = std::array{10., 20., 30.};
        const auto x = linspace<5>(1., 10.);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<2>(y, {1.25, 6.5625, 18.75, 35.625, 52.5}));
    }

    SECTION("std::array / std::vector") {
        const auto a = std::vector{8.};
        const auto b = std::vector{10., 20., 30.};
        const auto x = linspace<5>(1., 10.);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<2>(y, {1.25, 6.5625, 18.75, 35.625, 52.5}));
    }

    SECTION("std::array - len(a) == 1 (constexpr)") {
        constexpr auto a = std::array{8.};
        constexpr auto b = std::array{10., 20., 30.};
        constexpr auto x = std::array{1., 3.25, 5.5, 7.75, 10.};
        constexpr auto y = lfilter(b, a, x);
        // print(y);
        static_assert(float_equal(y[0], 1.25));
        static_assert(float_equal(y[1], 6.5625));
        static_assert(float_equal(y[2], 18.75));
        static_assert(float_equal(y[3], 35.625));
        static_assert(float_equal(y[4], 52.5));
    }

    SECTION("std::array 2") {
        const auto a = std::array{1., 10., 100., 1000.};
        const auto b = std::array{10., 20., 30.};
        const auto x = linspace<20>(0.0, 100.0);
        const auto y = lfilter(b, a, x);
        // print(y);
        REQUIRE(almost_equal<25>(y,
                                 {0.,
                                  5.2631578947368425e+01,
                                  -3.1578947368421052e+02,
                                  -1.5789473684210541e+03,
                                  -4.4210526315789357e+03,
                                  5.1905263157894736e+05,
                                  -3.1679999999999995e+06,
                                  -1.5802421052631602e+07,
                                  -4.4226315789473481e+07,
                                  5.1905076842105255e+09,
                                  -3.1680021473684204e+10,
                                  -1.5802423484210544e+11,
                                  -4.4226318505262958e+11,
                                  5.1905076812105258e+13,
                                  -3.1680021476968419e+14,
                                  -1.5802423484567385e+15,
                                  -4.4226318505648220e+15,
                                  5.1905076812101120e+17,
                                  -3.1680021476968858e+18,
                                  -1.5802423484567446e+19}));
    }

    SECTION("std::array 3") {
        constexpr auto a = std::array{1., 10., 100., 1000.};
        constexpr auto b = std::array{10., 20., 30.};
        constexpr auto x = linspace<20>(0.0, 100.0);
        constexpr auto y = lfilter(b, a, x);
        static_assert(y.size() == 20);
        static_assert(float_equal(y[0], 0.));
        static_assert(float_equal(y[1], 5.2631578947368425e+01));
    }

    SECTION("std::array 4") {
        constexpr auto a = std::array{10.};
        constexpr auto b = std::array{8., 16.};
        constexpr auto x = linspace<5>(1., 10.);
        constexpr auto zi = ones<1, double>();
        constexpr auto tup = lfilter(b, a, x, zi);
        constexpr auto y = std::get<0>(tup);
        constexpr auto zf = std::get<1>(tup);
        // print(y);
        // print(zf);
        static_assert(zf.size() == 1);
        static_assert(float_equal(zf[0], 16.));
        static_assert(y.size() == 5);
        static_assert(float_equal(y[0], 1.8));
        static_assert(float_equal(y[1], 4.2));
        static_assert(float_equal(y[2], 9.600000000000001));
        static_assert(float_equal(y[3], 15.));
        static_assert(float_equal(y[4], 20.4));
    }

    SECTION("std::array 5") {
        constexpr auto a = std::array{10.};
        constexpr auto b = std::array{8., 16., 32.};
        constexpr auto x = linspace<5>(1., 10.);
        constexpr auto zi = std::array{2., 4.};
        constexpr auto tup = lfilter(b, a, x, zi);
        constexpr auto y = std::get<0>(tup);
        constexpr auto zf = std::get<1>(tup);
        // print(y);
        // print(zf);
        REQUIRE(almost_equal<2>(y, {2.8, 8.2, 12.8, 25.400000000000002, 38.}));
        REQUIRE(almost_equal<2>(zf, {40.8, 32.}));
    }

    SECTION("std::array 6") {
        constexpr auto a = std::array{1., 10., 100.};
        constexpr auto b = std::array{8., 16.};
        constexpr auto x = linspace<5>(1., 10.);
        constexpr auto zi = ones<2, double>();
        constexpr auto tup = lfilter(b, a, x, zi);
        constexpr auto y = std::get<0>(tup);
        constexpr auto zf = std::get<1>(tup);
        // print(y);
        // print(zf);
        static_assert(y.size() == 5);
        static_assert(float_equal(y[0], 9.));
        static_assert(float_equal(y[1], -4.7000e+01));
        static_assert(float_equal(y[2], -3.3400e+02));
        static_assert(float_equal(y[3], 8.1900e+03));
        static_assert(float_equal(y[4], -4.8296e+04));
        static_assert(zf.size() == 2);
        static_assert(float_equal(zf[0], -335880.));
        static_assert(float_equal(zf[1], 4829600.));
    }
}

TEST_CASE("filtfilt") {
    SECTION("std::vector - len(a) == 1") {
        const auto a = std::vector{1.};
        const auto b = std::vector{10., 20., 30.};
        const auto x = linspace(10., 100., 20);
        const auto y = filtfilt(b, a, x);
        // print(y);
        REQUIRE(almost_equal<10>(y, {35999.99999999999,  53052.63157894737,
                                     70105.26315789473,  87157.8947368421,
                                     104210.52631578948, 121263.15789473683,
                                     138315.7894736842,  155368.42105263157,
                                     172421.05263157896, 189473.68421052632,
                                     206526.3157894737,  223578.94736842104,
                                     240631.57894736843, 257684.2105263158,
                                     274736.84210526315, 291789.47368421056,
                                     308842.10526315786, 325894.7368421053,
                                     342947.3684210526,  360000.}));
    }

    SECTION("std::vector") {
        const auto a = std::vector{1., 10., 100.};
        const auto b = std::vector{8., 16.};
        const auto x = linspace(1., 10., 10);
        const auto y = filtfilt(b, a, x);
        print(y);
        REQUIRE(almost_equal<400>(y,
                                  {-4.9970352380871936e+43,
                                   -1.1715611423427560e+43,
                                   1.6712646661514752e+42,
                                   -4.9970352380871911e+40,
                                   -1.1715611423427561e+40,
                                   1.6712646661514751e+39,
                                   -4.9970352380871903e+37,
                                   -1.1715611423427564e+37,
                                   1.6712646661514754e+36,
                                   -4.9970352380871888e+34}));
    }

    SECTION("std::array") {
        const auto a = std::array{1., 10., 100.};
        const auto b = std::array{8., 16.};
        const auto x = linspace<10>(1., 10.);
        const auto y = filtfilt(b, a, x);
        print(y);
        REQUIRE(almost_equal<400>(y,
                                  {-4.9970352380871936e+43,
                                   -1.1715611423427560e+43,
                                   1.6712646661514752e+42,
                                   -4.9970352380871911e+40,
                                   -1.1715611423427561e+40,
                                   1.6712646661514751e+39,
                                   -4.9970352380871903e+37,
                                   -1.1715611423427564e+37,
                                   1.6712646661514754e+36,
                                   -4.9970352380871888e+34}));
    }
}

TEST_CASE("deconvolve") {
    SECTION("std::vector") {
        const auto original = std::vector{0., 1., 0., 0., 1., 1., 0., 0.};
        const auto impulse_response = std::vector{2., 1.};
        const auto recorded = convolve(impulse_response, original);
        REQUIRE(
            almost_equal<1>(recorded, {0., 2., 1., 0., 2., 3., 1., 0., 0.}));
        const auto [recovered, remainder] =
            deconvolve(recorded, impulse_response);
        REQUIRE(almost_equal<1>(recovered, {0., 1., 0., 0., 1., 1., 0., 0.}));
        REQUIRE(
            almost_equal<1>(remainder, {0., 0., 0., 0., 0., 0., 0., 0., 0.}));
    }

    SECTION("std::vector 2") {
        const auto signal = std::vector{1.};
        const auto divisor = std::vector{1., 2.};
        const auto [recovered, remainder] = deconvolve(signal, divisor);
        REQUIRE(recovered.empty());
        REQUIRE(almost_equal(remainder, {1.}));
    }

    SECTION("std::array 1") {
        constexpr auto original = std::array{0., 1., 0., 0., 1., 1., 0., 0.};
        constexpr auto impulse_response = std::array{2., 1.};
        constexpr auto recorded = convolve(impulse_response, original);
        static_assert(recorded.size() == 9);
        static_assert(float_equal(recorded[0], 0.));
        static_assert(float_equal(recorded[1], 2.));
        static_assert(float_equal(recorded[2], 1.));
        static_assert(float_equal(recorded[3], 0.));
        static_assert(float_equal(recorded[4], 2.));
        static_assert(float_equal(recorded[5], 3.));
        static_assert(float_equal(recorded[6], 1.));
        static_assert(float_equal(recorded[7], 0.));
        static_assert(float_equal(recorded[8], 0.));

        constexpr auto res = deconvolve(recorded, impulse_response);
        constexpr auto recovered = std::get<0>(res);
        constexpr auto remainder = std::get<1>(res);

        static_assert(recovered.size() == 8);
        static_assert(float_equal(recovered[0], 0.));
        static_assert(float_equal(recovered[1], 1.));
        static_assert(float_equal(recovered[2], 0.));
        static_assert(float_equal(recovered[3], 0.));
        static_assert(float_equal(recovered[4], 1.));
        static_assert(float_equal(recovered[5], 1.));
        static_assert(float_equal(recovered[6], 0.));
        static_assert(float_equal(recovered[7], 0.));

        static_assert(remainder.size() == 9);
        static_assert(float_equal(remainder[0], 0.));
        static_assert(float_equal(remainder[1], 0.));
        static_assert(float_equal(remainder[2], 0.));
        static_assert(float_equal(remainder[3], 0.));
        static_assert(float_equal(remainder[4], 0.));
        static_assert(float_equal(remainder[5], 0.));
        static_assert(float_equal(remainder[6], 0.));
        static_assert(float_equal(remainder[7], 0.));
        static_assert(float_equal(remainder[8], 0.));
    }

    SECTION("std::array 2") {
        constexpr auto signal = std::array{1.};
        constexpr auto divisor = std::array{1., 2.};
        constexpr auto res = deconvolve(signal, divisor);
        constexpr auto recovered = std::get<0>(res);
        constexpr auto remainder = std::get<1>(res);
        static_assert(recovered.empty());
        static_assert(float_equal(remainder[0], 1.));
    }

    SECTION("std::vector / std::array") {
        const auto original = std::vector{0., 1., 0., 0., 1., 1., 0., 0.};
        const auto impulse_response = std::array{2., 1.};
        const auto recorded = convolve(impulse_response, original);
        REQUIRE(
            almost_equal<1>(recorded, {0., 2., 1., 0., 2., 3., 1., 0., 0.}));
        const auto [recovered, remainder] =
            deconvolve(recorded, impulse_response);
        REQUIRE(almost_equal<1>(recovered, {0., 1., 0., 0., 1., 1., 0., 0.}));
        REQUIRE(
            almost_equal<1>(remainder, {0., 0., 0., 0., 0., 0., 0., 0., 0.}));
    }
}

// TEST_CASE("Signal Tools") {
//     using namespace std::complex_literals;

//     const int N = 2;
//     const double Wn = 0.5;
//     const std::vector<int> x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// SECTION("LFilter ZI") {

//     auto ba = butter<BTYPE::LOWPASS, FOUTPUT::BA, double>(2, 0.5);
//     const auto result = lfilter_zi(ba.b, ba.a);
//     REQUIRE(almost_equal<1000000000>(result, {0.70710678, 0.12132034}));
// }
// SECTION("Raw Filter") {

//     auto ba = butter<BTYPE::LOWPASS, FOUTPUT::BA, double>(2, 0.5);
//     const auto zi = lfilter_zi(ba.b, ba.a);

//     auto [y, zf] =
//         detail::_raw_filter(ba.b, ba.a, {0.0, 1.0, 2.0, 3.0, 4.0}, zi);

//     REQUIRE(almost_equal<1000000000>(
//         y, {0.70710678, 0.41421356, 1.05025253, 2.27207794, 3.33452378}));
//     REQUIRE(almost_equal<1000000000>(zf, {2.83199846, 0.59945904}));
// }
// SECTION("FiltFilt") {

//     std::vector<double> x = {
//         0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
//     auto ba = butter<BTYPE::LOWPASS, FOUTPUT::BA, double>(2, 0.5);

//     auto y = filtfilt(ba.b, ba.a, x);
//     REQUIRE(almost_equal<1000000000>(y,
//                                      {-6.33606839e-05,
//                                       9.99973717e-01,
//                                       2.00001036e+00,
//                                       3.00000451e+00,
//                                       4.00000123e+00,
//                                       4.99999923e+00,
//                                       5.99998228e+00,
//                                       7.00000013e+00,
//                                       8.00010508e+00,
//                                       8.99999998e+00}));
// }
// }

} // namespace scicpp::signal
