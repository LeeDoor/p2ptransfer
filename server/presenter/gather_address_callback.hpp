#pragma once

#include "address.hpp"
class IGatherAddressCallback {
public:
    virtual void set_address(const Address& address) = 0;
};
