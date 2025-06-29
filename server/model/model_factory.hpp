#pragma once

#include "address_gatherer.hpp"
#include "file_processor.hpp"
#include "socket_manager.hpp"
#include "listener.hpp"
#include "thread_wrapper.hpp"

class SocketManagerFactory {
public:
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) = 0;
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) = 0;
};
class ModelFactory {
public:
    virtual std::shared_ptr<Listener> create_listener() = 0;
    virtual std::shared_ptr<AddressGatherer> create_address_gatherer() = 0;
    virtual std::shared_ptr<SocketManagerFactory> create_socket_builder(net::io_context& context) = 0;
    virtual std::shared_ptr<FileProcessor> create_file_processor(
        std::shared_ptr<SocketManager> socket_manager, 
        std::shared_ptr<FileTransferCallback> callback) = 0;
    virtual std::shared_ptr<ThreadWrapper> create_thread_wrapper() = 0;
};
