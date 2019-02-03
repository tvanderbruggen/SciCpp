.. _signal_sawtooth:

scicpp::signal::sawtooth
====================================

Defined in header <scicpp/signal.hpp>

Return a periodic sawtooth or triangle waveform.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              std::array<T, N> sawtooth(const std::array<T, N> &t, T width = 1)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> sawtooth(const std::vector<T> &t, T width = 1)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.sawtooth.html#scipy.signal.sawtooth>`_