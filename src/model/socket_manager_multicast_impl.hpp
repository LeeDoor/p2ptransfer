#pragma once
#include "socket_manager_multicast.hpp"

namespace p2ptransfer {

class SocketManagerMulticastImpl : public SocketManagerMulticast {
public:
    using SocketType = net::ip::udp::socket;
    using SocketDeleter = std::function<void(SocketType*)>;
    using SocketPtr = std::unique_ptr<SocketType, SocketDeleter>;
    using EndpointType = net::ip::udp::endpoint;

    SocketManagerMulticastImpl(ContextPtr context);

    net::awaitable<void> send(std::string message, Address address, Port port) override;
    net::awaitable<MulticastResponse> receive(Address address, Port port) override;

    void stop() override;

private:
    void prepare_for_receiving(Address address, Port port);

    SocketDeleter get_socket_deleter() {
        return [] (SocketType* socket) {
            ErrorCode ec;
            std::ignore = socket->shutdown(SocketType::shutdown_both, ec);
            socket->close();
            delete socket;
        };
    }

    ContextPtr context_;
    SocketPtr socket_;
    bool is_bound_to_local_address_{ false };
};

}
