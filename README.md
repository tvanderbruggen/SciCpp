[![CircleCI](https://circleci.com/gh/tvanderbruggen/SciCpp.svg?style=shield)](https://circleci.com/gh/tvanderbruggen/SciCpp)

# SciCpp

<p float="left">
  <img src="/doc/source/_static/psd.png" width="250" />
  <img src="/doc/source/_static/basic_hist.png" width="250" /> 
  <img src="/doc/source/_static/boxplot.png" width="250" /> 
</p>

Yet another C++ scientific computing library.

Objective: Provide the SciPy / NumPy API for `std::array` and `std::vector`.

Scicpp is a header only library.
It's only dependence is the Eigen linear algebra library, also header only.

SciCpp uses the C++-17 standard.
It is tested with the GCC and Clang compilers under Linux.

See [documentation](https://tvanderbruggen.github.io/scicpp/) for more details.

## Installing and using SciCpp

To use SciCpp you can directly download a release zip file.
Since SciCpp is a header library you can just unzip the content to a convenient place for you.

If you want to install it globally on your system you can unzip it in the `/usr/include` directory.

Else you can also save it directly into your project and add an include directive `-I` to the compiler to specify the path.

SciCpp requires the Eigen library for some functions.
You can install it by running the script:

```
sudo bash scripts/install_eigen.sh
```

that will install it globally on your system.

If you want to use the plotting functions, you also need to install Sciplot:

```
sudo bash scripts/install_sciplot.sh
```

## Developing SciCpp

### Setup your development environment

After cloning the repository, you can follow the [circleci configuration file](.circleci/config.yml) to setup your
development environment. Using Ubuntu 22.04 at the moment.

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

Scicpp uses the Nonius framework for benchmarking.
The header is already included in the repo.
However the Boost library is required, you can install it using:

```
sudo make setup_benchmark
```

To build and run the benchmarks:

```
make benchmark
```

List of benchmarks to run can be found [here](benchmarks/scicpp_benchmark.cpp).

You can comment out the tests you don't want to run.

### Running an example

```
make EXAMPLE=estimate-zeros example
```

### Formating the code

```
make format
```

