.. _signal_windows_general_hamming:

scicpp::signal::windows::general_hamming
====================================

Defined in header <scicpp/signal.hpp>

Return a generalized Hamming window defined as:

:math:`w(n) = \alpha - (1 - \alpha) \cos \left( \frac{2 \pi n}{M - 1} \right) \;\;\;\; 0 \leq n \leq M - 1.`

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> general_hamming(T alpha)

.. function:: template <typename T> \
              std::vector<T> general_hamming(std::size_t M, T alpha)

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.general_hamming.html#scipy.signal.windows.general_hamming>`_