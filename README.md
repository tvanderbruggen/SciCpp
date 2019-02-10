# SciCpp

Yet another C++ scientific computing library.

Objective: Provide the SciPy / NumPy API for `std::array` and `std::vector`.

Scicpp is a header only library.
It's only dependence is the Eigen linear algebra library, also header only.

SciCpp uses the C++-17 standard.
It is tested with the GCC and Clang compilers under Linux.

See documentation (TODO link) for more details.

## Installing and using SciCpp

## Developing SciCpp

### Setup your development environment

After cloning the repository, you can follow the [circleci configuration file](.circleci/config.yml) to setup your
development environment. Only tested on Ubuntu 18.04 for now.

### Running the test suite

SciCpp tries to get as much test coverage as possible.
Each file library header file (ex. `my_header.hpp`) should have a
corresponding test file in the same folder (`my_header.t.hpp`).

To build and run the tests:

```
make test
```

List of tests to run can be found [here](tests/scicpp_test.cpp).
You can comment out the tests you don't want to run.

### Running the benchmarks

```
make benchmark
```

### Running an example

```
make EXAMPLE=example1 example
```

### Formating the code

```
make format
```

