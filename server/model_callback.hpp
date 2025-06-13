#pragma once

#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
class IModelCallback {
public:
    virtual void connection_opened(const Address& address, Port port) = 0;
    virtual void connected(const Address& address, Port port) = 0;
    virtual void connection_aborted(const Address& address, Port port) = 0;
    virtual void file_transfered() = 0;
    virtual void set_progressbar_status(double present) = 0;
    virtual bool verify_file(SendRequest send_request) = 0;
    virtual void cant_open_socket() = 0;
    virtual void set_address(const Address& address) = 0;
};
