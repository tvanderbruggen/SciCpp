.. _signal_Spectrum:

scicpp::signal::Spectrum
====================================

Defined in header <scicpp/signal.hpp>

--------------------------------------

.. class:: template<typename T = double>  Spectrum

A class to configure spectral analysis parameters:

- Sampling frequency (:expr:`fs`)
- Number of overlapping points (:expr:`noverlap`)
- Window (:expr:`window`)

Once the class is configure, various spectrum estimators can be called:
:expr:`periodogram`, :expr:`welch`, :expr:`csd`, :expr:`coherence`, :expr:`tfestimate`. 

Configuration
-------------------------

.. function:: fs(T fs)

Set the sampling frequency. 1.0 by default.

--------------------------------------

.. function:: noverlap(signed_size_t noverlap)

Number of points to overlap between segments.
By default half of the window length.

--------------------------------------

.. function:: window(const std::vector<T> &window)

.. function:: window(std::vector<T> &&window)

.. function:: window(windows::Window win, std::size_t N)

Set the window. Hann window of length 256 by default.

--------------------------------------

.. function:: nthreads(std::size_t nthreads)

Number of threads to be used for parallel computing of FFTs
when using Welch's method.

--------------------------------------

Estimators
-------------------------

Scaling options::

    enum SpectrumScaling : int {
        NONE,     // No scaling
        DENSITY,  // Power spectral density
        SPECTRUM  // Power spectrum
    };

-------------------------------------

.. _signal_Spectrum_periodogram:

.. function:: template <SpectrumScaling scaling = DENSITY, typename Array> \
              auto periodogram(const Array &x)

Estimate power spectral density using a periodogram
(:expr:`noverlap` is set to zero).

-------------------------------------

.. _signal_Spectrum_welch:

.. function:: template <SpectrumScaling scaling = DENSITY, typename Array> \
              auto welch(const Array &x)

Estimate power spectral density using Welch’s method.

-------------------------------------

.. _signal_Spectrum_csd:

.. function:: template <SpectrumScaling scaling = DENSITY, \
                        typename Array1, \
                        typename Array2> \
              auto csd(const Array1 &x, const Array2 &y)

Estimate the cross power spectral density (Pxy) using Welch’s method.

-------------------------------------

.. _signal_Spectrum_coherence:

.. function:: template <typename Array1, typename Array2> \
              auto coherence(const Array1 &x, const Array2 &y)

Estimate the magnitude squared coherence estimate (Cxy) of discrete-time signals X and Y using Welch’s method.

-------------------------------------

.. _signal_Spectrum_tfestimate:

.. function:: template <typename Array1, typename Array2> \
              auto tfestimate(const Array1 &x, const Array2 &y)

Estimate the transfer function using Welch’s method.

Example
-------------------------

::

    #include <scicpp/core.hpp>
    #include <scicpp/signal.hpp>

    int main() {
        namespace sci = scicpp;
        using namespace sci::operators;
        using namespace sci::units::literals;
        using namespace std::literals::complex_literals;

        const auto N = 1E5;
        const auto fs = 1E3;
        const auto omega0 = 2_rad * sci::pi<double> * 100;

        const auto t = sci::arange(0.0, N) / fs;
        const auto noise = sci::random::randn<double>(t.size());
        const auto x = sci::cos(omega0 * t) + noise;
        const auto y = sci::signal::sawtooth(omega0.value() * t) + 5.0i * noise;

        auto spec = sci::signal::Spectrum{}.fs(fs).window(
            sci::signal::windows::Hamming, t.size());

        const auto [f1, Pxx] = spec.welch(x);
        sci::print(Pxx);

        const auto [f2, Pyy] =
            spec.welch<sci::signal::SpectrumScaling::SPECTRUM>(y);
        sci::print(Pyy);

        const auto [f3, Pxy] = spec.csd(x, y);
        sci::print(Pxy);

        const auto [f4, Cxy] = spec.coherence(1.0i * x, y);
        sci::print(Cxy);

        return 0;
    }