#include "address_gatherer_impl.hpp"

AddressGathererImpl::AddressGathererImpl(std::shared_ptr<ModelFactory> factory) :
    factory_(factory),
    thread_wrapper_(factory->create_thread_wrapper())
{}

void AddressGathererImpl::gather_local_address() {
    if(thread_wrapper_->is_running()) 
        throw std::logic_error("gathering local address twice");
    auto rethrow_functor = [](std::exception_ptr ptr) {
        if(ptr) std::rethrow_exception(ptr);
    };
    net::co_spawn(context_, gather_async(), rethrow_functor);
    thread_wrapper_->execute([this] {
        context_.run();
        context_.restart();
    });
}

net::awaitable<void> AddressGathererImpl::gather_async() {
    auto socket_manager = factory_->create_socket_manager_udp(context_);
    try {
        co_await socket_manager->connect_to("192.168.0.1", 8080);
        callback()->set_address(socket_manager->get_local_endpoint().address);
    } catch (const std::exception& ex) {
        callback()->set_address(ex.what());
    }
}

