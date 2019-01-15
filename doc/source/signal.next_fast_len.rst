.. _signal_next_fast_len:

scicpp::signal::next_fast_len
====================================

Defined in header <scicpp/signal/fft.hpp>

Find the next fast size of input data to FFT.

Useful for zero-padding.

This returns the next composite of the prime factors 2, 3, and 5 
which is greater than or equal to target.
(These are also known as 5-smooth numbers, regular numbers, or Hamming numbers.)

--------------------------------------

.. function:: template <typename Integral> \
              Integral next_fast_len(Integral n)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.fftpack.next_fast_len.html#scipy.fftpack.next_fast_len>`_