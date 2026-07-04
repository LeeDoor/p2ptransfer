#include "socket_manager_multicast_impl.hpp"

namespace p2ptransfer {

SocketManagerMulticastImpl::SocketManagerMulticastImpl(ContextPtr context, const Address& address, Port port) 
: context_{ context }
, socket_{ SocketPtr(new SocketType(*context_), get_socket_deleter()) }
, multicast_group_endpoint_(net::ip::make_address(address), port)
{
    socket_->open(net::ip::udp::v4());
    socket_->set_option(net::socket_base::reuse_address(true));
    socket_->set_option(net::ip::multicast::enable_loopback(true));
    socket_->set_option(net::ip::multicast::hops(16));
    socket_->set_option(net::ip::multicast::join_group(net::ip::make_address(address)));
    socket_->bind(multicast_group_endpoint_);
}

net::awaitable<void> SocketManagerMulticastImpl::send(std::string message) {
    try {
        co_await socket_->async_send_to(
            net::buffer(message),
            multicast_group_endpoint_,
            net::use_awaitable);
    } catch (const boost::system::system_error& ex) {
        throw std::runtime_error(ex.code().message());
    }
}
net::awaitable<SocketManagerMulticast::MulticastResponse> SocketManagerMulticastImpl::receive() {
    try {
        std::string reading_buffer;
        net::ip::udp::endpoint remote_endpoint;
        size_t bytes_read = 
            co_await socket_->async_receive_from(
                net::buffer(reading_buffer), 
                remote_endpoint, 
                net::use_awaitable);
        co_return MulticastResponse{ 
            reading_buffer.substr(0, bytes_read),
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

}
