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
std::shared_ptr<SocketManagerFactory> ModelMockFactory::create_socket_builder([[maybe_unused]] net::io_context& context) {
    if(!socket_manager_factory_)
        throw std::runtime_error("Socket manager is not set as a mock");
    return socket_manager_factory_;
}
std::shared_ptr<FileProcessor> ModelMockFactory::create_file_processor(
    [[maybe_unused]] std::shared_ptr<SocketManager> socket_manager, 
    [[maybe_unused]] std::shared_ptr<FileTransferCallback> callback) {
    if(!file_processor_)
        throw std::runtime_error("File Processor is not set as a mock");
    return file_processor_;
}
std::shared_ptr<ThreadWrapper> ModelMockFactory::create_thread_wrapper() {
    if(!thread_wrapper_)
        throw std::runtime_error("ThreadWrapper is not set as a mock");
    return thread_wrapper_;
}

void ModelMockFactory::set_listener(std::shared_ptr<Listener> value) {
    listener_ = value;
}
void ModelMockFactory::set_address_gatherer(std::shared_ptr<AddressGatherer> value) {
    address_gatherer_ = value;
}
void ModelMockFactory::set_socket_builder(std::shared_ptr<SocketManagerFactory> value) {
    socket_manager_factory_ = value;
}
void ModelMockFactory::set_file_processor(std::shared_ptr<FileProcessor> value) {
    file_processor_ = value;
}
void ModelMockFactory::set_thread_wrapper(std::shared_ptr<ThreadWrapper> value) {
    thread_wrapper_ = value;
}
