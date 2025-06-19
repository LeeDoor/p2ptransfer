#pragma once
#include "i_address_gatherer.hpp"
#include "common_types.hpp"

class AddressGathererImpl : public IAddressGatherer {
public:
    void gather_local_address() override;
private:
    net::io_context context_;
    std::jthread gather_thread_;
};
