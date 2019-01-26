.. _signal_windows_hamming:

scicpp::signal::windows::hamming
====================================

Defined in header <scicpp/signal.hpp>

Return a Hamming window, that is a :ref:`general Hamming window <signal_windows_general_hamming>`
for :math:`\alpha = 0.54`.

--------------------------------------

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> hamming()

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> hamming(std::size_t M)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.hamming.html#scipy.signal.windows.hamming>`_