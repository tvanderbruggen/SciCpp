.. Scicpp documentation master file, created by
   sphinx-quickstart on Sat Dec  8 16:32:58 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

SciCpp's documentation
==================================

SciCpp has for objective to provide an implementation of the `SciPy <https://scipy.org/>`_ 
functions for C++ 1D array types: ``std::array`` and ``std::vector``.

SciCpp is a lightweight header only library.
Its only dependence is the `Eigen <http://eigen.tuxfamily.org/index.php?title=Main_Page>`_
linear algebra library, which is also header only.

SciCpp is released under the MIT licence.

Modules:
--------

.. toctree::
   :maxdepth: 2

   core.rst
   linalg.rst
   polynomial.rst
   signal.rst

Introduction
--------

Array generation
*****************

Size related parameters are template parameters for ``std::array`` and function parameters for ``std::vector``.
For example:
::
    // Generate fixed size arrays (std::array):
    //     The fixed array size (5) is specified as the template parameter
    auto a = scicpp::linspace<double, 5>(1., 10.);
    auto a = scicpp::rand<double, 5>();
    auto a = scicpp::signal::hann<double, 5>();
    
    // Generate dynamic size arrays (std::vector):
    //     The dynamic array size (5) is specified as the function parameter
    auto v = scicpp::linspace<double>(1., 10., 5);
    auto v = scicpp::rand<double>(5);
    auto v = scicpp::signal::hann<double>(5);

Using arrays
*****************

Most functions accept both fixed sized (``std::array``) and dynamic size (``std::vector``) arrays:
::
    auto a = scicpp::rand<double, 5>(); // std::array
    auto v = scicpp::rand<double>(5);   // std::vector

    // No size related parameter: same call
    double m = scicpp::mean(a);
    double m = scicpp::mean(v);

    auto c = scicpp::cumsum(a);
    auto c = scicpp::cumsum(v);

    // A size related parameter:
    //     Here, the returned array size depends on the number of differentiations
    auto d = scicpp::diff<2>(a); // Fixed template parameter
    auto d = scicpp::diff(v, 2); // Dynamic function parameter


Shoudl I use a fixed size or a dynamic array ?
*****************

Well, basicaly if you know the size at compile time use a ``std::array`` else use a ``std::vector``.

Sometimes the size is known at compile but it is large (the exact size depends on your system and on 
your workload but let's say a few 1000s), then you might want to prefer to use a ```std::vector```.

Finally, ```std::vector``` is a resizable array, so it is well suited for algorithms requiring dynamical
resizing of the data container (ex. :ref:`filter <core_filter>`).