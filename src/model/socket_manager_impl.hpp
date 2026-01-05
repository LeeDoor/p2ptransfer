#pragma once
#include "socket_manager.hpp"

namespace p2ptransfer {

/*!
 * \brief Implementation class for \ref SocketManager
 *
 * Implements \ref SocketManager interface for network communication.
 * Constructor is hidden to force class users firstly establish connection with
 * special static member functions open_for_listening or open_for_connecting.
 * Has two prepared implementations for boost::asio::ip::tcp and boost::asio::ip::udp:
 * SocketManagerTcp and SocketManagerUdp.
 * \tparam InternetProtocol means either TCP or UDP connection is used.
 */
template<typename InternetProtocol>
class SocketManagerImpl : public SocketManager {
public:
    using InternetProtocolType = InternetProtocol;
    using SocketType = net::basic_stream_socket<InternetProtocolType>;
    using SocketDeleter = std::function<void(SocketType*)>;
    using SocketPtr = std::unique_ptr<SocketType, SocketDeleter>;
    using EndpointType = net::ip::basic_endpoint<InternetProtocolType>;
    using AcceptorType = net::basic_socket_acceptor<InternetProtocolType>;

    /// Object initialization with pre-established connection. Listens for incoming connection at port.
    /*! \throws std::runtime_error if connection failed. */
    static net::awaitable<std::shared_ptr<SocketManagerImpl>> open_for_listening(ContextPtr context, AcceptorType& acceptor) {
        auto sm = std::shared_ptr<SocketManagerImpl>(new SocketManagerImpl(context));
        co_await sm->listen_connection_at(acceptor);
        co_return sm;
    }

    /// Object initialization with pre-established connection. Connecting to Endpoint.
    /*! \throws std::runtime_error if connection failed. */
    static net::awaitable<std::shared_ptr<SocketManagerImpl>> open_for_connecting(ContextPtr context, const EndpointType& endpoint) {
        auto sm = std::shared_ptr<SocketManagerImpl>(new SocketManagerImpl(context));
        co_await sm->connect_to(endpoint);
        co_return sm;
    }

    bool connected() const noexcept override {
        return socket_ != nullptr;
    }
    Endpoint get_remote_endpoint() const override {
        if(!connected()) 
            throw std::logic_error("get_remote_endpoint called while socket is nullptr. "
                                   "Connect first");
        return {
            socket_->remote_endpoint().address().to_string(), 
            socket_->remote_endpoint().port()
        };
    }

    Endpoint get_local_endpoint() const override {
        if(!connected()) 
            throw std::logic_error("get_local_endpoint called while socket is nullptr. "
                                   "Connect first");
        return {
            socket_->local_endpoint().address().to_string(), 
            socket_->local_endpoint().port()
        };
    }
    /// Stops running socket from any network operations.
    /// To stop socket at the creation process use the builder instead.
    void stop() override {
        ErrorCode ec;
        socket_->shutdown(SocketType::shutdown_both, ec);
        socket_->close(ec);
    }

    net::awaitable<std::string> read_request() override {
        size_t bytes;
        auto dynamic_buffer = net::dynamic_buffer(reading_buffer_, MAX_SEND_REQUEST_SIZE);
        bytes = 
            co_await net::async_read_until(*socket_, dynamic_buffer, 
                                           REQUEST_COMPLETION, 
                                           net::use_awaitable);
        std::string result = reading_buffer_.substr(0, bytes);
        reading_buffer_.erase(0, bytes);
        co_return result;
    }

    net::awaitable<void> write(std::string response) override {
        co_await net::async_write(*socket_, 
                                  net::buffer(response), 
                                  net::use_awaitable);
    }

    net::awaitable<size_t> read_part_to(BufferType& buffer, size_t& bytes_remaining) override {
        size_t bytes_read = co_await net::async_read(
            *socket_, 
            net::buffer(buffer, std::min(BUFFER_SIZE, bytes_remaining)),
            net::use_awaitable);
        bytes_remaining -= bytes_read;
        co_return bytes_read;
    }

    net::awaitable<size_t> write_part_from(WriteBufferType& buffer, size_t& bytes_remaining) override {
        size_t bytes_written = co_await net::async_write(
            *socket_, 
            net::buffer(buffer.get_data(), buffer.get_data_size()),
            net::use_awaitable);
        bytes_remaining -= bytes_written;
        buffer.grab(bytes_written);
        co_return bytes_written;
    }

protected:
    SocketManagerImpl(ContextPtr context) :
        context_{context} {}

    SocketDeleter get_socket_deleter() {
        return [] (SocketType* socket) {
            ErrorCode ec;
            socket->shutdown(SocketType::shutdown_both, ec);
            socket->close();
            delete socket;
        };
    }

    /*! \throws std::runtime_error if connection failed */
    net::awaitable<void> listen_connection_at(AcceptorType& acceptor) {
        socket_ = SocketPtr(new SocketType(*context_), get_socket_deleter());
        co_await acceptor.async_accept(*socket_, net::use_awaitable);
    }

    /*! \throws std::runtime_error if connection failed */
    net::awaitable<void> connect_to(const EndpointType& endpoint) {
        socket_ = SocketPtr(new SocketType(*context_, InternetProtocolType::v4()), get_socket_deleter());
        co_await socket_->async_connect(endpoint, net::use_awaitable);
    }

    ContextPtr context_;
    SocketPtr socket_;
    std::string reading_buffer_;
};

using SocketManagerTcp = SocketManagerImpl<tcpip>;
using SocketManagerUdp = SocketManagerImpl<net::ip::udp>;

}
