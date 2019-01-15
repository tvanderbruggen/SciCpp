.. _basics_stats_amax:

scicpp::stats::amax
====================================

Defined in header <scicpp/stats/stats.hpp>

Return the maximum value of an array.

----------------

.. function:: template <typename T, std::size_t N> \
              constexpr T amax(const std::array<T, N> &f)

----------------

.. function:: template <typename T> \
              T amax(const std::vector<T> &f)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.amax.html#numpy.amax>`_