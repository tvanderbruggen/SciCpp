.. _signal_ifft:

scicpp::signal::ifft
====================================

Defined in header <scicpp/signal.hpp>

Inverse discrete Fourier transform of real or complex sequence.

--------------------------------------

.. function:: template <typename T> \
              std::vector<std::complex<T>> ifft(const std::vector<std::complex<T>> &y)

Performs the inverse FFT of a std::vector of complex numbers.

--------------------------------------

.. function:: template <typename T> \
              std::vector<std::complex<T>> ifft(const std::vector<T> &y)

Performs the inverse FFT of a std::vector of real numbers.