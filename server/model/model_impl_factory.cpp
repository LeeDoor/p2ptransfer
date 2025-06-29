#include "model_impl_factory.hpp"
#include "address_gatherer_impl.hpp"
#include "file_processor_impl.hpp"
#include "listener_impl.hpp"
#include "socket_manager_impl_factory.hpp"
#include "thread_wrapper_impl.hpp"

std::shared_ptr<Listener> ModelImplFactory::create_listener() {
    return std::make_shared<ListenerImpl>(shared_from_this());
}
std::shared_ptr<AddressGatherer> ModelImplFactory::create_address_gatherer() {
    return std::make_shared<AddressGathererImpl>(shared_from_this());
}
std::shared_ptr<SocketManagerFactory> ModelImplFactory::create_socket_builder(net::io_context& context) {
    return std::make_shared<SocketManagerImplFactory>(context);
}
std::shared_ptr<FileProcessor> ModelImplFactory::create_file_processor(
    std::shared_ptr<SocketManager> socket_manager, 
    std::shared_ptr<FileTransferCallback> callback) {
    auto file_processor = std::make_shared<FileProcessorImpl>(socket_manager);
    file_processor->set_callback(callback);
    return file_processor;
}

std::shared_ptr<ThreadWrapper> ModelImplFactory::create_thread_wrapper() {
    return std::make_shared<ThreadWrapperImpl>();
}
