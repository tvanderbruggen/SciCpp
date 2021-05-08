// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "spectral.hpp"

#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/signal/windows.hpp"

namespace scicpp::signal {

TEST_CASE("welch") {

    // >>> x = np.linspace(1,10,100)
    // >>> sp.signal.welch(x, window=sp.signal.windows.hann(20))
    //returns
    // f = [0.  , 0.05, 0.1 , 0.15, 0.2 , 0.25, 0.3 , 0.35, 0.4 , 0.45, 0.5 ]
    // Pxx = [1.17174629e-30, 1.08566857e+00, 1.02172300e-01, 5.16909533e-03,
    //        6.70540964e-04, 1.27112807e-04, 2.83514674e-05, 6.50751930e-06,
    //        1.38567588e-06, 2.78448686e-07, 5.62118532e-08]

    const auto x = linspace(1.0, 10.0, 100);
    const auto w = windows::hann<double>(20);
    // print(w);
    const auto [f, Pxx] = welch(x, 1.0, w);
    REQUIRE(almost_equal(
        f, {0., 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5}));
    print(Pxx);
}

} // namespace scicpp::signal