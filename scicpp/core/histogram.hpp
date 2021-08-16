// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_HISTOGRAM
#define SCICPP_CORE_HISTOGRAM

#include "scicpp/core/constants.hpp"
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

enum BinEdgesMethod : int { SCOTT, SQRT, RICE, STURGES };

namespace detail {

template <BinEdgesMethod method = SQRT, typename Array>
auto bin_width(const Array &x) {
    using T = typename Array::value_type;

    if constexpr (method == SQRT) {
        return ptp(x) / sqrt(x.size());
    } else if constexpr (method == SCOTT) {
        return cbrt(T{24} * sqrt(pi<T>) / T(x.size())) * std(x);
    } else if constexpr (method == RICE) {
        return T{0.5} * ptp(x) / cbrt(x.size());
    } else if constexpr (method == STURGES) {
        return ptp(x) / (log2(x.size()) + 1.0);
    } else {
        scicpp_unreachable;
    }
}

} // namespace detail

template <BinEdgesMethod method = SQRT, typename Array>
auto histogram_bin_edges(const Array &x) {
    const auto width = detail::bin_width<method>(x);
    const auto first_edge = amin(x);
    const auto last_edge = amax(x);

    if (std::fpclassify(units::value(width)) == FP_ZERO) {
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