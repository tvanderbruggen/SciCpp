// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "waveforms.hpp"

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
        auto t = 2. * M_PI * 5. * linspace<20>(0., 1.);
        const auto res = sawtooth(t);
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
        auto t = 2. * M_PI * 5. * linspace(0., 1., 20);
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

} // namespace scicpp::signal