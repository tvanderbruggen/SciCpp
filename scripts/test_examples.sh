#!/bin/bash
set -e

compiler=$1

make clean_examples
make COMPILER=$compiler EXAMPLE=example1 example
make COMPILER=$compiler EXAMPLE=estimate-zeros example
make COMPILER=$compiler EXAMPLE=white-noise example