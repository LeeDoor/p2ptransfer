#include "broadcast_socket_manager_impl.hpp"

namespace p2ptransfer {

BroadcastSocketManagerImpl::BroadcastSocketManagerImpl(ContextPtr context)
: context_{ context }
, socket_{ SocketPtr(new SocketType(*context_), get_socket_deleter()) }
{
    socket_->open(net::ip::udp::v4());
    socket_->set_option(net::socket_base::broadcast(true));
}

net::awaitable<void> BroadcastSocketManagerImpl::broadcast_send(Port port, std::string message) {
    try {
        net::ip::udp::endpoint broadc_endp{ net::ip::address_v4::broadcast(), static_cast<net::ip::port_type>(port) };
        co_await socket_->async_send_to(net::buffer(message), broadc_endp, net::use_awaitable);
    } catch (const boost::system::system_error& ex) {
        throw std::runtime_error(ex.code().message());
    }
}
net::awaitable<BroadcastSocketManager::BroadcastResponse> BroadcastSocketManagerImpl::receive() {
    try {
        std::string reading_buffer_;
        net::ip::udp::endpoint remote_endpoint;
        size_t bytes_read = co_await socket_->async_receive_from(reading_buffer_, remote_endpoint, net::use_awaitable);
        co_return BroadcastResponse{ 
            reading_buffer_.substr(0, bytes_read),
            remote_endpoint.address().to_string(),
            remote_endpoint.port()
        };
    } catch (const boost::system::system_error& ex) {
        throw std::runtime_error(ex.code().message());
    }
}

void BroadcastSocketManagerImpl::stop() {
    ErrorCode ec;
    std::ignore = socket_->shutdown(SocketType::shutdown_both, ec);
    std::ignore = socket_->close(ec);
}

}
