.. _core_fromstring:

scicpp::fromstring
====================================

Defined in header <scicpp/core.hpp>

Load a vector from a string.

--------------------------------------

.. function:: template <typename DataType = double> \
              std::vector<DataType> fromstring(const char *str, char sep, const ConvertersDict &converters = {})

.. function:: template <typename DataType = double> \
              std::vector<DataType> fromstring(const std::string &str, char sep, const ConvertersDict &converters = {})

The template argument :code:`DataType` is the type of the loaded data (by default :code:`double`).

:Parameters:
:str: The string containing the data.
:sep: The character separating the data.
:converters: (Optional) A dictionary mapping column number to a function that will parse the column string into the desired value.


Example
-------------------------

::

    #include <cstdlib>
    #include <scicpp/core.hpp>

    int main()
    {
        const auto v1 = scicpp::fromstring("42, 89, 66", ',');
        scicpp::print(v1); // [42.0, 89.0, 66.0]

        const auto v2 = scicpp::fromstring("42 89 66", ' ');
        scicpp::print(v2); // [42.0, 89.0, 66.0]

        const auto v3 = scicpp::fromstring<int>(
            "42 89 66",
            ' ',
            {{0, [](auto x) { return -std::atoi(x); }},
             {1, [](auto x) { return 10 - std::atoi(x); }}});
        scicpp::print(v3); // [-42, -79, 66]
    }

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.fromstring.html#numpy.fromstring>`_
