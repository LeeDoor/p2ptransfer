#pragma once
#include "address_gatherer_builder.hpp"
#include "address_gatherer_impl.hpp"
#include "socket_manager_impl_factory.hpp"
#include "thread_wrapper_impl.hpp"

class AddressGathererImplBuilder : public AddressGathererBuilder {
public:
    std::shared_ptr<AddressGatherer> create_address_gatherer() override {
        std::shared_ptr<net::io_context> context;
        return std::make_shared<AddressGathererImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplFactory>(context)
        );
    }
};
