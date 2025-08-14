#include "signal_handler.hpp"
#include "logger.hpp"

namespace p2ptransfer {

std::function<void()> SignalHandler::stop_func_;

void SignalHandler::handle_SIGINT(std::function<void()>&& stop_func) {
    using namespace std::placeholders;
    stop_func_ = std::move(stop_func);
    std::signal(SIGINT, &SignalHandler::handle_signal);
}

void SignalHandler::handle_signal([[maybe_unused]] int signal) {
    stop_func_();
    Logger::log() << "SIGINT successfully handled\n";
    std::exit(0);
}

}
