#pragma once

#include "common_types.hpp"
#include "model_address_gather.hpp"
class AddressGatherer : public IModelAddressGatherer {
public:
    ~AddressGatherer();
    void gather_local_address();
private:
    net::io_context context_;
    std::thread gather_thread_;
};
