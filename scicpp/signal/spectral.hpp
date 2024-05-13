// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_SPECTRAL
#define SCICPP_SIGNAL_SPECTRAL

#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/core/units/units.hpp"
#include "scicpp/core/utils.hpp"
#include "scicpp/signal/fft.hpp"
#include "scicpp/signal/windows.hpp"

#include <algorithm>
#include <complex>
#include <cstdlib>
#include <mutex>
#include <thread>
#include <tuple>
#include <type_traits>
#include <vector>

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
        m_use_dflt_overlap = false;
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

    template <SpectrumScaling scaling = DENSITY,
              bool return_freqs = true,
              typename Array>
    auto periodogram(const Array &x) {
        scicpp_require(x.size() == m_window.size());
        noverlap(0);
        return welch<scaling, return_freqs>(x);
    }

    template <SpectrumScaling scaling = DENSITY,
              bool return_freqs = true,
              typename Array>
    auto welch(const Array &x) {
        using namespace scicpp::operators;

        using ArrTp = typename Array::value_type;
        using EltTp = std::conditional_t<units::is_quantity_v<ArrTp>,
                                         units::representation_t<ArrTp>,
                                         ArrTp>;

        static_assert(meta::is_iterable_v<Array>);
        static_assert(std::is_same_v<EltTp, T> ||
                      std::is_same_v<EltTp, std::complex<T>>);

        if (unlikely(x.empty())) {
            if constexpr (return_freqs) {
                return std::tuple{empty<T>(), empty<T>()};
            } else {
                return empty<T>();
            }
        }

        std::vector<T> psd;

        if constexpr (meta::is_complex_v<EltTp>) {
            psd = normalize<scaling, TWOSIDED>(
                welch_impl(std::size_t(m_nperseg), x, fft_func));
        } else {
            psd = normalize<scaling, ONESIDED>(
                welch_impl(std::size_t(m_nperseg) / 2 + 1, x, rfft_func));
        }

        if constexpr (return_freqs) {
            return std::tuple{get_freqs<EltTp>(), psd};
        } else {
            return psd;
        }
    }

    template <SpectrumScaling scaling = DENSITY,
              bool return_freqs = true,
              typename Array1,
              typename Array2>
    auto csd(const Array1 &x, const Array2 &y) {
        using namespace scicpp::operators;

        using ArrTp1 = typename Array1::value_type;
        using ArrTp2 = typename Array2::value_type;
        using EltTp1 = std::conditional_t<units::is_quantity_v<ArrTp1>,
                                          units::representation_t<ArrTp1>,
                                          ArrTp1>;
        using EltTp2 = std::conditional_t<units::is_quantity_v<ArrTp2>,
                                          units::representation_t<ArrTp2>,
                                          ArrTp2>;

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

        if (unlikely(x.empty() || y.empty())) {
            if constexpr (return_freqs) {
                return std::tuple{empty<T>(), empty<std::complex<T>>()};
            } else {
                return empty<std::complex<T>>();
            }
        }

        if (x.size() == y.size()) {
            std::vector<std::complex<T>> csd;

            if constexpr (meta::is_complex_v<EltTp>) {
                csd = normalize<scaling, TWOSIDED>(
                    welch2_impl(std::size_t(m_nperseg), x, y, fft_func));
            } else {
                csd = normalize<scaling, ONESIDED>(welch2_impl(
                    std::size_t(m_nperseg) / 2 + 1, x, y, rfft_func));
            }

            if constexpr (return_freqs) {
                return std::tuple{get_freqs<EltTp>(), csd};
            } else {
                return csd;
            }
        } else {
            if (x.size() > y.size()) {
                return csd<scaling, return_freqs>(x, zero_padding(y, x.size()));
            } else { // x.size() < y.size()
                return csd<scaling, return_freqs>(zero_padding(x, y.size()), y);
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

        return std::tuple{
            freqs, norm(std::move(Pxy)) / std::move(Pxx) / std::move(Pyy)};
    }

    template <typename Array1, typename Array2>
    auto tfestimate(const Array1 &x, const Array2 &y) {
        using namespace scicpp::operators;
        scicpp_require(x.size() == y.size());

        auto [freqs, Pyx] = csd<NONE>(y, x);
        auto Pxx = welch<NONE, false>(x);
        return std::tuple{freqs, std::move(Pyx) / std::move(Pxx)};
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
    bool m_use_dflt_overlap = true;
    signed_size_t m_noverlap = m_nperseg / 2;
    std::size_t m_nthreads = 0;

    auto get_nperseg() { return signed_size_t(m_window.size()); }

    void set_parameters() {
        m_s1 = windows::s1(m_window);
        m_s2 = windows::s2(m_window);
        m_nperseg = get_nperseg();

        if (m_use_dflt_overlap) {
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

    template <class Func>
    auto compute_segments_multithread(signed_size_t nseg, Func func) {
        std::vector<std::thread> threads_pool(m_nthreads);
        signed_size_t i = 0;

        while (i < nseg) {
            for (auto &thrd : threads_pool) {
                thrd = std::thread(func, i);
                ++i;

                if (i >= nseg) {
                    break;
                }
            }

            for (auto &thrd : threads_pool) {
                if (thrd.joinable()) {
                    thrd.join();
                }
            }
        }
    }

    template <typename Tp, class SegPsdFunc>
    auto compute_spectrum(std::size_t nfft,
                          signed_size_t nseg,
                          SegPsdFunc get_segment_psd) {
        using namespace scicpp::operators;

        auto res = zeros<Tp>(nfft);

        if (m_nthreads <= 1 || nseg == 1) {
            for (signed_size_t i = 0; i < nseg; ++i) {
                res = std::move(res) + get_segment_psd(i);
            }
        } else {
            std::mutex mtx;
            compute_segments_multithread(nseg, [&](auto i) {
                auto seg_spectrum = get_segment_psd(i);

                {
                    std::lock_guard guard(mtx);
                    res = std::move(res) + std::move(seg_spectrum);
                }
            });
        }

        return std::move(res) / T(nseg);
    }

    template <typename Array, typename FFTFunc>
    auto welch_impl(std::size_t nfft, const Array &a, FFTFunc &&fftfunc) {
        const auto asize = signed_size_t(a.size());
        const auto nstep = m_nperseg - m_noverlap;
        const auto nseg = 1 + (asize - m_nperseg) / nstep;

        return compute_spectrum<T>(nfft, nseg, [&](auto i) {
            using namespace scicpp::operators;

            auto seg = utils::subvector(a, m_nperseg, i * nstep);
            scicpp_require(seg.size() == m_window.size());
            return norm(fftfunc(detrend(value(std::move(seg))) * m_window));
        });
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

        return compute_spectrum<std::complex<T>>(nfft, nseg, [&](auto i) {
            using namespace scicpp::operators;

            auto seg_x = utils::subvector(x, m_nperseg, i * nstep);
            scicpp_require(seg_x.size() == m_window.size());
            auto seg_y = utils::subvector(y, m_nperseg, i * nstep);
            scicpp_require(seg_y.size() == m_window.size());
            return conj(fftfunc(detrend(value(std::move(seg_x))) * m_window)) *
                   fftfunc(detrend(value(std::move(seg_y))) * m_window);
        });
    }

    // TODO Normalize should return proper quantity if input data is a physical quantity
    // Need to store fs unit
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

    // Convert vector of quantity to values
    template <typename Array, meta::enable_if_iterable<Array> = 0>
    constexpr auto value(Array &&x) {
        using Tp = typename Array::value_type;

        if constexpr (units::is_quantity_v<Tp>) {
            return map([&](auto a) { return a.value(); },
                       std::forward<Array>(x));
        } else {
            return std::forward<Array>(x);
        }
    }
}; // class Spectrum

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_SPECTRAL