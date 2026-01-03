#pragma once
#include "logger.hpp"
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
            using SocketType = net::basic_stream_socket<InternetProtocol>;
            using SocketDeleter = std::function<void(SocketType*)>;
            using SocketPtr = std::shared_ptr<SocketType>;
            using EndpointType = net::ip::basic_endpoint<InternetProtocol>;
            using AcceptorType = net::basic_socket_acceptor<InternetProtocol>;

            /// Object initialization with pre-established connection. Listens for incoming connection at port.
            /*! \throws std::runtime_error if connection failed. */
            static net::awaitable<std::shared_ptr<SocketManagerImpl>> open_for_listening(ContextPtr context, Port port) {
                auto sm = std::shared_ptr<SocketManagerImpl>(new SocketManagerImpl(context));
                co_await sm->listen_connection_at(port);
                co_return sm;
            }

            /// Object initialization with pre-established connection. Connecting to Endpoint.
            /*! \throws std::runtime_error if connection failed. */
            static net::awaitable<std::shared_ptr<SocketManagerImpl>> open_for_connecting(ContextPtr context, const Address& address, Port port) {
                auto sm = std::shared_ptr<SocketManagerImpl>(new SocketManagerImpl(context));
                co_await sm->connect_to(address, port);
                co_return sm;
            }

            ~SocketManagerImpl() {
                std::ignore = stop_socket();
            }

            [[nodiscard]] ErrorCode stop_socket() override {
                stopped_ = true;
                ErrorCode ec;
                if(socket_ && socket_->is_open()){
                    socket_->shutdown(SocketType::shutdown_both, ec);
                    socket_->cancel(ec);
                    socket_->close(ec);
                }
                return ec;
            }

            [[nodiscard]] bool connected() const noexcept override {
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

            net::awaitable<std::string> read_request() override {
                size_t bytes;
                auto dynamic_buffer = net::dynamic_buffer(reading_buffer_, MAX_SEND_REQUEST_SIZE);
                bytes = 
                    co_await net::async_read_until(*socket_, dynamic_buffer, 
                            REQUEST_COMPLETION, 
                            net::use_awaitable);
                if(stopped_) 
                    throw std::runtime_error("Trying to read request with stopped socket");
                std::string result = reading_buffer_.substr(0, bytes);
                reading_buffer_.erase(0, bytes);
                co_return result;
            }

            net::awaitable<void> write(std::string response) override {
                if(stopped_) 
                    throw std::runtime_error("Trying to write with stopped socket");
                co_await net::async_write(*socket_, 
                        net::buffer(response), 
                        net::use_awaitable);
            }

            net::awaitable<size_t> read_part_to(BufferType& buffer, size_t& bytes_remaining) override {
                size_t bytes_read = co_await net::async_read(
                        *socket_, 
                        net::buffer(buffer, std::min(BUFFER_SIZE, bytes_remaining)),
                        net::use_awaitable);
                if(stopped_) 
                    throw std::runtime_error("Trying to read request with stopped socket");
                bytes_remaining -= bytes_read;
                co_return bytes_read;
            }

            net::awaitable<size_t> write_part_from(WriteBufferType& buffer, size_t& bytes_remaining) override {
                size_t bytes_written = co_await net::async_write(
                        *socket_, 
                        net::buffer(buffer.get_data(), buffer.get_data_size()),
                        net::use_awaitable);
                if(stopped_) 
                    throw std::runtime_error("Trying to read request with stopped socket");
                bytes_remaining -= bytes_written;
                buffer.grab(bytes_written);
                co_return bytes_written;
            }

        protected:
            SocketManagerImpl(ContextPtr context) :
                context_(context){}

            /*! \throws std::runtime_error if connection failed */
            net::awaitable<void> listen_connection_at(Port port) {
                EndpointType endpoint(InternetProtocol::v4(), port);
                AcceptorType acceptor(*context_, endpoint);
                socket_ = SocketPtr(new SocketType(*context_));
                co_await acceptor.async_accept(*socket_, net::use_awaitable);
            }

            /*! \throws std::runtime_error if connection failed */
            net::awaitable<void> connect_to(const Address& address, Port port) {
                const EndpointType ep (net::ip::make_address(address), port);
                socket_ = SocketPtr(new SocketType(*context_, InternetProtocol::v4()));
                co_await socket_->async_connect(ep, net::use_awaitable);
            }

            ContextPtr context_;
            SocketPtr socket_;
            std::string reading_buffer_;
            bool stopped_ = false;
    };

using SocketManagerTcp = SocketManagerImpl<tcpip>;
using SocketManagerUdp = SocketManagerImpl<net::ip::udp>;

}
