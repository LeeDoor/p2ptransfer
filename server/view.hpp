#pragma once

#include "address.hpp"
#include "port.hpp"
class Presenter;
class IView {
public:
    virtual int run() = 0;

    virtual void set_presenter(std::shared_ptr<Presenter> presenter) = 0;
    virtual void listen() = 0;
    virtual void set_progressbar(double persent) = 0;
    virtual void set_address(const Address& address) = 0;
    virtual Port get_port() = 0;

    virtual void on_connection_opened(const Address& address, Port port) = 0;
    virtual void on_connection_aborted(const Address& address, Port port) = 0;
    virtual void on_connected(const Address& address, Port port) = 0;
};
