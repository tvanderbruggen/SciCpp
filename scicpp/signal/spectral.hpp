// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_SPECTRAL
#define SCICPP_SIGNAL_SPECTRAL

#include "scicpp/core/functional.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/signal/fft.hpp"

#include <algorithm>
#include <tuple>

namespace scicpp::signal {

template <class Array, typename T = typename Array::value_type>
auto welch(const Array &a, T fs, const Array &w) {
    using namespace scicpp::operators;

    const auto asize = signed_size_t(a.size());

    // S2 = Sum_i w_i^2
    const auto S2 =
        std::get<0>(reduce(w, [](auto r, auto v) { return r + v * v; }, T{0}));

    //Default values
    const auto nperseg = signed_size_t(w.size());
    const auto noverlap = nperseg / 2;
    const auto nfft = nperseg;

    const auto nstep = nperseg - noverlap;
    const auto nseg = 1 + (asize - nperseg) / nstep;
    const auto npad = nperseg + (nseg - 1) * nstep - asize;
    auto x = zero_padding(a, std::size_t(npad));

    if constexpr (meta::is_complex_v<T>) {
        const auto freqs = fftfreq(std::size_t(nfft), T{1} / fs);
        auto res = std::vector<T>(std::size_t(freqs.size()));
        std::fill(res.begin(), res.end(), T{0});

        for (int i = 0; i < nseg; ++i) {
            auto seg = std::vector(a.cbegin() + i * nstep,
                                   a.cbegin() + i * nstep + nperseg);
            scicpp_require(seg.size() == w.size());

            // No operator += in numeric ??!!
            res = std::move(res) + norm(fft(std::move(seg - stats::mean(seg)) * w));
        }

        return std::make_tuple(freqs, res / (fs * S2 * T(nseg)));
    } else {
        const auto freqs = rfftfreq(std::size_t(nfft), T{1} / fs);
        auto res = std::vector<T>(std::size_t(freqs.size()));
        std::fill(res.begin(), res.end(), T{0});

        for (int i = 0; i < nseg; ++i) {
            auto seg = std::vector(a.cbegin() + i * nstep,
                                   a.cbegin() + i * nstep + nperseg);
            scicpp_require(seg.size() == w.size());

            // detrend = "constant" => Substract mean
            res = std::move(res) + norm(rfft(std::move(seg - stats::mean(seg)) * w));
        }

        res = std::move(res) / (fs * S2 * T(nseg));
        const auto last = res.back();
        res = 2.0 * std::move(res);
        // Last point is unpaired Nyquist freq point, don't double
        res.back() = last;

        return std::make_tuple(freqs, res);
    }
}

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_SPECTRAL