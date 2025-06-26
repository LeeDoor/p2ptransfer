#pragma once

#include "model_factory.hpp"
class ModelImplFactory : public ModelFactory, public std::enable_shared_from_this<ModelImplFactory> {
public:
    std::shared_ptr<Listener> create_listener() override;
    std::shared_ptr<AddressGatherer> create_address_gatherer() override;
    std::shared_ptr<SocketManager> create_socket_manager(net::io_context& context) override;
    std::shared_ptr<ConnectionEstablisher> create_connection_establisher(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<ConnectionStatusCallback> callback) override;
    std::shared_ptr<FileProcessor> create_file_processor(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<FileTransferCallback> callback) override;
    std::shared_ptr<ThreadWrapper> create_thread_wrapper() override;
};
