#pragma once

#include "address_gatherer.hpp"
#include "listener.hpp"
class ModelFactory {
public:
    virtual std::shared_ptr<Listener> create_listener() = 0;
    virtual std::shared_ptr<AddressGatherer> create_address_gatherer() = 0;
};
