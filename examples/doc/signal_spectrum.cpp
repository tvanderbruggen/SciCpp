// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <scicpp/core.hpp>
#include <scicpp/signal.hpp>

int main() {
    namespace sci = scicpp;
    using namespace sci::operators;
    using namespace sci::units::literals;
    using namespace std::literals::complex_literals;

    const auto N = 1E5;
    const auto fs = 1E3;
    const auto omega0 = 2_rad * sci::pi<double> * 100;

    const auto t = sci::arange(0.0, N) / fs;
    const auto noise = sci::random::randn<double>(t.size());
    const auto x = sci::cos(omega0 * t) + noise;
    const auto y = sci::signal::sawtooth(omega0.value() * t) + 5.0i * noise;

    auto spec = sci::signal::Spectrum{}.fs(fs).window(
        sci::signal::windows::Hamming, t.size());

    const auto [f1, Pxx] = spec.welch(x);
    sci::print(Pxx);

    const auto [f2, Pyy] =
        spec.welch<sci::signal::SpectrumScaling::SPECTRUM>(y);
    sci::print(Pyy);

    const auto [f3, Pxy] = spec.csd(x, y);
    sci::print(Pxy);

    const auto [f4, Cxy] = spec.coherence(1.0i * x, y);
    sci::print(Cxy);

    return 0;
}