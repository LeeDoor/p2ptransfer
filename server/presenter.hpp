#pragma once

#include "address.hpp"
#include "address_gatherer.hpp"
#include "network_manager.hpp"
#include "port.hpp"
#include "request_structures.hpp"
#include "view.hpp"
class Presenter : public std::enable_shared_from_this<Presenter> {
public:
    Presenter(std::shared_ptr<IView> view);
    int run();
    // Model interface
    void connection_opened(const Address& address, Port port);
    void connected(const Address& address, Port port);
    void connection_aborted(const Address& address, Port port);
    void file_transfered();
    void set_progressbar_status(double present);
    void set_address(const Address& address);
    bool verify_file(SendRequest send_request);
    void cant_open_socket();
    // View interface
    void listen();
private:
    std::shared_ptr<IView> view_;
    std::shared_ptr<NetworkManager> network_manager_;
    std::shared_ptr<AddressGatherer> address_gatherer_;
};
