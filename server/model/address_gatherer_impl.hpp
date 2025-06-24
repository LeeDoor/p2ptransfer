#pragma once
#include "address_gatherer.hpp"
#include "common_types.hpp"

class AddressGathererImpl : public AddressGatherer {
public:
    void gather_local_address() override;
private:
    net::io_context context_;
    std::jthread gather_thread_;
};
