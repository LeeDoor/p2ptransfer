#pragma once

#include "address.hpp"
#include "port.hpp"
namespace general {
namespace server {
namespace presenter {

/// The union of \ref ConnectionStatusCallback and \ref FileTransferCallback.
class NetworkStatusCallback {
public:
    virtual ~NetworkStatusCallback() = default;

    /*! \brief While file sharing, notifying about sending progress.
     * \param persent the floating point number in [0;100] segment 
    */
    virtual void set_progressbar(double persent) = 0;
#if 0
    /// Asks for user input about file gathering confirmation.
    virtual bool verify_file(const Filename& filename, Filesize filesize) = 0;
#endif
    /// Called when file successfully gathered.
    virtual void file_transfered() = 0;
    /// Called if something went wrong with gathering a file.
    virtual void connection_aborted(const Address& address, Port port) = 0;

    /// Notification called if connection established successfully 
    /// with given address and port.
    virtual void connected(const Address& address, Port port) = 0;
    /// Notification called if failed to establish connection for
    /// any reason.
    virtual void cant_open_socket() = 0;
};

}
}
}
