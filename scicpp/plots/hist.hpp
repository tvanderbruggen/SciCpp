// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_PLOTS
#define SCICPP_PLOTS_PLOTS

#include "scicpp/core/histogram.hpp"

#include <sciplot/sciplot.hpp>
#include <string>
#include <utility>
#include <vector>

namespace scicpp::plots {

enum HistType : int { BAR, BARSTACKED, STEP, STEPFILLED };

namespace detail {

template <typename HistVector>
struct histplot : sciplot::Plot {
  public:
    template <typename Array>
    histplot(HistVector &&hist, const Array &bins)
        : m_hist(std::move(hist)),
          m_bins(sciplot::Vec(bins.data(), bins.size())) {
        this->xrange(bins.front(), bins.back());
        redraw();
    }

    auto color(const std::string &fill_color) {
        m_fill_color = fill_color;
        redraw();
        return *this;
    }

    auto log(bool logscale) {
        m_logscale = logscale;

        if (m_logscale) {
            this->ytics().logscale(10);
        }

        return *this;
    }

    auto histtype(HistType hist_type) {
        m_hist_type = hist_type;
        redraw();
        return *this;
    }

    auto rwidth(double rwidth) {
        m_rwidth = rwidth;
        redraw();
        return *this;
    }

  private:
    std::string m_fill_color = "blue";
    HistVector m_hist;
    sciplot::Vec m_bins;
    bool m_logscale = false;
    HistType m_hist_type = BAR;
    double m_rwidth = 1.0;

    void redraw() {
        this->clear();

        if (m_hist_type == BAR) {
            this->drawBoxes(m_bins, m_hist).fillColor(m_fill_color).labelNone();
            this->boxWidthRelative(m_rwidth);
        } else if (m_hist_type == STEPFILLED) {
            this->drawStepsFilled(m_bins, m_hist)
                .fillColor(m_fill_color)
                .labelNone();
        } else {
            // TODO: BARSTACKED, STEP
        }

        this->border().right().top();

        if (m_logscale) {
            this->ytics().logscale(10);
        }
    }
}; // class histplot

} // namespace detail

template <bool use_uniform_bins = false,
          typename Array,
          typename T = typename Array::value_type>
auto hist(const Array &x, const std::vector<T> &bins) {
    return detail::histplot(stats::histogram<use_uniform_bins>(x, bins), bins);
}

template <stats::BinEdgesMethod method, typename Array>
auto hist(const Array &x) {
    auto [hist_res, bins] = stats::histogram<method>(x);
    return detail::histplot(std::move(hist_res), std::move(bins));
}

template <typename Array>
auto hist(const Array &x, std::size_t nbins = 10) {
    auto [hist_res, bins] = stats::histogram(x, nbins);
    return detail::histplot(std::move(hist_res), std::move(bins));
}

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_PLOTS