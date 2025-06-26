#pragma once

#include "address_gatherer.hpp"
#include "connection_establisher.hpp"
#include "file_processor.hpp"
#include "socket_manager.hpp"
#include "listener.hpp"
class ModelFactory {
public:
    virtual std::shared_ptr<Listener> create_listener() = 0;
    virtual std::shared_ptr<AddressGatherer> create_address_gatherer() = 0;
    virtual std::shared_ptr<SocketManager> create_socket_manager(net::io_context& context) = 0;
    virtual std::shared_ptr<ConnectionEstablisher> create_connection_establisher(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<ConnectionStatusCallback> callback) = 0;
    virtual std::shared_ptr<FileProcessor> create_file_processor(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<FileTransferCallback> callback) = 0;
};
