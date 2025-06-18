#pragma once

#include "callback.hpp"
#include "gather_address_callback.hpp"
class IAddressGatherer : public WithCallback<IGatherAddressCallback> {
public:
    virtual ~IAddressGatherer() = default;
    virtual void gather_local_address() = 0;
};
