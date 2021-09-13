// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_PLOTS
#define SCICPP_PLOTS_PLOTS

#include "scicpp/core/histogram.hpp"

#include <sciplot/sciplot.hpp>
#include <string>

namespace scicpp::plots {

enum HistType : int { BAR, BARSTACKED, STEP, STEPFILLED };

struct hist : sciplot::Plot {
  public:
    template <stats::BinEdgesMethod method = stats::AUTO, typename Array>
    hist(const Array &x) {
        auto [hist, bins] = stats::histogram<method>(x);
        m_hist = std::move(hist);
        m_bins = sciplot::Vec(bins.data(), bins.size());

        this->xrange(bins.front(), bins.back());
        redraw();
    }

    auto color(const std::string &fill_color) {
        m_fill_color = fill_color;
        redraw();
        return (*this);
    }

    auto log(bool logscale) {
        m_logscale = logscale;
        redraw();
        return (*this);
    }

    auto histtype(HistType hist_type) {
        m_hist_type = hist_type;
        redraw();
        return (*this);
    }

    auto rwidth(double rwidth) {
        m_rwidth = rwidth;
        redraw();
        return (*this);
    }

  private:
    std::string m_fill_color = "blue";
    sciplot::Vec m_bins;
    std::vector<signed_size_t> m_hist;
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
            this->ytics().logscale(2);
        }
    }
}; // class hist

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_PLOTS