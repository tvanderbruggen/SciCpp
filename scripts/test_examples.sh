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

# doc examples

make EXAMPLE=doc/core_stats_mean example
make EXAMPLE=doc/core_stats_var example
make EXAMPLE=doc/core_map example
make EXAMPLE=doc/polynomial_Polynomial example
make EXAMPLE=doc/polynomial_polydiv example
make EXAMPLE=doc/linalg_lstsq example
make EXAMPLE=doc/signal_windows_general_cosine example
