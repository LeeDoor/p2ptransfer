#pragma once
#include "address_gatherer_impl.hpp"
#include "file_processor_impl_builder.hpp"
#include "listener_impl.hpp"
#include "socket_manager_impl_builder.hpp"
#include "thread_wrapper_impl.hpp"

namespace p2ptransfer {
namespace server {
namespace model {

/// Creates main Model objects and hides DI overwhelming.
class ModelBuilder {
public:
    static std::shared_ptr<ListenerImpl> create_listener() {
        auto context = std::make_shared<net::io_context>();
        return std::make_shared<ListenerImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context),
            std::make_shared<FileProcessorImplBuilder>()
        );
    }

    static std::shared_ptr<AddressGatherer> create_address_gatherer() {
        auto context = std::make_shared<net::io_context>();
        return std::make_shared<AddressGathererImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context)
        );
    }

};

}
}
}
