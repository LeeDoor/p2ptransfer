#pragma once
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {

template<typename T>
concept Readable = requires(T& obj, char* buffer, std::size_t size) {
{ obj.read(buffer, size) };
};
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
    class WriteBufferType {
        BufferType value_;
        size_t bytes_remaining_;
        size_t bytes_shift_;
    public:
        WriteBufferType() : value_{}, bytes_remaining_{value_.size()}, bytes_shift_{0} {}

        template<Readable Stream>
        void fill(Stream& ifs) { 
            ifs.read(value_.data(), value_.size());
            if (ifs.eof()) bytes_remaining_ = ifs.gcount();
            else bytes_remaining_ = value_.size();
            bytes_shift_ = 0;
        }
        size_t remaining() const { return bytes_remaining_; }
        void grab(size_t grabbed) { 
            bytes_remaining_ -= grabbed;
            bytes_shift_ += grabbed;
        }
        bool completed() const { return bytes_remaining_ == 0; }
        char* get_data() { return value_.data() + bytes_shift_; }
        size_t get_data_size() { return bytes_remaining_; }
    };

    virtual ~SocketManager() = default;

    /// Makes this socket stop reading and writing loop and shutdown.
    virtual ErrorCode stop_socket() = 0;
/// Defines if socket is connected already. Shouldn't ping for connection.
    virtual bool connected() const noexcept = 0;
/*! \throws std::logic_error if forgot to establish connection before use. */
    virtual Endpoint get_remote_endpoint() const = 0;
/*! \throws std::logic_error if forgot to establish connection before use. */
    virtual Endpoint get_local_endpoint() const = 0;
    /// Stops current execution of network process.
    virtual void stop() = 0;
/// Reads request from network connection. Request is "\n\n"-terminated string.
/*! \throws std::runtime_error if connection aborted. */
    virtual net::awaitable<std::string> read_request() = 0;
/// Writes data to network connection. User should check the "\n\n" termination if request sent.
/*! \throws std::runtime_error if connection aborted. */
    virtual net::awaitable<void> write(std::string data) = 0;
/// Reads a part of required file.
/*!
    * \param buffer gathered data from remote connection is written to buffer.
    * \param bytes_remaining function substracts readed bytes from bytes_remaining.
    * Guaranteed that bytes_remaining == 0 if file is fully gathered.
    * \returns the amount of bytes of the gathered file part.
    * \throws std::runtime_error if connection aborted.
    */
    virtual net::awaitable<size_t> read_part_to(BufferType& buffer, size_t& bytes_remaining) = 0;
/// Writes a part of required file.
/*!
    * \param buffer buffer with data which should be send to the remote user.
    * \param bytes_remaining function substracts readed bytes from bytes_remaining.
    * Guaranteed that bytes_remaining == 0 if file is fully sent.
    * \returns the amount of bytes of the sent file part.
    * \throws std::runtime_error if connection aborted.
    */
    virtual net::awaitable<size_t> write_part_from(WriteBufferType& buffer, size_t& bytes_remaining) = 0;

    constexpr static size_t MAX_SEND_REQUEST_SIZE = 512;
    constexpr static std::string_view REQUEST_COMPLETION = "\n\n";
};

}
