.. _signal_convolve:

scicpp::signal::convolve
====================================

Defined in header <scicpp/signal.hpp>

Convolve two 1D arrays (either std::array or std::vector).

For now only the "full" mode is implemented.

--------------------------------------

.. function:: template <typename T, std::size_t N, std::size_t M> \
              constexpr std::array<T, N + M - 1> convolve(const std::array<T, N> &a, const std::array<T, M> &v)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> convolve(const std::vector<T> &a, const std::vector<T> &v)

The returned std::vector is of size :expr:`a.size() + v.size() - 1`.

--------------------------------------

See also
"""""""""

:ref:`polymul <polynomial_polymul>`: polynomial multiplication is a convolution.


`Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.correlate.html>`_