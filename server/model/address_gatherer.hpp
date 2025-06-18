#pragma once
#include "i_address_gatherer.hpp"
#include "common_types.hpp"

class AddressGatherer: public IAddressGatherer {
public:
    ~AddressGatherer();
    void gather_local_address() override;
private:
    net::io_context context_;
    std::thread gather_thread_;
};
