# SciCpp

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

