#pragma once
#include "address.hpp"
#include "callback.hpp"
#include "port.hpp"

namespace p2ptransfer {
namespace presenter {

/// The union of \ref ConnectionStatusCallback and \ref FileTransferCallback.
class NetworkStatusCallback {
public:
    virtual ~NetworkStatusCallback() = default;

    /// Notification called if connection established successfully 
    /// with given address and port.
    virtual void connection_established(const Address& address, Port port) = 0;
    /// Called when file successfully gathered.
    virtual void transfer_succeed() = 0;
    /// Called if something went wrong with gathering a file.
    virtual void transfer_failed(const Address& address, Port port, std::string reason) = 0;
    /// Notification called if failed to establish connection for
    /// any reason.
    virtual void cant_open_socket(std::string reason) = 0;
    /*! \brief While file sharing, notifying about sending progress.
     * \param persent the floating point number in [0;100] segment 
    */
    virtual void set_progressbar(size_t bytes_remaining, size_t filesize) = 0;
};

}
using WithNetworkCallback = WithCallback<presenter::NetworkStatusCallback>;
}
