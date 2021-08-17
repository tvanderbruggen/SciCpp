// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_HISTOGRAM
#define SCICPP_CORE_HISTOGRAM

#include "scicpp/core/constants.hpp"
#include "scicpp/core/equal.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace scicpp::stats {

enum BinEdgesMethod : int { SCOTT, SQRT, RICE, STURGES, FD, DOANE, AUTO };

namespace detail {

template <BinEdgesMethod method, typename Array>
auto bin_width(const Array &x) {
    scicpp_require(!x.empty());
    using T = typename Array::value_type;
    using raw_t = typename units::representation_t<T>;

    if constexpr (method == SQRT) {
        return ptp(x) / sqrt(x.size());
    } else if constexpr (method == SCOTT) {
        return cbrt(raw_t{24} * sqrt(pi<raw_t>) / raw_t(x.size())) * std(x);
    } else if constexpr (method == RICE) {
        return raw_t{0.5} * ptp(x) / cbrt(x.size());
    } else if constexpr (method == STURGES) {
        return ptp(x) / (log2(x.size()) + 1.0);
    } else if constexpr (method == FD) {
        // Freedman-Diaconis histogram bin estimator.
        return raw_t{2} * iqr(x) / cbrt(x.size());
    } else if constexpr (method == DOANE) {
        if (x.size() <= 2) {
            return T{0};
        }

        const auto sg1 = sqrt(raw_t{6.0} * (x.size() - 2) /
                              raw_t((x.size() + 1.0) * (x.size() + 3.0)));
        const auto g1 = units::value(skew(x));

        if (std::isnan(g1)) {
            return T{0};
        }

        return ptp(x) / (1.0 + log2(x.size()) + log2(1.0 + absolute(g1) / sg1));
    } else { // AUTO
        const auto fd_bw = bin_width<FD>(x);
        const auto sturges_bw = bin_width<STURGES>(x);

        if (units::fpclassify(fd_bw) == FP_ZERO) {
            return sturges_bw;
        }

        return units::fmin(fd_bw, sturges_bw);
    }
}

} // namespace detail

template <BinEdgesMethod method = SQRT, typename Array>
auto histogram_bin_edges(const Array &x) {
    using T = typename Array::value_type;

    if (x.empty()) {
        return linspace(T{0}, T{1}, 2);
    }

    // get outer edges
    auto first_edge = amin(x);
    auto last_edge = amax(x);

    if (almost_equal(first_edge, last_edge)) {
        first_edge -= T{0.5};
        last_edge += T{0.5};
    }

    const auto width = detail::bin_width<method>(x);

    if (units::fpclassify(width) == FP_ZERO) {
        return linspace(first_edge, last_edge, 2);
    }

    const auto n_equal_bins =
        std::size_t(units::value(ceil((last_edge - first_edge) / width)));
    return linspace(first_edge, last_edge, n_equal_bins + 1);
}

template <typename T>
class Histogram {
  public:
    auto bins(const std::vector<T> &bins) {
        m_bins = bins;
        return *this;
    }

    auto bins(std::vector<T> &&bins) {
        m_bins = std::move(bins);
        return *this;
    }

    auto bins(signed_size_t nbins) {
        m_nbins = nbins;
        return *this;
    }

  private:
    signed_size_t m_nbins = 10;
    std::vector<T> m_bins;

}; // class Histogram

} // namespace scicpp::stats

#endif // SCICPP_CORE_HISTOGRAM