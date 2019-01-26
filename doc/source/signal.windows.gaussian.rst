.. _signal_windows_gaussian:

scicpp::signal::windows::gaussian
====================================

Defined in header <scicpp/signal.hpp>

Return a Gaussian window of standard deviation *sigma*.

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> gaussian(T sigma)

.. function:: template <typename T> \
              std::vector<T> gaussian(std::size_t M, T sigma)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy-1.0.0/reference/generated/scipy.signal.gaussian.html>`_