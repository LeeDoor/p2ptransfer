#pragma once

#include "address.hpp"
#include "port.hpp"
#include "view_callback_dependent.hpp"
#include "request_structures.hpp"
#include "connection_status_callback.hpp"
class Presenter;
class IView : public IConnectionStatusCallback, public ViewCallbackDependent {
public:
    virtual ~IView() = default;
    virtual int run() = 0;
    virtual void listen() = 0;
    virtual void set_progressbar(double persent) = 0;
    virtual void set_address(const Address& address) = 0;
    virtual Port get_port() = 0;
    virtual bool verify_file(SendRequest send_request) = 0;
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
        return std::format("{:.2f}", dsize / (1ll << 40)) + "TB";
    }
};
