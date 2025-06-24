#pragma once

#include "address.hpp"
#include "callback.hpp"
#include "port.hpp"
#include "request_structures.hpp"
#include "view_callback.hpp"
class IView : public WithCallback<IViewCallback> {
public:
    virtual int run() = 0;

    virtual void show_address(const Address& address) = 0;

    virtual void show_connected(const Address& address, Port port) = 0;
    virtual void show_socket_error() = 0;

    virtual void update_progressbar_status(double persent) = 0;
    virtual bool ask_file_verification(const Filename& filename, Filesize filesize) = 0;
    virtual void show_file_success() = 0;
    virtual void show_connection_aborted(const Address& address, Port port) = 0;
};
