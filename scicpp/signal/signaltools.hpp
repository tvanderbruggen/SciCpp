#ifndef SCICPP_SIGNAL_SIGNALTOOLS
#define SCICPP_SIGNAL_SIGNALTOOLS

#include "scicpp/core/equal.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/linalg/matrices.hpp"
#include "scicpp/linalg/solve.hpp"
#include "scicpp/signal/arraytools.hpp"
#include "scicpp/signal/convolve.hpp"

#include <Eigen/Dense>
#include <array>
#include <complex>
#include <tuple>
#include <vector>

namespace scicpp::signal {

enum class PadType : int { EVEN, ODD, CONSTANT, NONE };
// enum class METHOD : int { PAD, GUST };

namespace detail {

template <PadType padtype,
          typename Array,
          typename DiffTp = typename Array::difference_type>
auto validate_pad(const Array &x,
                  [[maybe_unused]] DiffTp ntaps,
                  [[maybe_unused]] DiffTp padlen = -1) {
    static_assert(meta::is_iterable_v<Array>);

    if constexpr (padtype == PadType::NONE) {
        return std::tuple{std::vector(x.cbegin(), x.cend()), 0};
    } else {
        const auto edge = (padlen == -1) ? 3 * ntaps : padlen;
        scicpp_require(x.size() > std::size_t(edge));

        if (edge > 0) {
            if constexpr (padtype == PadType::EVEN) {
                return std::tuple{even_ext(x, edge), edge};
            } else if constexpr (padtype == PadType::ODD) {
                return std::tuple{odd_ext(x, edge), edge};
            } else {
                return std::tuple{const_ext(x, edge), edge};
            }
        } else {
            return std::tuple{std::vector(x.cbegin(), x.cend()), edge};
        }
    }
}

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

// ----------------------------------------------------------------------------
// lfilter_zi
// ----------------------------------------------------------------------------

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
        k < (a.size() - 1) &&
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
auto lfilter_impl(
    const A &a, const B &b, const U &x, V &y, W &Z, std::size_t nfilt) {
    for (std::size_t k = 0; k < x.size(); k++) {
        y[k] = Z[0] + b[0] / a[0] * x[k];

        for (std::size_t n = 0; n < nfilt - 2; n++) {
            Z[n] = Z[n + 1] + x[k] * b[n + 1] / a[0] - y[k] * a[n + 1] / a[0];
        }

        Z.back() = x[k] * b[nfilt - 1] / a[0] - y[k] * a[nfilt - 1] / a[0];
    }
}

} // namespace detail

template <typename T>
auto lfilter(const std::vector<T> &b,
             const std::vector<T> &a,
             const std::vector<T> &x) {
    using namespace operators;
    scicpp_require(!almost_equal(a[0], T(0)));
    scicpp_require(!x.empty());

    if (a.size() == 1) {
        auto out = convolve(b / a[0], x);
        out.resize(out.size() - b.size() + 1);
        return out;
    } else {
        auto a_ = a;
        auto b_ = b;
        const auto nfilt = detail::zfill(a_, b_);
        auto y = zeros<T>(x.size());
        auto Z = zeros<T>(nfilt - 1);
        detail::lfilter_impl(a_, b_, x, y, Z, nfilt);
        return y;
    }
}

template <typename T>
auto lfilter(const std::vector<T> &b,
             const std::vector<T> &a,
             const std::vector<T> &x,
             const std::vector<T> &zi) {
    using namespace operators;
    scicpp_require(!almost_equal(a[0], T(0)));
    scicpp_require(!x.empty());
    scicpp_require(zi.size() == std::max(a.size(), b.size()) - 1);

    if (a.size() == 1) {
        auto out = convolve(b / a[0], x);
        std::vector<T> zf(out.end() - signed_size_t(b.size()) + 1, out.end());
        out.resize(out.size() - b.size() + 1);

        for (std::size_t k = 0; k < zi.size(); ++k) {
            out[k] += zi[k];
        }

        return std::tuple{out, zf};
    } else {
        auto a_ = a;
        auto b_ = b;
        const auto nfilt = detail::zfill(a_, b_);
        auto y = zeros<T>(x.size());
        auto Z = zi;
        detail::lfilter_impl(a_, b_, x, y, Z, nfilt);
        return std::tuple{y, Z};
    }
}

// template <typename T, PADTYPE ptype = PADTYPE::ODD, METHOD method = METHOD::PAD>
// auto filtfilt(std::vector<T> &b,
//               std::vector<T> &a,
//               std::vector<T> &x,
//               int axis = 0,
//               int padlen = -1,
//               std::optional<int> irlen = std::nullopt) {
//     using namespace scicpp::operators;
//     if (method == METHOD::GUST)
//         ;
//     auto [ext, edge] = detail::_validate_pad<T, ptype>(
//         x, 0, std::max(a.size(), b.size()), padlen);

//     const auto zi = lfilter_zi(b, a);

//     T x0 = ext[0];

//     // Forward filter.
//     auto [y, zf] = lfilter(b, a, ext, zi * x0);

//     // Backward filter.
//     // Create y0 so zi*y0 broadcasts appropriately.
//     T y0 = y[y.size() - 1];
//     auto y_rev = utils::set_array(y);

//     std::reverse_copy(y.begin(), y.end(), y_rev.begin());

//     auto [y_new, zf_new] = lfilter(b, a, y_rev, zi * y0);

//     // Reverse y.
//     std::reverse(y_new.begin(), y_new.end());

//     if (edge > 0)
//         // Slice the actual signal from the extended signal.
//         y_new = axis_slice(y_new, edge, -edge);

//     return y_new;
// }

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_SIGNALTOOLS
