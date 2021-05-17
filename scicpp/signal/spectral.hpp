// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_SPECTRAL
#define SCICPP_SIGNAL_SPECTRAL

#include "scicpp/core/functional.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/signal/fft.hpp"
#include "scicpp/signal/windows.hpp"

#include <algorithm>
#include <tuple>

namespace scicpp::signal {

template <typename T = double>
class Spectrum {
  public:
    Spectrum() = default;

    auto fs(T fs) {
        m_fs = fs;
        return *this;
    }

    auto window(const std::vector<T> &window) {
        m_window = window;
        set_parameters();
        return *this;
    }

    auto window(std::vector<T> &&window) {
        m_window = std::move(window);
        set_parameters();
        return *this;
    }

    template <typename Array>
    auto welch(const Array &a) {
        using namespace scicpp::operators;
        using EltTp = typename Array::value_type;

        static_assert(meta::is_iterable_v<Array>);
        static_assert(std::is_same_v<EltTp, T> ||
                      std::is_same_v<EltTp, std::complex<T>>);

        if (a.empty()) {
            return std::make_tuple(empty<T>(), empty<T>());
        }

        const auto asize = signed_size_t(a.size());
        const auto nstep = m_nperseg - m_noverlap;
        const auto nseg = 1 + (asize - m_nperseg) / nstep;

        // Padding
        // const auto npad = m_nperseg + nseg * nstep - asize;
        // scicpp_require(npad >= 0);
        // auto x = zero_padding(a, std::size_t(npad));

        if constexpr (meta::is_complex_v<EltTp>) {
            const auto freqs = fftfreq(std::size_t(m_nperseg), T{1} / m_fs);

            auto res = welch_impl(
                std::size_t(freqs.size()), nseg, nstep, a, [](auto v) {
                    return fft(v);
                });

            return std::make_tuple(freqs,
                                   std::move(res) / (m_fs * m_s2 * T(nseg)));
        } else {
            const auto freqs = rfftfreq(std::size_t(m_nperseg), T{1} / m_fs);

            auto res = welch_impl(
                std::size_t(freqs.size()), nseg, nstep, a, [](auto v) {
                    return rfft(v);
                });

            res = 2.0 * std::move(res) / (m_fs * m_s2 * T(nseg));
            // Don't find why in scipy code, but need it to match scipy result
            res.front() *= 0.5;

            if (!(m_nperseg % 2)) {
                // Last point is unpaired Nyquist freq point, don't double
                res.back() *= 0.5;
            }

            return std::make_tuple(freqs, res);
        }
    }

  private:
    static constexpr signed_size_t dflt_nperseg = 256;

    T m_fs = T{1};
    std::vector<T> m_window = windows::hann<T>(dflt_nperseg);
    T m_s2 = get_s2();
    signed_size_t m_nperseg = get_nperseg();
    signed_size_t m_noverlap = get_noverlap();

    auto get_s2() {
        // S2 = Sum_i w_i^2
        return std::get<0>(
            reduce(m_window, [](auto r, auto v) { return r + v * v; }, T{0}));
    }

    auto get_nperseg() { return signed_size_t(m_window.size()); }

    auto get_noverlap() { return m_nperseg / 2; }

    void set_parameters() {
        m_s2 = get_s2();
        m_nperseg = get_nperseg();
        m_noverlap = get_noverlap();
    }

    template <typename Array, typename FFTFunc>
    auto welch_impl(std::size_t nfft,
                    signed_size_t nseg,
                    signed_size_t nstep,
                    const Array &a,
                    FFTFunc &&fftfunc) {
        auto res = zeros<T>(nfft);

        for (signed_size_t i = 0; i < nseg; ++i) {
            using namespace scicpp::operators;

            auto seg = subvector(a, i * nstep, i * nstep + m_nperseg);
            scicpp_require(seg.size() == m_window.size());

            // detrend = "constant" => Substract mean
            res = std::move(res) +
                  norm(fftfunc((std::move(seg) - stats::mean(seg)) * m_window));
        }

        return res;
    }

    template <typename Array>
    auto subvector(const Array &a, signed_size_t first, signed_size_t last) {
        scicpp_require(last >= first);
        // could use C++20 span
        return std::vector(a.cbegin() + first, a.cbegin() + last);
    }
}; // class Spectrum

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_SPECTRAL