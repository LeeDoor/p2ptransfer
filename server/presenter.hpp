#pragma once

#include "address.hpp"
#include "model_address_gather.hpp"
#include "model_callback.hpp"
#include "model_network_manager.hpp"
#include "port.hpp"
#include "request_structures.hpp"
#include "view.hpp"
#include "view_callback.hpp"
class Presenter : public IModelCallback, public IViewCallback, public std::enable_shared_from_this<Presenter> {
public:
    Presenter(std::shared_ptr<IView> view, 
              std::shared_ptr<IModelNetworkManager> connection,
              std::shared_ptr<IModelAddressGatherer> address_gather);
    int run();
    // Model interface
    void connection_opened(const Address& address, Port port) override;
    void connected(const Address& address, Port port) override;
    void file_transfered() override;
    void connection_aborted(const Address& address, Port port) override;
    void cant_open_socket() override;
    void set_address(const Address& address) override;
    void set_progressbar(double present) override;
    bool verify_file(SendRequest send_request) override;
    // View interface
    void listen() override;
private:
    std::shared_ptr<IView> view_;
    std::shared_ptr<IModelNetworkManager> network_manager_;
    std::shared_ptr<IModelAddressGatherer> address_gatherer_;
};
