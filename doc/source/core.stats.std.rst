.. _core_stats_std:

scicpp::stats::std
====================================

Defined in header <scicpp/core.hpp>

Compute the standard deviation.

----------------

.. function:: template <typename T, std::size_t N> \
              constexpr T std(const std::array<T, N> &f)

----------------

.. function:: template <typename T> \
              T std(const std::vector<T> &f)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.std.html#numpy.std>`_