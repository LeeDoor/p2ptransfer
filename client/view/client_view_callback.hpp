#pragma once
#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
#include "view_callback.hpp"

namespace general {
namespace client {
namespace view {

class ClientViewCallback : public general::view::ViewCallback {
public:
    virtual ~ClientViewCallback() = default;

    virtual void send_file_to(Filename filename, Address address, Port port) = 0;
};

}
}
}
