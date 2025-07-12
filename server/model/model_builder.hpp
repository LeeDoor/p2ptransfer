#pragma once
#include "address_gatherer_impl.hpp"
#include "file_processor_impl_builder.hpp"
#include "listener_impl.hpp"
#include "socket_manager_impl_builder.hpp"
#include "thread_wrapper_impl.hpp"

namespace general {
namespace model {

class ModelBuilder {
public:
    static std::shared_ptr<ListenerImpl> create_listener() {
        auto context = std::make_shared<net::io_context>();
        return std::make_shared<ListenerImpl>(
            context,
            std::make_shared<thread_wrapper::ThreadWrapperImpl>(),
            std::make_shared<socket_manager::SocketManagerImplBuilder>(context),
            std::make_shared<FileProcessorImplBuilder>()
        );
    }

    static std::shared_ptr<address_gatherer::AddressGatherer> create_address_gatherer() {
        auto context = std::make_shared<net::io_context>();
        return std::make_shared<address_gatherer::AddressGathererImpl>(
            context,
            std::make_shared<thread_wrapper::ThreadWrapperImpl>(),
            std::make_shared<socket_manager::SocketManagerImplBuilder>(context)
        );
    }

};

}
}
