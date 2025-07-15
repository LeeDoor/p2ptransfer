#pragma once

#include "address.hpp"
namespace general {

class GatherAddressCallback {
public:
    virtual void set_address(const Address& address) = 0;
    virtual ~GatherAddressCallback() = default;
};

}
