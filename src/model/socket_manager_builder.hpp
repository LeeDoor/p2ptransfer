#pragma once
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {

class SocketManager;

/*!
* \brief socket builder for immediate connection
*
* Using socket builder prevents class user from accessing
* socket before connection. Takes over the responsibility
* to ensure the returned socket connection is established.
*/
class SocketManagerBuilder {
public:
    virtual ~SocketManagerBuilder() = default;

    /*!
    * \brief listening for connection via TCP protocol at given port.
    * \returns awaitable object with pointer to TCP socket manager.
    * \throws std::runtime_error's boost overloads on opening or connection failures.
    */
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_listening_at(Port port) = 0;
    /*!
    * \brief listening for UDP connection at given port.
    * \returns awaitable pointer to UDP socket manager.
    * \throws std::runtime_error if port opening failed or connection not established.
    */
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_listening_at(Port port) = 0;
    /*!
    * \brief connecting to given Endpoint with TCP protocol.
    * \returns awaitable pointer to TCP socket manager.
    * \throws std::runtime_error if connection rejected or any other connection error occured.
    */
    virtual net::awaitable<std::shared_ptr<SocketManager>> tcp_connecting_to(const Address& address, Port port) = 0;
    /*!
    * \brief connecting to given Endpoint with UDP protocol.
    * \returns awaitable pointer to UDP socket manager.
    * \throws std::runtime_error if connection rejected or any other connection error occured.
    */
    virtual net::awaitable<std::shared_ptr<SocketManager>> udp_connecting_to(const Address& address, Port port) = 0;
};

}
