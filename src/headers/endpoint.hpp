#pragma once
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {

struct Endpoint {
    Address address;
    Port port;
};

}

