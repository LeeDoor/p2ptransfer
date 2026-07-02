#pragma once
#include "broadcast_socket_manager.hpp"

namespace p2ptransfer {

class BroadcastSocketManagerImpl : public BroadcastSocketManager {
public:
    using SocketType = net::ip::udp::socket;
    using SocketDeleter = std::function<void(SocketType*)>;
    using SocketPtr = std::unique_ptr<SocketType, SocketDeleter>;

    BroadcastSocketManagerImpl(ContextPtr context);

    net::awaitable<void> broadcast_send(Port port, std::string message) override;
    net::awaitable<BroadcastResponse> receive() override;

    void stop() override;

private:
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
};

}
