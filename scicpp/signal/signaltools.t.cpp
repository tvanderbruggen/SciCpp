#include "signaltools.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/signal/filter_design.hpp"

namespace scicpp::signal {

TEST_CASE("detail::validate_pad") {
    SECTION("std::vector") {
        const auto x = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        const auto [ext, edge] = detail::validate_pad<PadType::ODD>(x, 3);
        REQUIRE(array_equal(ext, {-9, -8, -7, -6, -5, -4, -3, -2, -1, 0,
                                  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20}));
        REQUIRE(edge == 9);
    }

    SECTION("std::array") {
        const auto x = std::array{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        const auto [ext, edge] = detail::validate_pad<PadType::ODD>(x, 3);
        REQUIRE(array_equal(ext, {-9, -8, -7, -6, -5, -4, -3, -2, -1, 0,
                                  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                  11, 12, 13, 14, 15, 16, 17, 18, 19, 20}));
        REQUIRE(edge == 9);
    }

    SECTION("PadType::NONE") {
        const auto x = std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        const auto [ext, edge] = detail::validate_pad<PadType::NONE>(x, 3);
        REQUIRE(array_equal(ext, x));
        REQUIRE(edge == 0);
    }
}

TEST_CASE("lfilter_zi") {
    SECTION("std::vector 1") {
        const auto a = std::vector{1., 2., 3.};
        const auto b = std::vector{4., 5., 6.};
        const auto zi = lfilter_zi(b, a);
        print(zi);
        REQUIRE(almost_equal<10>(zi, {-1.5, -1.5}));
    }

    SECTION("std::vector 2") {
        const auto a = std::vector{0., 0., 0., 1., 2., 3.};
        const auto b = std::vector{4., 5., 6.};
        const auto zi = lfilter_zi(b, a);
        print(zi);
        REQUIRE(almost_equal<10>(zi, {-1.5, -1.5}));
    }

    SECTION("std::vector 3") {
        const auto a = std::vector{1., 10.};
        const auto b = std::vector{10., 20., 30.};
        const auto zi = lfilter_zi(b, a);
        print(zi);
        REQUIRE(almost_equal<1>(zi, {-4.545454545454545454545454, 30.}));
    }

    SECTION("std::vector 4") {
        const auto a = std::vector{0., 1., 10., 100., 1000.};
        const auto b = std::vector{10., 20., 30.};
        const auto zi = lfilter_zi(b, a);
        print(zi);
        REQUIRE(almost_equal<10>(
            zi, {-9.945994599459945, -29.40594059405941, -54.005400540054005}));
    }

    SECTION("std::array") {
        const auto a = std::array{1., 10., 100., 1000.};
        const auto b = std::array{10., 20., 30.};
        const auto zi = lfilter_zi(b, a);
        print(zi);
        REQUIRE(almost_equal<10>(
            zi, {-9.945994599459945, -29.40594059405941, -54.005400540054005}));
    }
}

// TEST_CASE("Signal Tools") {
//     using namespace std::complex_literals;

//     const int N = 2;
//     const double Wn = 0.5;
//     const std::vector<int> x = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

// SECTION("Companion") {
//     const std::vector<double> a = {1., 2., 3., 4., 5.};
//     const auto result = detail::companion(a);
//     REQUIRE(almost_equal<1>(result,
//                             {{-2., -3., -4., -5.},
//                              {1., 0., 0., 0.},
//                              {0., 1., 0., 0.},
//                              {0., 0., 1., 0.}}));
// }
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
