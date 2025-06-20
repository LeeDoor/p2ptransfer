#pragma once
#include "address.hpp"
#include "port.hpp"
#include "common_types.hpp"


class SocketManagerImpl {
public:
    struct RemoteEndpoint {
        Address address;
        Port port;
    };

    static constexpr size_t BUFFER_SIZE = 4096;
    using BufferType = std::array<char, BUFFER_SIZE>;

    SocketManagerImpl(net::io_context& context) :
        context_(context){}
    net::awaitable<void> open_connection_async(Port port);
    SockPtr open_connection_sync(Port port);
    RemoteEndpoint get_remote_endpoint();

    net::awaitable<std::string> read_request();
    net::awaitable<void> send_response(std::string&& response);
    net::awaitable<size_t> read_file_part_to(BufferType& buffer, size_t& bytes_remaining);
private:
    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";
    net::io_context& context_;
    SockPtr socket_;
};
