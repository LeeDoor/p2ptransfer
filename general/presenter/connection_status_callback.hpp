#pragma once
#include "address.hpp"
#include "port.hpp"

namespace general {
namespace presenter {

/// Interface for connecting phase status notification.
class ConnectionStatusCallback {
public:
    virtual ~ConnectionStatusCallback() = default;

    /// Notification called if connection established successfully 
    /// with given address and port.
    virtual void connected(const Address& address, Port port) = 0;
    /// Notification called if failed to establish connection for
    /// any reason.
    virtual void cant_open_socket() = 0;
};

}
}
