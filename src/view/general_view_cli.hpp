#pragma once
#include "general_view.hpp"

namespace p2ptransfer {
namespace view {

/// \ref View implementation for Command Line Interface.
class GeneralViewCLI : public GeneralView {
public:
    int run() override;
    void show_address(const Address& address) override;
    void show_connected(const Address& address, Port port) override;
    void show_socket_error() override;
    void update_progressbar_status(double persent) override;
    void show_file_success() override;
    void show_connection_aborted(const Address& address, Port port) override;

    void subscribe_listen(std::function<void()> func);
    void subscribe_transfer(std::function<void()> func);

private:
    void notify_listen();
    void notify_transfer();

    std::list<std::function<void()>> listen_subs_;
    std::list<std::function<void()>> transfer_subs_;

    bool is_running_ = true;
};

}
}
