#include "socket_manager_impl.hpp"

SockPtr SocketManagerImpl::open_connection_sync(Port port) {
    SockPtr sock_;
    co_spawn(context_, open_connection_async(port), [&](std::exception_ptr a){});
    context_.run();
    return sock_;
}
net::awaitable<void> SocketManagerImpl::open_connection_async(Port port) {
    tcpip::endpoint endpoint(tcpip::v4(), port);
    SocketCloser socketCloser = [] (tcpip::socket* socket) {
        ErrorCode ec;
        socket->shutdown(tcpip::socket::shutdown_both, ec);
        socket->close();
    };
    tcpip::acceptor acceptor(context_, endpoint);
    socket_ = {new tcpip::socket(context_), socketCloser};
    co_await acceptor.async_accept(*socket_, net::use_awaitable);
}
SocketManagerImpl::RemoteEndpoint SocketManagerImpl::get_remote_endpoint() {
    if(socket_ == nullptr) 
        throw std::logic_error("get_remote_endpoint while socket is nullptr is illegal");
    return {
        socket_->remote_endpoint().address().to_string(), 
        socket_->remote_endpoint().port()
    };
}
net::awaitable<std::string> SocketManagerImpl::read_request() {
    std::string buffer;
    size_t bytes;
    auto dynamic_buffer = net::dynamic_buffer(buffer, MAX_SEND_REQUEST_SIZE);
    bytes = 
        co_await net::async_read_until(*socket_, dynamic_buffer, 
                                       REQUEST_COMPLETION, 
                                       net::use_awaitable);
    co_return buffer.substr(0, bytes);
}
net::awaitable<void> SocketManagerImpl::send_response(std::string&& response) {
    size_t bytes;
    co_await net::async_write(*socket_, 
                              net::buffer(response), 
                              net::use_awaitable);
}
net::awaitable<size_t> SocketManagerImpl::read_file_part_to(BufferType& buffer, size_t& bytes_remaining) {
    size_t bytes; ErrorCode ec;
    std::tie(ec, bytes) 
        = co_await net::async_read(*socket_, 
                                   net::buffer(buffer, std::min(BUFFER_SIZE, bytes_remaining)),
                                   net::as_tuple(net::use_awaitable));
    bytes_remaining -= bytes;
    if(ec && bytes_remaining) {
        throw std::runtime_error("failed to read file: " + ec.what());
    }
    co_return bytes;
}
