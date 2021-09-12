// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_PLOTS
#define SCICPP_PLOTS_PLOTS

#include "scicpp/core/histogram.hpp"

#include <sciplot/sciplot.hpp>
#include <string>

namespace scicpp::plots {

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

  private:
    std::string m_fill_color = "blue";
    sciplot::Vec m_bins;
    std::vector<signed_size_t> m_hist;

    void redraw() {
        this->clear();
        this->drawStepsFilled(m_bins, m_hist)
            .fillColor(m_fill_color)
            .labelNone();
        this->border().right().top();
    }
}; // class Hist

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_PLOTS