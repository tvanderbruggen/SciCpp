.. _signal_windows_bartlett:

scicpp::signal::windows::bartlett
====================================

Defined in header <scicpp/signal/windows.hpp>

Return a Bartlett window.

The Bartlett window is very similar to a triangular window,
except that the end points are at zero. It is often used in signal processing for tapering a signal, without generating too much ripple in the frequency domain.

--------------------------------------

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> bartlett()

Return a std::array of size M. Requires M > 1.

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> bartlett(std::size_t M)

Return a std::vector of size M. Requires M > 1.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.bartlett.html#scipy.signal.windows.bartlett>`_
