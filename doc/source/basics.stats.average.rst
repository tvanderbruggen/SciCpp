.. _basics_stats_average:

scicpp::stats::average
====================================

Defined in header <scicpp/stats/stats.hpp>

Compute the weighted average.

----------------

.. function:: template <typename T, std::size_t N> \
              T average(const std::array<T, N> &f, const std::array<T, N> &weights)

----------------

.. function:: template <typename T> \
              T average(const std::vector<T> &f, const std::vector<T> &weights)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.average.html#numpy.average>`_