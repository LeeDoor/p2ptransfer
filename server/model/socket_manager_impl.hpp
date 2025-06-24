#pragma once
#include "common_types.hpp"
#include "socket_manager.hpp"

class SocketManagerImpl : public SocketManager {
public:
    SocketManagerImpl(net::io_context& context) :
        context_(context){}
    net::awaitable<void> establish_connection_async(Port port) override;
    SockPtr establish_connection_sync(Port port);
    RemoteEndpoint get_remote_endpoint() override;
 
    net::awaitable<std::string> read_request() override;
    net::awaitable<void> send_response(const std::string& response) override;
    net::awaitable<size_t> read_file_part_to(BufferType& buffer, size_t& bytes_remaining) override;
protected:
    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";
    net::io_context& context_;
    SockPtr socket_;
};
