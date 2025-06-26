#include "model_impl_factory.hpp"
#include "address_gatherer_impl.hpp"
#include "listener_impl.hpp"
#include "socket_manager_impl.hpp"

std::shared_ptr<Listener> ModelImplFactory::create_listener() {
    return std::make_shared<ListenerImpl>(shared_from_this());
}
std::shared_ptr<AddressGatherer> ModelImplFactory::create_address_gatherer() {
    return std::make_shared<AddressGathererImpl>();
}
std::shared_ptr<SocketManager> ModelImplFactory::create_socket_manager(net::io_context& context) {
    return std::make_shared<SocketManagerImpl>(context);
}
std::shared_ptr<ConnectionEstablisherImpl> ModelImplFactory::create_connection_establisher(
    std::shared_ptr<SocketManager> socket_manager, 
    std::shared_ptr<ConnectionStatusCallback> callback) {
    auto connection_establisher = std::make_shared<ConnectionEstablisherImpl>(socket_manager);
    connection_establisher->set_callback(callback);
    return connection_establisher;
}
std::shared_ptr<FileProcessorImpl> ModelImplFactory::create_file_processor(
    std::shared_ptr<SocketManager> socket_manager, 
    std::shared_ptr<FileTransferCallback> callback) {
    auto file_processor = std::make_shared<FileProcessorImpl>(socket_manager);
    file_processor->set_callback(callback);
    return file_processor;
}
