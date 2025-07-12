#pragma once

#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
class FileTransferCallback {
public:
    virtual void set_progressbar(double present) = 0;
    virtual bool verify_file(const Filename& filename, Filesize filesize) = 0;
    virtual void file_transfered() = 0;
    virtual void connection_aborted(const Address& address, Port port) = 0;
    virtual ~FileTransferCallback() = default;
};
