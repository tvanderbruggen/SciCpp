// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "waveforms.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"

namespace scicpp::signal {

TEST_CASE("unit_impulse") {
    static_assert(unit_impulse<double, 5>().size() == 5);
    static_assert(unit_impulse<double, 4>(2).size() == 4);

    REQUIRE(almost_equal(unit_impulse<double, 5>(), {1., 0., 0., 0., 0.}));
    REQUIRE(almost_equal(unit_impulse<double, 4>(2), {0., 0., 1., 0.}));
    REQUIRE(almost_equal(unit_impulse<double, 4>(4), {0., 0., 0., 0.}));
    REQUIRE(almost_equal(unit_impulse<double, 0>(4), {}));

    REQUIRE(almost_equal(unit_impulse<double>(5), {1., 0., 0., 0., 0.}));
    REQUIRE(almost_equal(unit_impulse<double>(4, 2), {0., 0., 1., 0.}));
    REQUIRE(almost_equal(unit_impulse<double>(4, 4), {0., 0., 0., 0.}));
    REQUIRE(almost_equal(unit_impulse<double>(0, 4), {}));
}

TEST_CASE("sawtooth") {
    using namespace scicpp::operators;

    SECTION("std::array") {
        const auto res = sawtooth(2. * pi<double> * 5. * linspace<20>(0., 1.));
        // print(res);

        REQUIRE(almost_equal<8>(
            res, {-1.0000000000000000, -0.4736842105263158, 0.0526315789473684,
                  0.5789473684210527,  -0.8947368421052632, -0.3684210526315789,
                  0.1578947368421051,  0.6842105263157892,  -0.7894736842105263,
                  -0.2631578947368423, 0.2631578947368423,  0.7894736842105268,
                  -0.6842105263157897, -0.1578947368421063, 0.3684210526315783,
                  0.8947368421052628,  -0.5789473684210527, -0.0526315789473691,
                  0.4736842105263155,  -1.0000000000000000}));
    }

    SECTION("std::vector") {
        const auto t = 2. * pi<double> * 5. * linspace(0., 1., 20);
        const auto res = sawtooth(t);

        REQUIRE(almost_equal<8>(
            res, {-1.0000000000000000, -0.4736842105263158, 0.0526315789473684,
                  0.5789473684210527,  -0.8947368421052632, -0.3684210526315789,
                  0.1578947368421051,  0.6842105263157892,  -0.7894736842105263,
                  -0.2631578947368423, 0.2631578947368423,  0.7894736842105268,
                  -0.6842105263157897, -0.1578947368421063, 0.3684210526315783,
                  0.8947368421052628,  -0.5789473684210527, -0.0526315789473691,
                  0.4736842105263155,  -1.0000000000000000}));
    }
}

TEST_CASE("sweep_poly") {
    SECTION("std::array") {
        const std::array poly{2., 1.25, -0.36, .025};
        const auto t = linspace<10>(0., 10.);
        const auto res = sweep_poly(t, poly);
        // print(res);

        REQUIRE(almost_equal<2000>(res,
                                   {1.0000000000000000,
                                    0.5291475130785160,
                                    -0.6679198682512369,
                                    0.9257239692688911,
                                    0.6459001092455789,
                                    0.1802459804009975,
                                    0.8135520702629856,
                                    0.1575930433609465,
                                    0.8056830092586544,
                                    1.0000000000000000}));
    }

    SECTION("std::vector") {
        const std::array poly{2., 1.25, -0.36, .025};
        const auto t = linspace(0., 10., 10);
        const auto res = sweep_poly(t, poly);

        REQUIRE(almost_equal<2000>(res,
                                   {1.0000000000000000,
                                    0.5291475130785160,
                                    -0.6679198682512369,
                                    0.9257239692688911,
                                    0.6459001092455789,
                                    0.1802459804009975,
                                    0.8135520702629856,
                                    0.1575930433609465,
                                    0.8056830092586544,
                                    1.0000000000000000}));
    }
}

} // namespace scicpp::signal