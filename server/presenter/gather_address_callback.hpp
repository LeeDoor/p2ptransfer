#pragma once

#include "address.hpp"
class GatherAddressCallback {
public:
    virtual void set_address(const Address& address) = 0;
};
