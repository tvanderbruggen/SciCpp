#include <scicpp/plots.hpp>
#include <scicpp/signal.hpp>

namespace plt = scicpp::plots;
namespace win = scicpp::signal::windows;

int main() {
    plt::winvis(win::Window::Hann,
                win::Window::Bohman,
                win::Window::Flattop,
                win::Window::Parzen);
    plt::winvis(win::Window::Hann, win::Window::Lanczos);
}
