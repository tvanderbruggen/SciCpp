.. _signal_windows_boxcar:

scicpp::signal::windows::boxcar
====================================

Defined in header <scicpp/signal.hpp>

Return a boxcar or rectangular window.

Also known as a rectangular window or Dirichlet window, this is equivalent to no window at all.

--------------------------------------

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> boxcar()

Return a std::array of size M.

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> boxcar(std::size_t M)

Return a std::vector of size M.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.boxcar.html#scipy.signal.windows.boxcar>`_