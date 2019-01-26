.. _signal_windows_hann:

scicpp::signal::windows::hann
====================================

Defined in header <scicpp/signal.hpp>

Return a Hann window, that is a :ref:`general Hamming window <signal_windows_general_hamming>`
for :math:`\alpha = 0.5`.

--------------------------------------

.. function:: template <typename T, std::size_t M> \
              std::array<T, M> hann()

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> hann(std::size_t M)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.hann.html#scipy.signal.windows.hann>`_