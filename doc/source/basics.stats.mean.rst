.. _basics_stats_mean:

scicpp::stats::mean
====================================

Defined in header <scicpp/stats/stats.hpp>

Compute the arithmetic mean.

----------------

.. function:: template <typename T, std::size_t N> \
              constexpr T mean(const std::array<T, N> &f)

----------------

.. function:: template <typename T> \
              T mean(const std::vector<T> &f)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.mean.html#numpy.mean>`_