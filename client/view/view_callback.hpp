#pragma once

#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
namespace general {
namespace client {
namespace view {

class ViewCallback {
public:
    virtual void send_file_to(Filename filename, Address address, Port port) = 0;
};

}
}
}
