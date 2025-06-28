#pragma once
#include "address_gatherer.hpp"
#include "common_types.hpp"
#include "thread_wrapper.hpp"

class AddressGathererImpl : public AddressGatherer {
public:
    AddressGathererImpl(std::shared_ptr<ThreadWrapper> thread_wrapper);

    void gather_local_address() override;
private:
    std::shared_ptr<ThreadWrapper> thread_wrapper_;
    net::io_context context_;
};
