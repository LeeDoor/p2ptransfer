#include "socket_manager_multicast_impl.hpp"

namespace p2ptransfer {

SocketManagerMulticastImpl::SocketManagerMulticastImpl(ContextPtr context)
: context_{ context }
, socket_{ SocketPtr(new SocketType(*context_), get_socket_deleter()) }
{
    socket_->open(net::ip::udp::v4());
    socket_->set_option(net::socket_base::reuse_address(true));
    socket_->set_option(net::ip::multicast::enable_loopback(true));
    socket_->set_option(net::ip::multicast::hops(16));
}

net::awaitable<void> SocketManagerMulticastImpl::send(std::string message, Address address, Port port) {
    try {
        EndpointType multicast_group_endpoint (net::ip::make_address(address), port);
        co_await socket_->async_send_to(net::buffer(message), multicast_group_endpoint, net::use_awaitable);
    } catch (const boost::system::system_error& ex) {
        throw std::runtime_error(ex.code().message());
    }
}
net::awaitable<SocketManagerMulticast::MulticastResponse> SocketManagerMulticastImpl::receive(Address address, Port port) {
    try {
        prepare_for_receiving(address, port);
        std::string reading_buffer_;
        net::ip::udp::endpoint remote_endpoint;
        size_t bytes_read = 
            co_await socket_->async_receive_from(
                net::buffer(reading_buffer_), 
                remote_endpoint, 
                net::use_awaitable);
        co_return MulticastResponse{ 
            reading_buffer_.substr(0, bytes_read),
            remote_endpoint.address().to_string(),
            remote_endpoint.port()
        };
    } catch (const boost::system::system_error& ex) {
        throw std::runtime_error(ex.code().message());
    }
}

void SocketManagerMulticastImpl::stop() {
    ErrorCode ec;
    std::ignore = socket_->shutdown(SocketType::shutdown_both, ec);
    std::ignore = socket_->close(ec);
}

void SocketManagerMulticastImpl::prepare_for_receiving(Address address, Port port) {
    if(is_bound_to_local_address_) 
        return;
    socket_->bind({ net::ip::udp::v4(), static_cast<net::ip::port_type>(port) });
    socket_->set_option(net::ip::multicast::join_group(net::ip::make_address(address)));
    is_bound_to_local_address_ = true;
}

}
