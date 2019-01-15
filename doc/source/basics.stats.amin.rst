.. _basics_stats_amin:

scicpp::stats::amin
====================================

Defined in header <scicpp/stats/stats.hpp>

Return the minimum value of an array.

----------------

.. function:: template <typename T, std::size_t N> \
              constexpr T amin(const std::array<T, N> &f)

----------------

.. function:: template <typename T> \
              T amin(const std::vector<T> &f)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.amin.html#numpy.amin>`_