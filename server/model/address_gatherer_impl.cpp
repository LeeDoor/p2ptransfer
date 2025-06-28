#include "address_gatherer_impl.hpp"

AddressGathererImpl::AddressGathererImpl(std::shared_ptr<ThreadWrapper> thread_wrapper) :
    thread_wrapper_(thread_wrapper)
{}

void AddressGathererImpl::gather_local_address() {
    using udp = net::ip::udp;
    if(thread_wrapper_->is_running()) 
        throw std::logic_error("gathering local address twice");
    thread_wrapper_->execute([this] {
        const udp::endpoint ep (net::ip::make_address("192.168.0.1"), 8080);
        udp::socket socket(context_, udp::endpoint(udp::v4(), 8081));
        ErrorCode ec;
        socket.connect(ep, ec);
        if(ec) {
            callback()->set_address("Unable to gather LAN address");
        } else {
            callback()->set_address(socket.local_endpoint().address().to_string());
        }
    });
}

