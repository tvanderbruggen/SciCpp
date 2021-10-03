.. _core_stats_histogram:

scicpp::histogram
====================================

Defined in header <scicpp/core.hpp>

Compute the histogram of a dataset.

----------------

.. function:: template <bool density = false, bool use_uniform_bins = false, class InputIt> \
              auto histogram(InputIt first, InputIt last, const std::vector<T> &bins)

Compute the histogram of a dataset defined by a pair of iterators :expr:`(first, last)` for a given set of :expr:`bins`.

If the template parameter :expr:`density` is true then it returns the probability density, else it returns the bin count (default).

If the bins are uniformly distributed, the template parameter :expr:`use_uniform_bins` can be set to true to use an efficient algorithm.

Return the histogram.

----------------

.. function:: template <bool density = false, bool use_uniform_bins = false, class Array> \
              auto histogram(const Array &x, const std::vector<T> &bins)

Compute the histogram of a dataset defined by an array :expr:`x` for a given set of :expr:`bins`.

Return the histogram.

----------------

.. function:: template <BinEdgesMethod method, bool density = false, class Array> \
              auto histogram(const Array &x)

Compute the histogram of a dataset defined by an array :expr:`x` and compute the bins using a given :ref:`BinEdgesMethod <core_stats_histogram_bin_edges>`.

Return a pair [histogram, bins].

----------------

.. function:: template <bool density = false, class Array> \
              auto histogram(const Array &x, std::size_t nbins = 10)

Compute the histogram of a dataset defined by an array :expr:`x` using a number of bins :expr:`nbins` (default 10).

Return a pair [histogram, bins].

----------------

Example
-------------------------

::

    #include <scicpp/core.hpp>

    int main() {
        using namespace scicpp::stats;
        const auto a = std::array{0, 0, 0, 1, 2, 3, 3, 4, 5};
        const auto [hist_a, bins_a] = histogram<BinEdgesMethod::AUTO>(a);
        scicpp::print(bins_a);
        scicpp::print(hist_a);

        const auto b = std::vector{0., 1.25, 2.5, 3.75, 5.};
        const auto [hist_b, bins_b] = histogram(b, 4);
        scicpp::print(bins_b);
        scicpp::print(hist_b);

        using namespace scicpp::units::literals;
        const auto c = std::array{0_V, 0_V, 0_V, 1_V, 2_V, 3_V, 3_V, 4_V, 5_V};
        const auto [hist_c, bins_c] = histogram<Density>(c);
        scicpp::print(bins_c);
        scicpp::print(hist_c);
    }

See also
    ----------
    `Scipy documentation <https://numpy.org/doc/stable/reference/generated/numpy.histogram.html>`_