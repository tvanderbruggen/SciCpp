.. _basics_linspace:

scicpp::linspace
====================================

Defined in header <scicpp/range.hpp>

Return evenly spaced numbers over a specified interval.

----------------

.. function:: template <std::size_t N, typename T> \
              constexpr std::array<T, N> linspace(T start, T stop)

----------------

.. function:: template <typename T> \
              std::vector<T> linspace(T start, T stop, std::size_t num)

----------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.linspace.html>`_