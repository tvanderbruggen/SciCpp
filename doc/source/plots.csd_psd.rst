.. _plots_csd_psd:

scicpp::plots::csd, scicpp::plots::psd
====================================

Defined in header <scicpp/plots.hpp>

Plot the spectral density.

Return a :expr:`sciplot::Plot2D` object.

--------------------------------------

Plot scale options::

    enum SpectrumPlotScale : int {
        LINEAR,  // Linear scale
        DECIBEL  // Logarithm scale (dB)
    };

--------------------------------------

.. function:: template <signal::SpectrumScaling scaling = signal::DENSITY, SpectrumPlotScale plot_scale = DECIBEL, typename Array1, typename Array2, typename T = double> \
              auto csd(signal::Spectrum<T> spec, const Array1 &x, const Array2 &y)

Plot the cross-spectral density between arrays :expr:`x` and :expr:`y`
using a given :ref:`Spectrum <signal_Spectrum>` analyzer :expr:`spec`.

--------------------------------------

.. function:: template <signal::SpectrumScaling scaling = signal::DENSITY, SpectrumPlotScale plot_scale = DECIBEL, typename Array, typename T = double> \
              auto psd(signal::Spectrum<T> spec, const Array &x)

Plot the power spectral density of array :expr:`x`
using a given :ref:`Spectrum <signal_Spectrum>` analyzer :expr:`spec`.

--------------------------------------

Example
-------------------------

::

    #include <scicpp/core.hpp>
    #include <scicpp/plots.hpp>
    #include <scicpp/signal.hpp>

    namespace sci = scicpp;
    namespace plt = sci::plots;

    int main() {
        using namespace sci::operators;
        using namespace sci::units::literals;

        const auto N = 1E3;
        const auto fs = 1E2;
        const auto omega0 = 2_rad * sci::pi<double> * 10.0;

        const auto t = sci::arange(0.0, N) / fs;

        // Colored noise
        const auto nse1 = sci::random::randn<double, 2813981405>(t.size() / 2);
        const auto nse2 = sci::random::randn<double, 4130028572>(t.size() / 2);
        const auto r = sci::exp(-sci::arange(0.0, N / 2 + 1) / fs / 0.05);
        const auto cnse1 = sci::signal::fftconvolve(nse1, r) / fs;
        const auto cnse2 = sci::signal::fftconvolve(nse2, r) / fs;

        const auto x = 0.01 * sci::sin(omega0 * t) + cnse1;
        const auto y = 0.01 * sci::sin(omega0 * t) + cnse2;

        auto spec = sci::signal::Spectrum{}.fs(fs).window(
            sci::signal::windows::Hamming, t.size());

        // Plot csd
        auto plot1 = plt::csd(spec, x, y);
        plot1.size(1000, 500);
        plot1.xlabel("FREQUENCY (Hz)");
        plot1.ylabel("CSD (dB / Hz)");
        plot1.show();

        // Plot psd
        auto plot2 = plt::psd(spec, x);
        plot2.size(1000, 500);
        plot2.xlabel("FREQUENCY (Hz)");
        plot2.ylabel("PSD (dB / Hz)");
        plot2.show();
    }


.. image:: _static/csd.png
   :width: 600

.. image:: _static/psd.png
   :width: 600


See also
    ----------
    `Matplotlib documentation <https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.csd.html>`_
    
    `Matplotlib demo <https://matplotlib.org/stable/gallery/lines_bars_and_markers/csd_demo.html>`_
