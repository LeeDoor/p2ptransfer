#pragma once

#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
class Presenter;
class IView {
public:
    virtual int run() = 0;

    virtual void set_presenter(std::shared_ptr<Presenter> presenter) = 0;
    virtual void listen() = 0;
    virtual void set_progressbar(double persent) = 0;
    virtual void set_address(const Address& address) = 0;
    virtual Port get_port() = 0;
    virtual bool verify_file(SendRequest send_request) = 0;

    virtual void on_connection_opened(const Address& address, Port port) = 0;
    virtual void on_connection_aborted(const Address& address, Port port) = 0;
    virtual void on_connected(const Address& address, Port port) = 0;
    virtual void on_file_transfered() = 0;
protected:
    std::string filesize_to_string(Filesize filesize) {
        if(filesize < (1 << 10)) {
            return std::to_string(filesize) + "B";
        }
        double dsize = filesize;
        if(dsize < (1 << 20)) {
            return std::format("{:.1f}", dsize / (1 << 10)) + "KB";
        }
        if(dsize < (1 << 30)) {
            return std::format("{:.1f}", dsize / (1 << 20)) + "MB";
        }
        if(dsize < (1ll << 40)) {
            return std::format("{:.1f}", dsize / (1 << 30)) + "GB";
        }
        return "NIGO";
    }
};
