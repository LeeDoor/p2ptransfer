#pragma once
#include "address.hpp"
#include "port.hpp"

namespace general {

/*!
* \brief SocketManager interface for network communication.
*
* SocketManager provides Input/Output interface for network connection.
* This class doesn't provide any interface for opening/closing connection:
* preferred to use \ref SocketManagerBuilder for that purpose. 
* Socket destruction supposes the connection shutdown.
*/
class SocketManager {
public:
    struct Endpoint {
        Address address;
        Port port;
        bool operator==(const Endpoint& b) const = default;
    };
    static constexpr size_t BUFFER_SIZE = 4096;
    using BufferType = std::array<char, BUFFER_SIZE>;

    /// Defines if socket is connected already. Shouldn't ping for connection.
    virtual bool connected() const noexcept = 0;
    /// \throws std::logic_error if forgot to establish connection before use.
    virtual Endpoint get_remote_endpoint() const = 0;
    /// \throws std::logic_error if forgot to establish connection before use.
    virtual Endpoint get_local_endpoint() const = 0;
    /// Reads request from network connection. Request is "\n\n"-terminated string.
    // \throws std::runtime_error if connection aborted.
    virtual net::awaitable<std::string> read_request() = 0;
    /// Writes data to network connection. User should check the "\n\n" termination if request sent.
    /// \throws std::runtime_error if connection aborted.
    virtual net::awaitable<void> write(std::string response) = 0;
    /// Reads a part of required file.
    /*!
    * \param buffer gathered data from remote connection is written to buffer.
    * \param bytes_remaining function substracts readed bytes from bytes_remaining.
    * Guaranteed that bytes_remaining == 0 if file is fully gathered.
    * \returns the amount of bytes of the gathered file part.
    * \throws std::runtime_error if connection aborted.
    */
    virtual net::awaitable<size_t> read_part_to(BufferType& buffer, size_t& bytes_remaining) = 0;
    
    virtual ~SocketManager() = default;

    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";
};

}
