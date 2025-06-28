#pragma once
#include "address_gatherer.hpp"
#include "common_types.hpp"
#include "model_factory.hpp"
#include "thread_wrapper.hpp"

class AddressGathererImpl : public AddressGatherer {
public:
    AddressGathererImpl(std::shared_ptr<ModelFactory> factory);

    void gather_local_address() override;
private:
    net::awaitable<void> gather_async();
    std::shared_ptr<ModelFactory> factory_;
    std::shared_ptr<ThreadWrapper> thread_wrapper_;
    net::io_context context_;
};
