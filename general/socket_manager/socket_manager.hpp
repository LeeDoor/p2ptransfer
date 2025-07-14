#pragma once
#include "address.hpp"
#include "port.hpp"

namespace general {
namespace socket_manager {

class SocketManager {
public:
    struct Endpoint {
        Address address;
        Port port;
        bool operator==(const Endpoint& b) const = default;
    };
    static constexpr size_t BUFFER_SIZE = 4096;
    using BufferType = std::array<char, BUFFER_SIZE>;

    virtual bool connected() = 0;
    virtual Endpoint get_remote_endpoint() = 0;
    virtual Endpoint get_local_endpoint() = 0;
    virtual net::awaitable<std::string> read_request() = 0;
    virtual net::awaitable<void> write(std::string response) = 0;
    virtual net::awaitable<size_t> read_part_to(BufferType& buffer, size_t& bytes_remaining) = 0;
    
    virtual ~SocketManager() = default;

    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";
};

}
}
