.. _signal_unit_impulse:

scicpp::signal::unit_impulse
====================================

Defined in header <scicpp/signal/waveforms.hpp>

Unit impulse signal (discrete delta function) or unit basis vector.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              constexpr std::array<T, N> unit_impulse(std::size_t idx = 0)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> unit_impulse(std::size_t len, std::size_t idx = 0)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.unit_impulse.html#scipy.signal.unit_impulse>`_