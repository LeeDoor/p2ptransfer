#pragma once
#include "i_address_gatherer.hpp"
#include "common_types.hpp"
#include "gather_address_callback.hpp"
#include "callback.hpp"

class AddressGatherer: public IAddressGatherer, public WithCallback<std::weak_ptr<IGatherAddressCallback>> {
public:
    ~AddressGatherer();
    void gather_local_address() override;
private:
    net::io_context context_;
    std::thread gather_thread_;
};
