#pragma once

#include "address.hpp"
#include "connection_status_callback.hpp"
#include "request_structures.hpp"
class IModelCallback : public IConnectionStatusCallback {
public:
    virtual ~IModelCallback() = default;
    virtual void set_progressbar_status(double present) = 0;
    virtual bool verify_file(SendRequest send_request) = 0;
    virtual void set_address(const Address& address) = 0;
};
