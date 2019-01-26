.. _signal_windows_get_window:

scicpp::signal::windows::get_window
====================================

Defined in header <scicpp/signal.hpp>

Return a window with kind defined by the enum::

    enum Window : std::size_t {
        Boxcar,
        Bartlett,
        Cosine,
        Hann,
        Hamming,
        Blackman,
        Nuttall,
        Blackmanharris,
        Flattop
    };

--------------------------------------

.. function:: template <Window win, std::size_t N, typename T = double> \
              std::array<T, N> get_window()

--------------------------------------

.. function:: template <typename T = double> \
              std::vector<T> get_window(Window win, std::size_t N)