.. _basics_stats_var:

scicpp::stats::var
====================================

Defined in header <scicpp/stats/stats.hpp>

Compute the variance.

----------------

.. function:: template <typename T, std::size_t N> \
              constexpr T var(const std::array<T, N> &f)

----------------

.. function:: template <typename T> \
              T var(const std::vector<T> &f)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.var.html#numpy.var>`_