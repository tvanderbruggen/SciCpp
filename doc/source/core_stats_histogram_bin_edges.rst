.. _core_stats_histogram_bin_edges:

scicpp::histogram_bin_edges
====================================

Defined in header <scicpp/core.hpp>

Compute the the edges of the bins for an histogram.

Return a sorted :expr:`std::vector` with uniformly distributed bin edges.

----------------

.. function:: template <typename Array> \
              auto histogram_bin_edges(const Array &x, std::size_t nbins = 10)

:expr:`nbins` defines the number of equal-width bins in the given range (10, by default).

----------------

.. function:: template <BinEdgesMethod method, typename Array> \
              auto histogram_bin_edges(const Array &x)

Compute the bin edges using a given method::

    enum class BinEdgesMethod : int {
        SCOTT,   // Less robust estimator that that takes into account data variability and data size.
        SQRT,    // Square root (of data size) estimator, used by Excel and other programs for its speed and simplicity.
        RICE,    // Estimator does not take variability into account, only data size. Commonly overestimates number of bins required.
        STURGES, // R’s default method, only accounts for data size.
        FD,      // Freedman Diaconis Estimator: Robust (resilient to outliers) estimator.
        DOANE,   // An improved version of Sturges’ estimator that works better with non-normal datasets.
        AUTO     // Maximum of the ‘sturges’ and ‘fd’ estimators. Provides good all around performance.
    };

----------------

Example
-------------------------

::

    #include <scicpp/core.hpp>

    int main() {
        using namespace scicpp::stats;
        const auto a = std::array{0, 0, 0, 1, 2, 3, 3, 4, 5};
        const auto bins0 = histogram_bin_edges<BinEdgesMethod::AUTO>(a);
        scicpp::print(bins0);

        const auto b = std::vector{0., 1.25, 2.5, 3.75, 5.};
        const auto bins1 = histogram_bin_edges(b);
        scicpp::print(bins1);

        using namespace scicpp::units::literals;
        const auto c = std::array{0_V, 0_V, 0_V, 1_V, 2_V, 3_V, 3_V, 4_V, 5_V};
        const auto bins2 = histogram_bin_edges(c, 20);
        scicpp::print(bins2);
    }

See also
    ----------
    `Scipy documentation <https://numpy.org/doc/stable/reference/generated/numpy.histogram_bin_edges.html#numpy.histogram_bin_edges>`_