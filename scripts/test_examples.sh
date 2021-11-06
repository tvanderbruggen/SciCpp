#!/bin/bash
set -e

compiler=$1

make clean_examples
make COMPILER=$compiler EXAMPLE=example1 example
make COMPILER=$compiler EXAMPLE=example1 example_py
make COMPILER=$compiler EXAMPLE=estimate-zeros example
make COMPILER=$compiler EXAMPLE=estimate-zeros example_py
make COMPILER=$compiler EXAMPLE=white-noise example
make COMPILER=$compiler EXAMPLE=white-noise example_py
make COMPILER=$compiler EXAMPLE=load-text-data example
make COMPILER=$compiler EXAMPLE=wave-height example
make COMPILER=$compiler EXAMPLE=photodetector-noise example

# doc examples

make COMPILER=$compiler EXAMPLE=doc/core_stats_median example
make COMPILER=$compiler EXAMPLE=doc/core_stats_mean example
make COMPILER=$compiler EXAMPLE=doc/core_stats_var example
make COMPILER=$compiler EXAMPLE=doc/core_map example
make COMPILER=$compiler EXAMPLE=doc/core_fromstring example
make COMPILER=$compiler EXAMPLE=doc/core_stats_histogram_bin_edges example
make COMPILER=$compiler EXAMPLE=doc/core_stats_histogram example
make COMPILER=$compiler EXAMPLE=doc/polynomial_Polynomial example
make COMPILER=$compiler EXAMPLE=doc/polynomial_polydiv example
make COMPILER=$compiler EXAMPLE=doc/linalg_lstsq example
make COMPILER=$compiler EXAMPLE=doc/signal_windows_general_cosine example
make COMPILER=$compiler EXAMPLE=doc/signal_spectrum example

# Plots => Build only
make COMPILER=$compiler EXAMPLE=plot_hist example_build
