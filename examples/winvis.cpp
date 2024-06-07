#include <scicpp/plots.hpp>
#include <scicpp/signal.hpp>

namespace plt = scicpp::plots;
namespace win = scicpp::signal::windows;

int main() {
    // plt::winvis(win::Hann);
    // plt::winvis(win::Bohman);
    plt::winvis(win::Hann, win::Bohman, win::Flattop);
}
