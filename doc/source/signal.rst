signal
=============================

Signal processing module.

Convolution
-------------

:ref:`convolve <signal_convolve>`
    Convolve two arrays.

:ref:`fftconvolve <signal_fftconvolve>`
    Convolve two arrays using FFT.

:ref:`correlate <signal_correlate>`
    Correlate two arrays.

Fast Fourier Transforms (FFTs)
-------------------------------

:ref:`fft <signal_fft>`
    Discrete Fourier transform of real or complex sequence.

:ref:`ifft <signal_ifft>`
    Inverse discrete Fourier transform of real or complex sequence.

:ref:`rfft <signal_rfft>`
    Discrete Fourier transform of a real sequence.

:ref:`irfft <signal_irfft>`
    Inverse of the discrete Fourier transform for real input.

FFT Helper Functions
-------------------------------

:ref:`fftfreq <signal_fftfreq>`
    Return the Discrete Fourier Transform sample frequencies.

:ref:`rfftfreq <signal_rfftfreq>`
    Return the Discrete Fourier Transform sample frequencies.

:ref:`fftshift <signal_fftshift>`
    Shift the zero-frequency component to the center of the spectrum.

:ref:`ifftshift <signal_ifftshift>`
    The inverse of fftshift.

:ref:`next_fast_len <signal_next_fast_len>`
    Find the next fast size of input data to FFT.

:ref:`zero_padding <signal_zero_padding>`
    Performs zero padding of an array.

Window Functions
-----------------

:ref:`windows::get_window <signal_windows_get_window>`
    Return a window.

:ref:`windows::boxcar <signal_windows_boxcar>`
    Return a boxcar or rectangular window.

:ref:`windows::bartlett <signal_windows_bartlett>`
    Return a Bartlett window.

:ref:`windows::cosine <signal_windows_cosine>`
    Return a window with a cosine shape.

:ref:`windows::bohman <signal_windows_bohman>`
    Return a Bohman window.

:ref:`windows::general_cosine <signal_windows_general_cosine>`
    Generic weighted sum of cosine terms window.

:ref:`windows::general_hamming <signal_windows_general_hamming>`
    Return a generalized Hamming window.

:ref:`windows::hann <signal_windows_hann>`
    Return a Hann window.

:ref:`windows::hamming <signal_windows_hamming>`
    Return a Hamming window.

:ref:`windows::blackman <signal_windows_blackman>`
    Return a Blackman window.

:ref:`windows::nutall <signal_windows_nuttall>`
    Return a Nutall window.

:ref:`windows::blackmanharris <signal_windows_blackmanharris>`
    Return a minimum 4-term Blackman-Harris window.

:ref:`windows::flattop <signal_windows_flattop>`
    Return a flat top window.

:ref:`windows::gaussian <signal_windows_gaussian>`
    Return a Gaussian window.

:ref:`windows::general_gaussian <signal_windows_general_gaussian>`
    Return a generalized Gaussian window.

:ref:`windows::kaiser <signal_windows_kaiser>`
    Return a Kaiser window.

:ref:`windows::parzen <signal_windows_parzen>`
    Return a Parzen (de la Vallée Poussin) window.

:ref:`windows::lanczos <signal_windows_lanczos>`
    Return a Lanczos (sinc) window.

:ref:`windows::tukey <signal_windows_tukey>`
    Return a Tukey window.

:ref:`windows::enbw <signal_windows_enbw>`
    Return the equivalent noise bandwidth of a window.

Spectral analysis
------------------

:ref:`Spectrum <signal_Spectrum>`
    A class to configure spectral analysis parameters.

:ref:`Spectrum::periodogram <signal_Spectrum_periodogram>`
    Estimate power spectral density using a periodogram.

:ref:`Spectrum::welch <signal_Spectrum_welch>`
    Estimate power spectral density using Welch’s method.

:ref:`Spectrum::csd <signal_Spectrum_csd>`
    Estimate the cross power spectral density using Welch’s method.

:ref:`Spectrum::coherence <signal_Spectrum_coherence>`
    Estimate the magnitude squared coherence estimate of discrete-time signals using Welch’s method.

:ref:`Spectrum::tfestimate <signal_Spectrum_tfestimate>`
    Estimate the transfer function using Welch’s method.

Waveforms
-----------

:ref:`unit_impulse <signal_unit_impulse>`
    Unit impulse signal (discrete delta function) or unit basis vector.

:ref:`sawtooth <signal_sawtooth>`
    Return a periodic sawtooth or triangle waveform.

:ref:`sweep_poly <signal_sweep_poly>`
    Frequency-swept cosine generator, with a time-dependent frequency.