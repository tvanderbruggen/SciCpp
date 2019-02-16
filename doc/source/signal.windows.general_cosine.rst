.. _signal_windows_general_cosine:

scicpp::signal::windows::general_cosine
========================================

Defined in header <scicpp/signal.hpp>

Generic weighted sum of cosine terms window.

The parameter a is a std::array that contains the weighting coefficients.
This uses the convention of being centered on the origin, 
so these will typically all be positive numbers, not alternating sign.

--------------------------------------

.. function:: template <typename T, std::size_t M, std::size_t n_weights> \
              std::array<T, M> general_cosine(const std::array<T, n_weights> &a)

Return a std::array of size M. Requires M > 1.

--------------------------------------

.. function:: template <typename T, std::size_t n_weights> \
              std::vector<T> general_cosine(std::size_t M, const std::array<T, n_weights> &a)

Return a std::vector of size M. Requires M > 1.

Example
-----------

Implements Heinzel flat-top window named “HFT90D” [1]_::

    #include <array>
    #include <scicpp/core.hpp>
    #include <scicpp/signal.hpp>
    #include <vector>

    int main()
    {
        // Define the weights
        constexpr std::array HFT90D{1., 1.942604, 1.340318, 0.440811, 0.043097};

        // Window of 16 points stored in a std::array
        const auto window_array = scicpp::signal::windows::general_cosine<double, 16>(HFT90D);
        scicpp::print(window_array);

        // Window of 8192 points stored in a std::vector
        const auto window_vector = scicpp::signal::windows::general_cosine(8192, HFT90D);
        scicpp::print(window_vector);
    }

References
    ----------
    .. [1] Heinzel G. et al., "Spectrum and spectral density estimation by the
           Discrete Fourier transform (DFT), including a comprehensive list of
           window functions and some new flat-top windows", February 15, 2002
           https://holometer.fnal.gov/GH_FFT.pdf

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.windows.general_cosine.html#scipy.signal.windows.general_cosine>`_