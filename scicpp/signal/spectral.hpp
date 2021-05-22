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

enum SpectrumScaling : int { NONE, DENSITY, SPECTRUM };

enum SpectrumSides : int { ONESIDED, TWOSIDED };

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

    auto window(windows::Window win, std::size_t N) {
        m_window = windows::get_window<T>(win, N);
        set_parameters();
        return *this;
    }

    template <SpectrumScaling scaling = DENSITY, typename Array>
    auto welch(const Array &x) {
        using namespace scicpp::operators;
        using EltTp = typename Array::value_type;

        static_assert(meta::is_iterable_v<Array>);
        static_assert(std::is_same_v<EltTp, T> ||
                      std::is_same_v<EltTp, std::complex<T>>);

        if (x.empty()) {
            return std::make_tuple(empty<T>(), empty<T>());
        }

        const auto freqs = get_freqs<EltTp>();

        if constexpr (meta::is_complex_v<EltTp>) {
            return std::make_tuple(freqs,
                                   normalize<scaling, TWOSIDED>(
                                       welch_impl(freqs.size(), x, fft_func)));
        } else {
            return std::make_tuple(freqs,
                                   normalize<scaling, ONESIDED>(
                                       welch_impl(freqs.size(), x, rfft_func)));
        }
    }

    template <SpectrumScaling scaling = DENSITY, typename Array>
    auto csd(const Array &x, const Array &y) {
        using namespace scicpp::operators;
        using EltTp = typename Array::value_type;

        static_assert(meta::is_iterable_v<Array>);
        static_assert(std::is_same_v<EltTp, T> ||
                      std::is_same_v<EltTp, std::complex<T>>);

        if (x.empty() || y.empty()) {
            return std::make_tuple(empty<T>(), empty<std::complex<T>>());
        }

        const auto freqs = get_freqs<EltTp>();

        if (x.size() == y.size()) {
            if constexpr (meta::is_complex_v<EltTp>) {
                return std::make_tuple(freqs,
                                       normalize<scaling, TWOSIDED>(welch2_impl(
                                           freqs.size(), x, y, fft_func)));
            } else {
                return std::make_tuple(freqs,
                                       normalize<scaling, ONESIDED>(welch2_impl(
                                           freqs.size(), x, y, rfft_func)));
            }

        } else {
            // TODO padding
            return std::make_tuple(empty<T>(), empty<std::complex<T>>());
        }
    }

  private:
    static constexpr signed_size_t dflt_nperseg = 256;
    static constexpr auto rfft_func = [](auto v) { return rfft(v); };
    static constexpr auto fft_func = [](auto v) { return fft(v); };

    T m_fs = T{1};
    std::vector<T> m_window = windows::hann<T>(dflt_nperseg);
    T m_s1 = get_s1();
    T m_s2 = get_s2();
    signed_size_t m_nperseg = get_nperseg();
    signed_size_t m_noverlap = get_noverlap();

    auto get_s1() {
        // S1 = (Sum_i w_i)^2
        const auto s = sum(m_window);
        return s * s;
    }

    auto get_s2() {
        // S2 = Sum_i w_i^2
        return std::get<0>(
            reduce(m_window, [](auto r, auto v) { return r + v * v; }, T{0}));
    }

    auto get_nperseg() { return signed_size_t(m_window.size()); }

    auto get_noverlap() { return m_nperseg / 2; }

    void set_parameters() {
        m_s1 = get_s1();
        m_s2 = get_s2();
        m_nperseg = get_nperseg();
        m_noverlap = get_noverlap();
    }

    template <typename EltTp>
    auto get_freqs() {
        if constexpr (meta::is_complex_v<EltTp>) {
            return fftfreq(std::size_t(m_nperseg), T{1} / m_fs);
        } else {
            return rfftfreq(std::size_t(m_nperseg), T{1} / m_fs);
        }
    }

    template <typename Array, typename FFTFunc>
    auto welch_impl(std::size_t nfft, const Array &a, FFTFunc &&fftfunc) {
        using namespace scicpp::operators;

        const auto asize = signed_size_t(a.size());
        const auto nstep = m_nperseg - m_noverlap;
        const auto nseg = 1 + (asize - m_nperseg) / nstep;

        auto res = zeros<T>(nfft);

        for (signed_size_t i = 0; i < nseg; ++i) {
            auto seg = subvector(a, i * nstep, i * nstep + m_nperseg);
            scicpp_require(seg.size() == m_window.size());

            // detrend = "constant" => Substract mean
            res = std::move(res) +
                  norm(fftfunc((std::move(seg) - stats::mean(seg)) * m_window));
        }

        return std::move(res) / T(nseg);
    }

    template <typename Array, typename FFTFunc>
    auto welch2_impl(std::size_t nfft,
                     const Array &x,
                     const Array &y,
                     FFTFunc &&fftfunc) {
        using namespace scicpp::operators;
        scicpp_require(x.size() == y.size());

        const auto asize = signed_size_t(x.size());
        const auto nstep = m_nperseg - m_noverlap;
        const auto nseg = 1 + (asize - m_nperseg) / nstep;

        auto res = zeros<std::complex<T>>(nfft);

        for (signed_size_t i = 0; i < nseg; ++i) {
            auto seg_x = subvector(x, i * nstep, i * nstep + m_nperseg);
            scicpp_require(seg_x.size() == m_window.size());
            auto seg_y = subvector(y, i * nstep, i * nstep + m_nperseg);
            scicpp_require(seg_y.size() == m_window.size());

            // detrend = "constant" => Substract mean
            res =
                std::move(res) +
                conj(fftfunc((std::move(seg_x) - stats::mean(seg_x)) *
                             m_window)) *
                    fftfunc((std::move(seg_y) - stats::mean(seg_y)) * m_window);
        }

        return std::move(res) / T(nseg);
    }

    template <SpectrumScaling scaling, SpectrumSides sides, typename SpecTp>
    auto normalize(std::vector<SpecTp> &&v) {
        using namespace scicpp::operators;

        if constexpr (sides == ONESIDED) {
            v = 2.0 * std::move(v);
            // Don't find why in scipy code, but need it to match scipy result
            v.front() *= 0.5;

            if (!(m_nperseg % 2)) {
                // Last point is unpaired Nyquist freq point, don't double
                v.back() *= 0.5;
            }
        }

        if constexpr (scaling == DENSITY) {
            return std::move(v) / (m_fs * m_s2);
        } else if constexpr (scaling == SPECTRUM) {
            return std::move(v) / m_s1;
        } else { // scaling == NONE
            return std::move(v);
        }
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