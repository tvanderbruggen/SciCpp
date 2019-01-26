.. _signal_irfft:

scicpp::signal::irfft
====================================

Defined in header <scicpp/signal.hpp>

Inverse of the discrete Fourier transform for real input.

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> irfft(const std::vector<std::complex<T>> &y, int n = -1)

*n* is an optional parameter to specify the output length of the transform.
If *n* is smaller than :code:`y.size()` then *y* will be cropped, else
zero padding is performed.

--------------------------------------

See also
"""""""""

`Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.fft.irfft.html>`_