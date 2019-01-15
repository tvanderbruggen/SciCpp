.. _basics_stats_ptp:

scicpp::stats::ptp
====================================

Defined in header <scicpp/stats/stats.hpp>

Return the peak-to-peak span of an array.

----------------

.. function:: template <typename T, std::size_t N> \
              constexpr T ptp(const std::array<T, N> &f)

----------------

.. function:: template <typename T> \
              T ptp(const std::vector<T> &f)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.ptp.html#numpy.ptp>`_