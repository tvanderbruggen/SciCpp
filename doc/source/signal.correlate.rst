.. _signal_correlate:

scicpp::signal::correlate
====================================

Defined in header <scicpp/signal.hpp>

Correlate two 1D arrays (either std::array or std::vector).

For now only the "full" mode is implemented.

--------------------------------------

.. function:: template <typename T, std::size_t N, std::size_t M> \
              constexpr std::array<T, N + M - 1> correlate(const std::array<T, N> &a, const std::array<T, M> &v)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> correlate(const std::vector<T> &a, const std::vector<T> &v)

The returned std::vector is of size :expr:`a.size() + v.size() - 1`.

--------------------------------------

See also
"""""""""

`Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.convolve.html#scipy.signal.convolve>`_