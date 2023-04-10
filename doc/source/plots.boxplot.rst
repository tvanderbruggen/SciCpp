.. _plots_boxplot:

scicpp::plots::boxplot
====================================

Defined in header <scicpp/plots.hpp>

Plot a boxplot: Represents descriptive statistics of a dataset.

Return a :expr:`sciplot::Plot2D` object.

--------------------------------------

.. function:: template <typename Array>\
              auto boxplot(const Array &x)

:expr:`x` is an array or a sequence of arrays (array of arrays).

--------------------------------------

Example
-------------------------

::

    #include <cmath>
    #include <scicpp/core.hpp>
    #include <scicpp/plots.hpp>

    namespace sci = scicpp;
    namespace plt = sci::plots;

    int main() {
        using namespace sci::operators;

        constexpr std::size_t data_size = 100;
        constexpr std::size_t data_num = 4;
        std::array<std::array<double, data_size>, data_num> data{};

        for (std::size_t i = 0; i < data.size(); ++i) {
            data[i] = sci::random::normal<data_size>(
                10.0 * std::sin(double(i - 1) / data_num), double(i + 5));
        }

        auto plot = plt::boxplot(data)
                        .showmeans(true)
                        .capwidths({0.05, 0.05, 0.15, 0.1})
                        .widths({0.1, 0.1, 0.3, 0.2});
        plot.show();
    }

.. image:: _static/boxplot.png
   :width: 600

See also
    ----------
    `Wikipedia <https://en.wikipedia.org/wiki/Box_plot>`_

    `Matplotlib documentation <https://matplotlib.org/3.1.1/api/_as_gen/matplotlib.pyplot.boxplot.html>`_
    
    `Matplotlib demo <https://matplotlib.org/3.1.1/gallery/pyplots/boxplot_demo_pyplot.html#sphx-glr-gallery-pyplots-boxplot-demo-pyplot-py>`_
