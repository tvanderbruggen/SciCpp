.. _signal_fftfreq:

scicpp::signal::fftfreq
====================================

Defined in header <scicpp/signal.hpp>

Return the Discrete Fourier Transform sample frequencies.

The returned array contains the frequency bin centers
in cycles per unit of the sample spacing (with zero at the start).

For instance, if the sample spacing d is in seconds,
then the frequency unit is cycles/second (Hz).

--------------------------------------

.. function::  template <std::size_t N, typename T = double> \
               constexpr std::array<T, N> fftfreq(T d = 1.0)
               
--------------------------------------

.. function::  template <typename T> \
               std::vector<T> fftfreq(std::size_t n, T d = 1.0)