#pragma once

#include "address.hpp"
namespace general {
namespace address_gatherer {

class GatherAddressCallback {
public:
    virtual void set_address(const Address& address) = 0;
    virtual ~GatherAddressCallback() = default;
};

}
}
