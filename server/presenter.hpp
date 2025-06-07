#pragma once

#include "address.hpp"
#include "network_manager.hpp"
#include "port.hpp"
#include "view.hpp"
class Presenter {
public:
    Presenter(std::unique_ptr<IView> view);
    int run();
    // Model interface
    void connection_opened(const Address& address, Port port);
    void connected(const Address& address, Port port);
    void connection_aborted(const Address& address, Port port);
    // View interface
    void Listen();
private:
    std::unique_ptr<IView> view_;
    std::unique_ptr<NetworkManager> network_manager_;
};
