#pragma once
#include "common_types.hpp"
#include "socket_manager.hpp"

template<typename InternetProtocol>
class SocketManagerImpl : public SocketManager {
public:
    using SockType = net::basic_stream_socket<InternetProtocol>;
    using SockDeleter = std::function<void(SockType*)>;
    using SockPtr = std::unique_ptr<SockType, SockDeleter>;
    using EndpointType = net::ip::basic_endpoint<InternetProtocol>;
    using AcceptorType = net::basic_socket_acceptor<InternetProtocol>;

    SocketManagerImpl(net::io_context& context) :
        context_(context){}
    
    net::awaitable<void> listen_connection_at(Port port) override {
        EndpointType endpoint(InternetProtocol::v4(), port);
        auto socketCloser = [] (SockType* socket) {
            ErrorCode ec;
            socket->shutdown(SockType::shutdown_both, ec);
            socket->close();
        };
        AcceptorType acceptor(context_, endpoint);
        socket_ = SockPtr(new SockType(context_), socketCloser);
        co_await acceptor.async_accept(*socket_, net::use_awaitable);
    }

    net::awaitable<void> connect_to(const Address& address, Port port) override {
        const EndpointType ep (net::ip::make_address(address), port);
        auto socketCloser = [] (SockType* socket) {
            ErrorCode ec;
            socket->shutdown(SockType::shutdown_both, ec);
            socket->close();
        };
        socket_ = SockPtr(new SockType(context_, InternetProtocol::v4()), socketCloser);
        co_await socket_->async_connect(ep, net::use_awaitable);
    }
    
    SockPtr listen_connection_at_sync(Port port) {
        SockPtr sock;
        co_spawn(context_, listen_connection_at(port), [&](std::exception_ptr a){});
        context_.run();
        return sock;
    }
    
    Endpoint get_remote_endpoint() override{
        if(socket_ == nullptr) 
            throw std::logic_error("get_remote_endpoint called while socket is nullptr. "
                                   "Use open_connection first");
        return {
            socket_->remote_endpoint().address().to_string(), 
            socket_->remote_endpoint().port()
        };
    }
    
    Endpoint get_local_endpoint() override {
        if(socket_ == nullptr) 
            throw std::logic_error("get_local_endpoint called while socket is nullptr. "
                                   "Use open_connection first");
        return {
            socket_->local_endpoint().address().to_string(), 
            socket_->local_endpoint().port()
        };
    }

    net::awaitable<std::string> read_request() override {
        std::string buffer;
        size_t bytes;
        auto dynamic_buffer = net::dynamic_buffer(buffer, MAX_SEND_REQUEST_SIZE);
        bytes = 
            co_await net::async_read_until(*socket_, dynamic_buffer, 
                                           REQUEST_COMPLETION, 
                                           net::use_awaitable);
        co_return buffer.substr(0, bytes);
    }

    net::awaitable<void> send_response(const std::string& response) override {
        size_t bytes;
        co_await net::async_write(*socket_, 
                                  net::buffer(response), 
                                  net::use_awaitable);
    }

    net::awaitable<size_t> read_file_part_to(BufferType& buffer, size_t& bytes_remaining) override {
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

protected:
    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";
    net::io_context& context_;
    SockPtr socket_;
};

using SocketManagerTcp = SocketManagerImpl<tcpip>;
using SocketManagerUdp = SocketManagerImpl<net::ip::udp>;
