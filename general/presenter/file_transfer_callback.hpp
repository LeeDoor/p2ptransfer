#pragma once
#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"

namespace general {
namespace presenter {

/// Interface for file sharing phase status notification.
class FileTransferCallback {
public:
    virtual ~FileTransferCallback() = default;

    /*! \brief While file sharing, notifying about sending progress.
     * \param persent the floating point number in [0;100] segment 
    */
    virtual void set_progressbar(double persent) = 0;
    /// Asks for user input about file gathering confirmation.
    virtual bool verify_file(const Filename& filename, Filesize filesize) = 0;
    /// Called when file successfully gathered.
    virtual void file_transfered() = 0;
    /// Called if something went wrong with gathering a file.
    virtual void connection_aborted(const Address& address, Port port) = 0;
};

}
}
