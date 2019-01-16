# SciCpp

Yet another C++ scientific computing library.

Objective: Provide the SciPy / NumPy API for `std::array` and `std::vector`.

Scicpp is a header only library.
It's only dependence is the Eigen linear algebra theory, also header only.

See documentation (TODO link)

## Installing and using SciCpp

## Developping SciCpp

### Running the test suite

SciCpp tries to get as much test coverage as possible.
Each file library header file (ex. `my_header.hpp`) should have a
corresping test file in the same folder (`my_header.t.hpp`).

To run the tests:

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

