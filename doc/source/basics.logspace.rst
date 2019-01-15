.. _basics_logspace:

scicpp::logspace
====================================

Defined in header <scicpp/range.hpp>

Return numbers spaced evenly on a log scale.

----------------

.. function:: template <std::size_t N, typename T, int Base = 10> \
              std::array<T, N> logspace(T start, T stop)

----------------

.. function:: template <typename T, int Base = 10> \
              std::vector<T> logspace(T start, T stop, std::size_t num)

----------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.logspace.html#numpy.logspace>`_