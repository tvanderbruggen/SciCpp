.. _signal_fft:

scicpp::signal::fft
====================================

Defined in header <scicpp/signal.hpp>

Discrete Fourier transform of real or complex sequence.

--------------------------------------

.. function:: template <typename T> \
              std::vector<std::complex<T>> fft(const std::vector<std::complex<T>> &x)

Performs the FFT of a std::vector of complex numbers.

--------------------------------------

.. function:: template <typename T> \
              std::vector<std::complex<T>> fft(const std::vector<T> &x)

Performs the FFT of a std::vector of real numbers.

You may want to consider using :ref:`rfft <signal_rfft>`.