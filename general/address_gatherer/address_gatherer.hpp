#pragma once

#include "callback.hpp"
#include "gather_address_callback.hpp"
class AddressGatherer : public WithCallback<GatherAddressCallback> {
public:
    virtual void gather_local_address() = 0;
    virtual void stop() = 0;
    virtual ~AddressGatherer() = default;
};
