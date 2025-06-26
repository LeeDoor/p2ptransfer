#include "model_mock_factory.hpp"

std::shared_ptr<Listener> ModelMockFactory::create_listener() {
    if(!listener_)
        throw std::runtime_error("Listener is not set as a mock");
    return listener_;
}
std::shared_ptr<AddressGatherer> ModelMockFactory::create_address_gatherer() {
    if(!address_gatherer_)
        throw std::runtime_error("Address Gatherer is not set as a mock");
    return address_gatherer_;
}
std::shared_ptr<SocketManager> ModelMockFactory::create_socket_manager(net::io_context& context) {
    if(!socket_manager_)
        throw std::runtime_error("Socket manager is not set as a mock");
    return socket_manager_;
}
std::shared_ptr<ConnectionEstablisher> ModelMockFactory::create_connection_establisher(
    std::shared_ptr<SocketManager> socket_manager, 
    std::shared_ptr<ConnectionStatusCallback> callback) {
    if(!connection_establisher_)
        throw std::runtime_error("Connection Establisher is not set as a mock");
    return connection_establisher_;
}
std::shared_ptr<FileProcessor> ModelMockFactory::create_file_processor(
    std::shared_ptr<SocketManager> socket_manager, 
    std::shared_ptr<FileTransferCallback> callback) {
    if(!file_processor_)
        throw std::runtime_error("File Processor is not set as a mock");
    return file_processor_;
}
void ModelMockFactory::set_listener(std::shared_ptr<Listener> value) {
    listener_ = value;
}
void ModelMockFactory::set_address_gatherer(std::shared_ptr<AddressGatherer> value) {
    address_gatherer_ = value;
}
void ModelMockFactory::set_socket_manager(std::shared_ptr<SocketManager> value) {
    socket_manager_ = value;
}
void ModelMockFactory::set_connection_establisher(std::shared_ptr<ConnectionEstablisher> value) {
    connection_establisher_ = value;
}
void ModelMockFactory::set_file_processor(std::shared_ptr<FileProcessor> value) {
    file_processor_ = value;
}
