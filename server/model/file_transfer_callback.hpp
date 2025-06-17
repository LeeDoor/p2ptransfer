#pragma once

#include "request_structures.hpp"
class IFileTransferCallback {
public:
    virtual void set_progressbar(double present) = 0;
    virtual bool verify_file(SendRequest send_request) = 0;
};
