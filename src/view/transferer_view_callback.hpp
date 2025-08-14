#pragma once

#include "address.hpp"
#include "port.hpp"
#include "request_structures.hpp"
namespace p2ptransfer {
namespace view {

class TransfererViewCallback {
public:
    virtual ~TransfererViewCallback() = default;

    virtual void transfer(const Address& address, Port port, const Filename& filename) = 0;
};

}
}
