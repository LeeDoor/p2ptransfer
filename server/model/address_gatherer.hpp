#pragma once
#include "common_types.hpp"
#include "gather_address_callback.hpp"
#include "callback.hpp"

class AddressGatherer: public WithCallback<std::weak_ptr<IGatherAddressCallback>> {
public:
    ~AddressGatherer();
    void gather_local_address();
private:
    net::io_context context_;
    std::thread gather_thread_;
};
