// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_FFT
#define SCICPP_SIGNAL_FFT

#include "scicpp/core/macros.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/signal/windows.hpp"

#include <algorithm>
#include <array>
#include <complex>
#include <unsupported/Eigen/FFT>
#include <vector>

namespace scicpp {
namespace signal {

//---------------------------------------------------------------------------------
// FFT helper functions
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename T = typename Array::value_type>
constexpr void fftfreq_filler(Array &res, T d) {
    const auto N = res.size();
    // FIXME Handle d = 0 and N = 0
    const T scaling = T(1) / (d * T(N));

    if (N % 2 == 0) {
        const std::size_t max = (N / 2) - 1;

        for (std::size_t i = 0; i <= max; ++i) {
            res[i] = T(i) * scaling;
            res[max + 1 + i] = -T(max + 1 - i) * scaling;
        }
    } else {
        const std::size_t max = (N - 1) / 2;

        for (std::size_t i = 0; i < max; ++i) {
            res[i] = T(i) * scaling;
            res[max + 1 + i] = -T(max - i) * scaling;
        }

        res[max] = T(max) * scaling;
    }
}

} // namespace detail

template <std::size_t N, typename T = double>
constexpr auto fftfreq(T d = T(1)) {
    std::array<T, N> res{};
    detail::fftfreq_filler(res, d);
    return res;
}

template <typename T>
auto fftfreq(std::size_t n, T d = T(1)) {
    std::vector<T> res(n);
    detail::fftfreq_filler(res, d);
    return res;
}

template <class Array>
void fftshift_inplace(Array &a) {
    std::rotate(a.begin(), a.begin() + int(a.size() + 1) / 2, a.end());
}

namespace detail {

template <class Array>
void fftshift_filler(const Array &src, Array &dest) {
    const auto offset = (int(src.size() - 1) / 2) + 1;
    std::copy(src.cbegin() + offset, src.cend(), dest.begin());
    std::copy(src.cbegin(),
              src.cbegin() + offset,
              dest.begin() + int(src.size()) / 2);
}

} // namespace detail

template <typename T, std::size_t N>
constexpr auto fftshift(const std::array<T, N> &a) {
    std::array<T, N> res{};
    detail::fftshift_filler(a, res);
    return res;
}

template <typename T>
auto fftshift(const std::vector<T> &v) {
    std::vector<T> res(v.size());
    detail::fftshift_filler(v, res);
    return res;
}

namespace detail {

// https://rosettacode.org/wiki/Hamming_numbers#C.2B.2B11_For_Each_Generator
// Hamming like sequences Generator
//
// Nigel Galloway. August 13th., 2012
template <typename Integral, std::size_t n_primes>
class Hamming {
  private:
    std::array<Integral, n_primes> m_H, m_hp, m_hv;
    std::vector<Integral> m_x;

  public:
    bool operator!=(const Hamming & /*unused*/) const { return true; }
    Hamming begin() const { return *this; }
    Hamming end() const { return *this; }
    Integral operator*() const { return m_x.back(); }

    explicit Hamming(const std::array<Integral, n_primes> &pfs)
        : m_H(pfs), m_hp{}, m_hv({pfs}), m_x({1}) {}

    const Hamming &operator++() {
        for (std::size_t i = 0; i < n_primes; ++i) {
            for (; m_hv[i] <= m_x.back(); m_hv[i] = m_x[++m_hp[i]] * m_H[i]) {
            }
        }

        m_x.push_back(m_hv[0]);

        for (const auto &v : m_hv) {
            if (v < m_x.back()) {
                m_x.back() = v;
            }
        }

        return *this;
    }
};

template <typename Integral>
Integral next_ugly_number(Integral num) {
    for (const auto h : Hamming<Integral, 3>({2, 3, 5})) {
        if (h >= num) {
            return h;
        }
    }

    SCICPP_UNREACHABLE;
    return 0;
}

} // namespace detail

template <typename Integral>
Integral next_fast_len(Integral n) {
    return detail::next_ugly_number(n);
}

template <typename T>
auto zero_padding(const std::vector<T> &v, std::size_t new_size) {
    std::vector<T> res(new_size, T(0));
    std::copy(v.cbegin(),
              v.cbegin() + int(std::min(new_size, v.size())),
              res.begin());
    return res;
}

//---------------------------------------------------------------------------------
// FFTs
//---------------------------------------------------------------------------------

template <typename T>
auto fft(const std::vector<std::complex<T>> &x) {
    Eigen::FFT<T> fft_engine;
    std::vector<std::complex<T>> y;
    fft_engine.fwd(y, x);
    return y;
}

template <typename T>
auto fft(const std::vector<T> &x) {
    Eigen::FFT<T> fft_engine;
    std::vector<std::complex<T>> y;
    fft_engine.fwd(y, x);
    return y;
}

template <typename T>
auto rfft(const std::vector<T> &x) {
    Eigen::FFT<T> fft_engine;
    fft_engine.SetFlag(Eigen::FFT<T>::HalfSpectrum);
    std::vector<std::complex<T>> y;
    fft_engine.fwd(y, x);
    return y;
}

template <typename T>
auto ifft(const std::vector<std::complex<T>> &y, int n = -1) {
    Eigen::FFT<T> fft_engine;
    std::vector<std::complex<T>> x;

    if (int(y.size()) == n || n < 0) {
        fft_engine.inv(x, y);
    } else {
        fft_engine.inv(x, zero_padding(y, std::size_t(n)));
    }

    return x;
}

namespace detail {

template <typename T>
auto to_complex(const std::vector<T> &y, std::size_t n) {
    std::vector<std::complex<T>> x(n, std::complex<T>(T(0), T(0)));

    std::transform(y.cbegin(),
                   y.cbegin() + int(std::min(n, y.size())),
                   x.begin(),
                   [](auto v) { return std::complex(v, T(0)); });

    return x;
}

} // namespace detail

template <typename T>
auto ifft(const std::vector<T> &y, int n = -1) {
    Eigen::FFT<T> fft_engine;
    std::vector<std::complex<T>> x;
    const auto size = n < 0 ? y.size() : std::size_t(n);
    fft_engine.inv(x, detail::to_complex(y, size));
    return x;
}

template <typename T>
auto irfft(const std::vector<std::complex<T>> &y, int n = -1) {
    Eigen::FFT<T> fft_engine;
    std::vector<T> x;

    if (int(y.size()) == n) {
        fft_engine.inv(x, y);
    } else {
        const auto size = n < 0 ? 2 * (y.size() - 1) : std::size_t(n);
        fft_engine.inv(x, zero_padding(y, size));
    }

    return x;
}

//---------------------------------------------------------------------------------
// Power spectrum
//---------------------------------------------------------------------------------

/// Compute the power spectrum density of a real signal
///
/// \param x Real signal vector
/// \param fs Sampling frequency (float)
/// \param w Window vector must have the same size than x
///
/// \return A vector that contains the power spectral density.
///
/// \rst
///  The output as unit of :math:`\rm{V}^{2}/\rm{Hz}` if x is in :math:`\rm{V}`
///  and fs is in :math:`\rm{Hz}`.
/// \endrst
template <typename T>
auto power_spectrum_density(const std::vector<T> &x,
                            T fs,
                            const std::vector<T> &w) {
    using namespace scicpp::operators;

    const auto tmp = x * w;
    T S2 = 0;

    for (const auto &v : w) {
        S2 += v * v;
    }

    return (2. / (fs * S2)) * norm(rfft(tmp));
}

/// Compute the power spectrum density of a real signal
///
/// \param x Real signal vector
/// \param fs Sampling frequency (float)
/// \param win Name of the window
///
/// \return A vector that contains the power spectral density.
///
/// \rst
///  The output as unit of :math:`\rm{V}^{2}/\rm{Hz}` if x is in :math:`\rm{V}`
///  and fs is in :math:`\rm{Hz}`.
/// \endrst
template <typename T>
auto power_spectrum_density(const std::vector<T> &x,
                            T fs,
                            windows::Window win) {
    return power_spectrum_density(x, fs, windows::get_window<T>(win, x.size()));
}

} // namespace signal
} // namespace scicpp

#endif // SCICPP_SIGNAL_FFT
