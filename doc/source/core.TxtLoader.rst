.. _core_TxtLoader:

scicpp::TxtLoader
====================================

Defined in header <scicpp/core.hpp>

Load data from a character separated values text file.

--------------------------------------

.. class:: template <typename... DataTypes> TxtLoader

:code:`DataTypes...` are the data types to be loaded.

If a single data type is provided the data will be loaded into an Eigen matrix of the given type.

If several data types are provided then the data are loaded into a tuple of vectors.
For example, if :code:`DataTypes... = int, double, bool` then the :code:`load` function returns a
:code:`std::tuple<std::vector<int>, std::vector<double>, std::vector<bool>>`.
In that case the number of data types must be equal to the number of used columns.

--------------------------------------

Loader configuration
-------------------------

.. function:: delimiter(char delimiter)

Set the character used to separate values. Default is a whitespace :code:`' '`.

--------------------------------------

.. function:: skiprows(int skiprows)

Number of rows to be skip at top of the file. Default value is :code:`0`.

--------------------------------------

.. function:: comments(char comments)

Character used to indicate the start of a commented row. Default is :code:`'#'`.

--------------------------------------

.. function:: usecols(const std::vector<int> &usecols)

.. function:: usecols(std::vector<int> &&usecols)

.. function:: template <typename... Columns> usecols(Columns... usecols)

Columns to be loaded. First column is 0. Can be specified either as a :code:`std::vector<int>` or as a list of integers.
By default all columns are loaded.

--------------------------------------

.. function:: converters(ConvertersDict converters)

A dictionary mapping column number to a function that will parse the column string into the desired value.
By default no converter is used.

--------------------------------------

.. function:: max_rows(int max_rows)

Set the maximum number of lines to be read. By default all lines are read.

--------------------------------------

.. function:: filters(FiltersDict filters)

A dictionary mapping column number to a function that defines data filters.
For example :code:`filters({{2, [](auto x) { return io::cast<int>(x) < 12; }}})`
will keep only the rows where column 2 data are positive.
Values must be cast into the column :code:`DataType` using :code:`io::cast<DataType>(x)`.
By default no filter is used.

--------------------------------------

File data loading
-------------------------

.. function:: Eigen::Matrix<DataType, Eigen::Dynamic, Eigen::Dynamic> load(const std::filesystem::path &fname)

.. function:: template <bool pack = false> \
              std::tuple<std::vector<DataTypes>...> load(const std::filesystem::path &fname)

.. function:: template <bool pack = true> \
              std::vector<std::tuple<DataTypes...>> load(const std::filesystem::path &fname)

Load a character separated file with path :code:`fname`.
In case several data types are specified, the :code:`pack` template parameter specifies whether the function returns
a tuple id vectors (:code:`pack = false`, default behavior) or a vector of tuple (:code:`pack = true`).

--------------------------------------

Examples
-------------------------

::
    TODO

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.loadtxt.html#numpy.loadtxt>`_
