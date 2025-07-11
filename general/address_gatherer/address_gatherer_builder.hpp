#pragma once

#include "address_gatherer.hpp"
class AddressGathererBuilder {
public:
    virtual std::shared_ptr<AddressGatherer> create_address_gatherer() = 0;
};
