#include "address_gatherer_impl.hpp"

AddressGathererImpl::AddressGathererImpl(
    std::shared_ptr<net::io_context> context,
    std::shared_ptr<ThreadWrapper> thread_wrapper,
    std::shared_ptr<SocketManagerBuilder> socket_manager_builder) :

    socket_builder_(socket_manager_builder),
    thread_wrapper_(thread_wrapper),
    context_(context)
{}

void AddressGathererImpl::gather_local_address() {
    if(thread_wrapper_->is_running()) 
        throw std::logic_error("gathering local address twice");
    net::co_spawn(*context_, gather_async(), net::detached);
    thread_wrapper_->execute([this] {
        context_->run();
        context_->restart();
    });
}

net::awaitable<void> AddressGathererImpl::gather_async() {
    try {
        auto socket_manager = co_await build_socket_manager("192.168.0.1", 8080);
        callback()->set_address(socket_manager->get_local_endpoint().address);
    } catch (const std::exception& ex) {
        callback()->set_address(ex.what());
    }
}

net::awaitable<std::shared_ptr<SocketManager>> AddressGathererImpl::build_socket_manager(const Address& address, Port port) {
    co_return co_await socket_builder_->udp_connecting_to(address, port);
}

void AddressGathererImpl::stop() {
    context_->stop();
    thread_wrapper_->join();
}
