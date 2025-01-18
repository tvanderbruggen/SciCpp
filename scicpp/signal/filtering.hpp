// SPDX-License-Identifier: MIT

#ifndef SCICPP_SIGNAL_SIGNALTOOLS
#define SCICPP_SIGNAL_SIGNALTOOLS

#include "scicpp/core/equal.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/manips.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/linalg/matrices.hpp"
#include "scicpp/linalg/solve.hpp"
#include "scicpp/signal/arraytools.hpp"
#include "scicpp/signal/convolve.hpp"

#include <Eigen/Dense>
#include <algorithm>
#include <array>
#include <complex>
#include <tuple>
#include <utility>
#include <vector>

namespace scicpp::signal {

// ----------------------------------------------------------------------------
// lfilter_zi
// ----------------------------------------------------------------------------

namespace detail {

template <typename Array1, typename Array2>
auto zfill(Array1 &b, Array2 &a) {
    const auto n = std::max(a.size(), b.size());

    if (a.size() < n) {
        a.resize(n);
    } else if (b.size() < n) {
        b.resize(n);
    }

    return n;
}

} // namespace detail

// Specialization for std::array.
// Return a std::array but require a[0] != 0 for the size to be known at compile time.
template <typename T, std::size_t N1, std::size_t N2>
auto lfilter_zi(const std::array<T, N1> &b, const std::array<T, N2> &a) {
    using namespace operators;

    scicpp_require(!almost_equal(std::get<0>(a), T(0)) &&
                   "lfilter_zi: Require a[0] != 0 for std::array");

    constexpr auto N = std::max(N1, N2);
    auto a_ = zeros<N, T>();
    auto b_ = zeros<N, T>();
    std::copy(a.begin(), a.end(), a_.begin());
    std::copy(b.begin(), b.end(), b_.begin());

    if (!almost_equal(std::get<0>(a_), T(1))) {
        b_ = std::move(b_) / std::get<0>(a_);
        a_ = std::move(a_) / std::get<0>(a_);
    }

    auto A = linalg::companion(a_);
    auto tmp = b_ - std::move(a_) * std::get<0>(b_);
    auto B = zeros<N - 1, T>();
    std::copy(tmp.begin() + 1, tmp.end(), B.begin());
    // Solve zi = A zi + B => (Id - A) zi = B
    return linalg::lstsq(linalg::eye<T, N - 1>() - A.transpose(), B);
}

template <typename Array1, typename Array2>
auto lfilter_zi(const Array1 &b, const Array2 &a) {
    using T = typename Array1::value_type;
    static_assert(std::is_same_v<T, typename Array2::value_type>);
    using namespace operators;

    std::size_t k = 0;
    while (k < a.size() && almost_equal(a[k], T(0))) {
        ++k;
    }

    scicpp_require(
        k <= (a.size() - 1) &&
        "lfilter_zi: There must be at least one nonzero `a` coefficient.");

    std::vector<T> a_(a.cbegin() + signed_size_t(k), a.cend());
    std::vector<T> b_(b.cbegin(), b.cend());

    if (!almost_equal(a_[0], T(1))) {
        b_ = std::move(b_) / a_[0];
        a_ = std::move(a_) / a_[0];
    }

    const auto n = detail::zfill(a_, b_);

    auto A = linalg::companion(a_);
    auto B = b_ - std::move(a_) * b_[0];
    B.erase(B.begin());
    // Solve zi = A zi + B => (Id - A) zi = B
    return linalg::lstsq(linalg::eye<T>(n - 1) - A.transpose(), B);
}

// ----------------------------------------------------------------------------
// lfilter
// ----------------------------------------------------------------------------

namespace detail {

template <typename A, typename B, typename U, typename V, typename W>
constexpr void lfilter_impl(
    const A &a, const B &b, const U &x, V &y, W &Z, std::size_t nfilt) {
    if (a.size() < nfilt) {
        for (std::size_t k = 0; k < x.size(); ++k) {
            y[k] = Z[0] + b[0] / a[0] * x[k];

            for (std::size_t n = 0; n < a.size() - 1; ++n) {
                Z[n] =
                    Z[n + 1] + x[k] * b[n + 1] / a[0] - y[k] * a[n + 1] / a[0];
            }

            for (std::size_t n = a.size() - 1; n < nfilt - 2; ++n) {
                Z[n] = Z[n + 1] + x[k] * b[n + 1] / a[0];
            }

            Z.back() = x[k] * b[nfilt - 1] / a[0];
        }
    } else { // b.size() <= nfilt
        for (std::size_t k = 0; k < x.size(); ++k) {
            y[k] = Z[0] + b[0] / a[0] * x[k];

            for (std::size_t n = 0; n < b.size() - 1; ++n) {
                Z[n] =
                    Z[n + 1] + x[k] * b[n + 1] / a[0] - y[k] * a[n + 1] / a[0];
            }

            for (std::size_t n = b.size() - 1; n < nfilt - 2; ++n) {
                Z[n] = Z[n + 1] - y[k] * a[n + 1] / a[0];
            }

            Z.back() = -y[k] * a[nfilt - 1] / a[0];
        }
    }
}

} // namespace detail

template <typename T, std::size_t Nb, std::size_t Na, std::size_t Nx>
constexpr auto lfilter(const std::array<T, Nb> &b,
                       const std::array<T, Na> &a,
                       const std::array<T, Nx> &x) {
    static_assert(Nx > 0);
    scicpp_require(fabs(std::get<0>(a)) > T(0));

    if constexpr (Na == 1) {
        using namespace operators;
        auto out = convolve(b, x);
        std::array<T, out.size() - Nb + 1> res{};
        std::copy(out.begin(), out.begin() + res.size(), res.begin());
        return res / std::get<0>(a);
    } else {
        constexpr auto n = std::max(Na, Nb);
        auto y = zeros<Nx, T>();
        auto Z = zeros<n - 1, T>();
        detail::lfilter_impl(a, b, x, y, Z, n);
        return y;
    }
}

template <typename T,
          std::size_t Nb,
          std::size_t Na,
          std::size_t Nx,
          std::size_t Nzi>
constexpr auto lfilter(const std::array<T, Nb> &b,
                       const std::array<T, Na> &a,
                       const std::array<T, Nx> &x,
                       const std::array<T, Nzi> &zi) {
    static_assert(Nx > 0);
    scicpp_require(fabs(std::get<0>(a)) > T(0));

    if constexpr (Na == 1) {
        using namespace operators;
        auto out = convolve(b, x) / std::get<0>(a);

        std::array<T, Nb - 1> zf{};
        std::copy(out.end() - signed_size_t(Nb) + 1, out.end(), zf.begin());

        std::array<T, out.size() - Nb + 1> res{};
        std::copy(out.begin(), out.begin() + res.size(), res.begin());

        for (std::size_t k = 0; k < zi.size(); ++k) {
            res[k] += zi[k];
        }

        return std::tuple{res, zf};
    } else {
        constexpr auto n = std::max(Na, Nb);
        auto y = zeros<Nx, T>();
        auto Z = zi;
        detail::lfilter_impl(a, b, x, y, Z, n);
        return std::tuple{y, Z};
    }
}

template <typename Array1, typename Array2, typename Array3>
auto lfilter(const Array1 &b, const Array2 &a, const Array3 &x) {
    using T = typename Array1::value_type;
    static_assert(std::is_same_v<T, typename Array2::value_type>);
    static_assert(std::is_same_v<T, typename Array3::value_type>);

    scicpp_require(!almost_equal(a[0], T(0)));
    scicpp_require(!x.empty());

    if (a.size() == 1) {
        using namespace operators;
        auto out = convolve(b, x);
        out.resize(out.size() - b.size() + 1);
        return out / a[0];
    } else {
        const auto nfilt = std::max(a.size(), b.size());
        auto y = zeros<T>(x.size());
        auto Z = zeros<T>(nfilt - 1);
        detail::lfilter_impl(a, b, x, y, Z, nfilt);
        return y;
    }
}

template <typename Array1, typename Array2, typename Array3, typename Array4>
auto lfilter(const Array1 &b,
             const Array2 &a,
             const Array3 &x,
             const Array4 &zi) {
    using T = typename Array1::value_type;
    static_assert(std::is_same_v<T, typename Array2::value_type>);
    static_assert(std::is_same_v<T, typename Array3::value_type>);
    static_assert(std::is_same_v<T, typename Array4::value_type>);

    scicpp_require(!almost_equal(a[0], T(0)));
    scicpp_require(!x.empty());
    scicpp_require(zi.size() == std::max(a.size(), b.size()) - 1);

    if (a.size() == 1) {
        using namespace operators;
        auto out = convolve(b, x) / a[0];
        std::vector<T> zf(out.end() - signed_size_t(b.size()) + 1, out.end());
        out.resize(out.size() - b.size() + 1);

        for (std::size_t k = 0; k < zi.size(); ++k) {
            out[k] += zi[k];
        }

        return std::tuple{out, zf};
    } else {
        const auto nfilt = std::max(a.size(), b.size());
        auto y = zeros<T>(x.size());
        auto Z = std::vector(zi.cbegin(), zi.cend());
        detail::lfilter_impl(a, b, x, y, Z, nfilt);
        return std::tuple{y, Z};
    }
}

// ----------------------------------------------------------------------------
// filtfilt
// ----------------------------------------------------------------------------

enum class FiltfiltPadType : int { EVEN, ODD, CONSTANT, NONE };
enum class FiltfiltMethod : int { PAD, GUST }; // Only PAD implemented for now

namespace detail {

template <FiltfiltPadType padtype,
          typename Array,
          typename DiffTp = typename Array::difference_type>
auto validate_pad(const Array &x,
                  [[maybe_unused]] DiffTp ntaps,
                  [[maybe_unused]] DiffTp padlen = -1) {
    static_assert(meta::is_iterable_v<Array>);

    if constexpr (padtype == FiltfiltPadType::NONE) {
        return std::tuple{std::vector(x.cbegin(), x.cend()), 0};
    } else {
        const auto edge = (padlen == -1) ? 3 * ntaps : padlen;
        scicpp_require(x.size() > std::size_t(edge));

        if (edge > 0) {
            if constexpr (padtype == FiltfiltPadType::EVEN) {
                return std::tuple{even_ext(x, edge), edge};
            } else if constexpr (padtype == FiltfiltPadType::ODD) {
                return std::tuple{odd_ext(x, edge), edge};
            } else {
                return std::tuple{const_ext(x, edge), edge};
            }
        } else {
            return std::tuple{std::vector(x.cbegin(), x.cend()), edge};
        }
    }
}

} // namespace detail

template <FiltfiltPadType padtype = FiltfiltPadType::ODD,
          FiltfiltMethod method = FiltfiltMethod::PAD,
          typename Array1,
          typename Array2,
          typename Array3,
          typename DiffTp = Array1::difference_type>
auto filtfilt(const Array1 &b,
              const Array2 &a,
              const Array3 &x,
              DiffTp padlen = -1) {
    using namespace scicpp::operators;

    const auto len = DiffTp(std::max(a.size(), b.size()));
    auto [ext, edge] = detail::validate_pad<padtype>(x, len, padlen);
    const auto zi = lfilter_zi(b, a);

    // Forward filter
    auto [y, zf] = lfilter(b, a, ext, zi * ext[0]);

    // Backward filter
    const auto y_last = y.back();
    auto [y_new, zf_new] = lfilter(b, a, flip(std::move(y)), zi * y_last);
    flip_inplace(y_new);

    if (edge > 0) {
        return slice_array(y_new, edge, -edge);
    } else {
        return y_new;
    }
}

// ----------------------------------------------------------------------------
// deconvolve
// ----------------------------------------------------------------------------

// TODO
// deconvolve https://github.com/scipy/scipy/blob/df134eab5a500c2146ed4552c8674a78d8154ee9/scipy/signal/_signaltools.py#L2258

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_SIGNALTOOLS
