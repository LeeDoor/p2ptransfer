#pragma once

#include "model_factory.hpp"
class ModelMockFactory : public ModelFactory {
public:
    std::shared_ptr<Listener> create_listener() override;
    std::shared_ptr<AddressGatherer> create_address_gatherer() override;
    std::shared_ptr<SocketManager> create_socket_manager(net::io_context& context) override;
    std::shared_ptr<ConnectionEstablisherImpl> create_connection_establisher(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<ConnectionStatusCallback> callback) override;
    std::shared_ptr<FileProcessor> create_file_processor(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<FileTransferCallback> callback) override;

    void set_listener(std::shared_ptr<Listener> value);
    void set_address_gatherer(std::shared_ptr<AddressGatherer> value);
    void set_socket_manager(std::shared_ptr<SocketManager> value);
    void set_connection_establisher(std::shared_ptr<ConnectionEstablisherImpl> value);
    void set_file_processor(std::shared_ptr<FileProcessor> value);
private:
    std::shared_ptr<Listener> listener_;
    std::shared_ptr<AddressGatherer> address_gatherer_;
    std::shared_ptr<SocketManager> socket_manager_;
    std::shared_ptr<ConnectionEstablisherImpl> connection_establisher_;
    std::shared_ptr<FileProcessor> file_processor_;
};
