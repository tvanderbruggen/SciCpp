.. _signal_fftconvolve:

scicpp::signal::fftconvolve
====================================

Defined in header <scicpp/signal/convolve.hpp>

Convolve two 1D arrays using FFT.

For now only the "full" mode is implemented.

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> fftconvolve(const std::vector<T> &a, const std::vector<T> &v)

Convolve two std::vector using FFT and return a std::vector of size :expr:`a.size() + v.size() - 1`.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.fftconvolve.html#scipy.signal.fftconvolve>`_