#pragma once
#include "view_callback.hpp"
#include "callback.hpp"

namespace general {
namespace client {
namespace view {

class View : public WithCallback<ViewCallback> {
public:
    virtual int run() = 0;
    virtual void stop() = 0;

    virtual void show_address(Address address) = 0;
    virtual void connected(Address address, Port port) = 0;
    virtual void connection_failed(Address address, Port port) = 0;
    virtual void update_progressbar(double percent) = 0;
    virtual void file_transfered() = 0;
    virtual void access_denied() = 0;
    virtual void connection_aborted() = 0;
};

}
}
}
