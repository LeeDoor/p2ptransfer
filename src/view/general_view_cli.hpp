#pragma once
#include "general_view.hpp"
#include "cli_args_parser.hpp"
#include "endpoint.hpp"

namespace p2ptransfer {
namespace view {

/// \ref View implementation for Command Line Interface.
class GeneralViewCLI : public GeneralView {
public:
    GeneralViewCLI(int argc, char** argv);

    int run() override;
    void stop() override;
    void show_address(const Address& address) override;
    void show_connected(const Address& address, Port port) override;
    void show_socket_error(std::string reason) override;
    void update_progressbar_status(double persent, double kbps) override;
    void show_file_success() override;
    void show_transfer_failed(const Address& address, Port port, std::string reason) override;

    using ListenNotification = std::function<void(Port)>;
    using TransferNotification = std::function<void(const Address&, Port, const Filename&)>;
    using LookupStartNotification = std::function<Endpoint()>;
    void subscribe_listen(ListenNotification func);
    void subscribe_transfer(TransferNotification func);
    void subscribe_lookup(LookupStartNotification func);

private:
    void fill_endpoint_if_required();
    void run_action();
    void close_program();
    void notify_listen();
    void notify_transfer();

    LookupStartNotification lookup_start_invoker_ {};
    std::list<ListenNotification> listen_subs_ {};
    std::list<TransferNotification> transfer_subs_ {};

    CLIArgsParser::CLIRunArgs args_ {};
};

}
}
