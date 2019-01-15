.. _signal_windows_blackmanharris:

scicpp::signal::windows::blackmanharris
=======================================

Defined in header <scicpp/signal/windows.hpp>

Return a minimum 4-term Blackman-Harris window.

--------------------------------------

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> blackmanharris()

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> blackmanharris(std::size_t M)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.blackmanharris.html#scipy.signal.windows.blackmanharris>`_