plots
=============================

Plotting functions defined in header <scicpp/plots.hpp>.

Based on the `Sciplot library <https://sciplot.github.io/>`_, which is a header only library.
Gnuplot is required to run the executable.
This is only required to use the plots module, not other scicpp modules.

Extends Sciplot with some specialized plots available in Matplotlib (hist, psd, cohere, csd)

Statistics
----------------

:ref:`hist <plots_hist>`
    Plot a histogram.

:ref:`boxplot <plots_boxplot>`
    Boxplots.

Spectrum
----------------

:ref:`csd, psd <plots_csd_psd>`
    Plot the spectral density.

:ref:`cohere <plots_cohere>`
    Plot the coherence between two signals.