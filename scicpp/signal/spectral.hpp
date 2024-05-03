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
#include "scicpp/core/units/units.hpp"
#include "scicpp/core/utils.hpp"
#include "scicpp/signal/fft.hpp"
#include "scicpp/signal/windows.hpp"

#include <algorithm>
#include <mutex>
#include <thread>
#include <tuple>

namespace scicpp::signal {

enum SpectrumScaling : int { NONE, DENSITY, SPECTRUM };

enum SpectrumSides : int { ONESIDED, TWOSIDED };

template <typename T = double>
class Spectrum {
  public:
    Spectrum() = default;

    // -------------------------------------------------------------------------
    // Spectrum analysis configuration
    // -------------------------------------------------------------------------

    template <typename FsTp>
    auto fs(FsTp fs) {
        if constexpr (units::is_quantity_v<FsTp>) {
            static_assert(units::is_frequency<FsTp>, "fs must be a frequency");
            m_fs = fs.eval();
        } else {
            m_fs = fs;
        }

        return *this;
    }

    auto noverlap(signed_size_t noverlap) {
        scicpp_require(noverlap <= m_nperseg);
        m_use_dflt_overlpap = false;
        m_noverlap = noverlap;
        return *this;
    }

    auto nthreads(std::size_t nthreads) {
        m_nthreads = nthreads;
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

    // -------------------------------------------------------------------------
    // Spectrum computations
    // -------------------------------------------------------------------------

    template <SpectrumScaling scaling = DENSITY, typename Array>
    auto periodogram(const Array &x) {
        scicpp_require(x.size() == m_window.size());
        noverlap(0);
        return welch(x);
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

    template <SpectrumScaling scaling = DENSITY,
              typename Array1,
              typename Array2>
    auto csd(const Array1 &x, const Array2 &y) {
        using namespace scicpp::operators;
        using EltTp1 = typename Array1::value_type;
        using EltTp2 = typename Array2::value_type;

        static_assert(meta::is_iterable_v<Array1>);
        static_assert(meta::is_iterable_v<Array2>);
        static_assert(std::is_same_v<EltTp1, T> ||
                      std::is_same_v<EltTp1, std::complex<T>>);
        static_assert(std::is_same_v<EltTp2, T> ||
                      std::is_same_v<EltTp2, std::complex<T>>);

        using EltTp = std::conditional_t<meta::is_complex_v<EltTp1> ||
                                             meta::is_complex_v<EltTp2>,
                                         std::complex<T>,
                                         T>;

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
            if (x.size() > y.size()) {
                return csd<scaling>(x, zero_padding(y, x.size()));
            } else { // x.size() < y.size()
                return csd<scaling>(zero_padding(x, y.size()), y);
            }
        }
    }

    template <typename Array1, typename Array2>
    auto coherence(const Array1 &x, const Array2 &y) {
        using namespace scicpp::operators;
        scicpp_require(x.size() == y.size());

        auto [freqs, Pxy] = csd<NONE>(x, y);
        auto Pxx = std::get<1>(welch<NONE>(x));
        auto Pyy = std::get<1>(welch<NONE>(y));

        scicpp_require(Pxy.size() == Pxx.size());
        scicpp_require(Pxy.size() == Pyy.size());

        return std::make_tuple(
            freqs, norm(std::move(Pxy)) / std::move(Pxx) / std::move(Pyy));
    }

    template <typename Array1, typename Array2>
    auto tfestimate(const Array1 &x, const Array2 &y) {
        using namespace scicpp::operators;
        scicpp_require(x.size() == y.size());

        auto [freqs, Pyx] = csd<NONE>(y, x);
        auto Pxx = std::get<1>(welch<NONE>(x));
        return std::make_tuple(freqs, std::move(Pyx) / std::move(Pxx));
    }

  private:
    static constexpr signed_size_t dflt_nperseg = 256;
    static constexpr auto rfft_func = [](auto v) { return rfft(v); };
    static constexpr auto fft_func = [](auto v) { return fft(v); };

    // detrend = "constant" => Substract mean
    static constexpr auto detrend = [](auto &&x) {
        using namespace scicpp::operators;
        return std::move(x) - stats::mean(x);
    };

    T m_fs = T{1};
    std::vector<T> m_window = windows::hann<T>(dflt_nperseg);
    T m_s1 = windows::s1(m_window);
    T m_s2 = windows::s2(m_window);
    signed_size_t m_nperseg = get_nperseg();
    bool m_use_dflt_overlpap = true;
    signed_size_t m_noverlap = m_nperseg / 2;
    std::size_t m_nthreads = 0;

    auto get_nperseg() { return signed_size_t(m_window.size()); }

    void set_parameters() {
        m_s1 = windows::s1(m_window);
        m_s2 = windows::s2(m_window);
        m_nperseg = get_nperseg();

        if (m_use_dflt_overlpap) {
            m_noverlap = m_nperseg / 2;
        }

        scicpp_require(m_noverlap <= m_nperseg);
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

        if (m_nthreads <= 1 || nseg == 1) {
            for (signed_size_t i = 0; i < nseg; ++i) {
                // In principle we could avoid to reallocate a vector each time
                auto seg =
                    utils::subvector(a, std::size_t(m_nperseg), i * nstep);
                scicpp_require(seg.size() == m_window.size());
                res = std::move(res) +
                      norm(fftfunc(detrend(std::move(seg)) * m_window));
            }
        } else {
            std::mutex mtx;
            std::vector<std::thread> threads_pool(m_nthreads);
            signed_size_t i = 0;

            while (i < nseg) {
                for (auto &thread : threads_pool) {
                    thread = std::thread(
                        [&](auto i) {
                            auto seg = utils::subvector(
                                a, std::size_t(m_nperseg), i * nstep);
                            scicpp_require(seg.size() == m_window.size());
                            auto seg_spectrum = norm(
                                fftfunc(detrend(std::move(seg)) * m_window));

                            {
                                std::lock_guard guard(mtx);
                                res = std::move(res) + std::move(seg_spectrum);
                            }
                        },
                        i);

                    ++i;

                    if (i >= nseg) {
                        break;
                    }
                }

                for (auto &thread : threads_pool) {
                    if (thread.joinable()) {
                        thread.join();
                    }
                }
            }
        }

        return std::move(res) / T(nseg);
    }

    template <typename Array1, typename Array2, typename FFTFunc>
    auto welch2_impl(std::size_t nfft,
                     const Array1 &x,
                     const Array2 &y,
                     FFTFunc &&fftfunc) {
        using namespace scicpp::operators;
        scicpp_require(x.size() == y.size());

        const auto asize = signed_size_t(x.size());
        const auto nstep = m_nperseg - m_noverlap;
        const auto nseg = 1 + (asize - m_nperseg) / nstep;

        auto res = zeros<std::complex<T>>(nfft);

        for (signed_size_t i = 0; i < nseg; ++i) {
            // In principle we could avoid to reallocate a vector each time
            auto seg_x = utils::subvector(x, std::size_t(m_nperseg), i * nstep);
            scicpp_require(seg_x.size() == m_window.size());
            auto seg_y = utils::subvector(y, std::size_t(m_nperseg), i * nstep);
            scicpp_require(seg_y.size() == m_window.size());

            res = std::move(res) +
                  conj(fftfunc(detrend(std::move(seg_x)) * m_window)) *
                      fftfunc(detrend(std::move(seg_y)) * m_window);
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
}; // class Spectrum

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_SPECTRAL