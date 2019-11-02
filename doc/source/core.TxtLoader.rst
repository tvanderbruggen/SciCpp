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
==============================

Load data into an Eigen matrix
-------------------------

::

    #include <scicpp/core.hpp>

    int main()
    {
        const auto m = scicpp::TxtLoader<double>()
                                    .delimiter(',')
                                    .skiprows(1)
                                    .load("tests/data0.csv");
        std::cout << m << '\n';
    }

Load data columns into vectors
-------------------------

We follow this `example <https://scipython.com/book/chapter-6-numpy/examples/using-numpys-loadtxt-method/>`_.
Here is the data file:

.. code-block:: text

    # https://scipython.com/book/chapter-6-numpy/examples/using-numpys-loadtxt-method/
    # Student data collected on 17 July 2014
    # Researcher: Dr Wicks, University College Newbury

    # The following data relate to N = 20 students. It
    # has been totally made up and so therefore is 100%
    # anonymous.

    Subject Sex    DOB      Height  Weight       BP     VO2max
    (ID)    M/F  dd/mm/yy     m       kg        mmHg  mL.kg-1.min-1
    JW-1     M    19/12/95    1.82     92.4    119/76   39.3
    JW-2     M    11/1/96     1.77     80.9    114/73   35.5
    JW-3     F    2/10/95     1.68     69.7    124/79   29.1
    JW-6     M    6/7/95      1.72     75.5    110/60   45.5
    # JW-7    F    28/3/96     1.66     72.4    101/68   -
    JW-9     F    11/12/95    1.78     82.1    115/75   32.3
    JW-10    F    7/4/96      1.60     -       -/-      30.1
    JW-11    M    22/8/95     1.72     77.2    97/63    48.8
    JW-12    M    23/5/96     1.83     88.9    105/70   37.7
    JW-14    F    12/1/96     1.56     56.3    108/72   26.0
    JW-15    F    1/6/96      1.64     65.0    99/67    35.7
    JW-16    M    10/9/95     1.63     73.0    131/84   29.9
    JW-17    M    17/2/96     1.67     89.8    101/76   40.2
    JW-18    M    31/7/96     1.66     75.1    -/-      -
    JW-19    F    30/10/95    1.59     67.3    103/69   33.5
    JW-22    F    9/3/96      1.70     -       119/80   30.9
    JW-23    M    15/5/95     1.97     89.2    124/82   -
    JW-24    F    1/12/95     1.66     63.8    100/78   -
    JW-25    F    25/10/95    1.63     64.4    -/-      28.0
    JW-26    M    17/4/96     1.69     -       121/82   39.

Here we load the data and filter out missing value, before performing some
statistical analysis.

Note that we use :code:`units` for a precise description of the data types.

::

    #include <scicpp/core.hpp>
    #include <cstdio>
    #include <string>
    #include <utility>

    namespace sci = scicpp;
    using namespace sci::operators;
    using namespace sci::units::literals;

    // We use type aliasing to define the data type of each column.
    // When the data have physical unit, we use an unit type.
    using Gender = bool; // True = M, False = F
    using Height = sci::units::meter<>;
    using Weight = sci::units::kilogram<>;
    using BloodPressure = std::pair<sci::units::mmHg<>, sci::units::mmHg<>>;

    // Replace the missing weight by an unphysical negative value
    auto parse_weight(const char *s) {
        try {
            return Weight(std::stod(s));
        } catch (...) {
            return -99_kg;
        }
    }

    auto parse_blood_pressure(const char *s) {
        double systolic, diastolic;

        if (std::sscanf(s, "%lf/%lf", &systolic, &diastolic) > 0) {
            return BloodPressure(systolic, diastolic);
        } else { // Invalid input
            return BloodPressure(-99_mmHg, -99_mmHg);
        }
    }

    int main() {
        // Use structured bindings to directly unpack the tuple
        const auto [genders, heights, weights, blood_pressures] =
            sci::TxtLoader<Gender, Height, Weight, BloodPressure>()
                .skiprows(10)
                .usecols(1, 3, 4, 5)
                .converters({{1, [](auto x) { return x[0] == 'M'; }},
                            {4, parse_weight},
                            {5, parse_blood_pressure}})
                .filters( // Use filters to keep only lines with valid (positives) values
                    {{4, [](auto x) { return sci::io::cast<Weight>(x) > 0_kg; }},
                     {5, [](auto x) { return sci::io::cast<BloodPressure>(x).first > 0_mmHg;}}})
                .load("examples/text-data.csv");

        const auto m_av = sci::stats::mean(sci::mask(heights, genders));
        const auto f_av = sci::stats::mean(sci::mask(heights, !genders));
        printf("Male average: %.2f m, Female average: %.2f m\n", m_av.value(), f_av.value());

        const auto m_wav = sci::stats::mean(sci::mask(weights, genders));
        const auto f_wav = sci::stats::mean(sci::mask(weights, !genders));
        printf("Male average: %.2f kg, Female average: %.2f kg\n", m_wav.value(), f_wav.value());

        const auto [systolic, diastolic] = sci::unpack(blood_pressures);
        const auto m_syst_av = sci::stats::mean(sci::mask(systolic, genders));
        const auto f_syst_av = sci::stats::mean(sci::mask(systolic, !genders));
        const auto m_diast_av = sci::stats::mean(sci::mask(diastolic, genders));
        const auto f_diast_av = sci::stats::mean(sci::mask(diastolic, !genders));
        printf("Male average: %.2f/%.2f mmHg, Female average: %.2f/%.2f mmHg\n",
               m_syst_av.value(),
               m_diast_av.value(),
               f_syst_av.value(),
               f_diast_av.value());
    }

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.loadtxt.html#numpy.loadtxt>`_
