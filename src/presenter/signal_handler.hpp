#pragma once

namespace p2ptransfer {

/// SIGINT handler for proper socket destruction.
class SignalHandler {
public:
    static void handle_SIGINT(std::function<void()>&& stop_func);
private:
    static void handle_signal([[maybe_unused]] int signal);

    static std::function<void()> stop_func_;
};

}
