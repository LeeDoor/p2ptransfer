#pragma once
#include "address_gatherer_impl.hpp"
#include "file_reader_impl_builder.hpp"
#include "file_writer_impl_builder.hpp"
#include "listener_impl.hpp"
#include "socket_manager_impl_builder.hpp"
#include "thread_wrapper_impl.hpp"
#include "transferer_impl.hpp"

namespace p2ptransfer {
namespace model {

/// Creates main Model objects and hides DI overwhelming.
class ModelBuilder {
public:
    ModelBuilder(std::shared_ptr<net::io_context> context) :
        context_{context}
    {}

    std::shared_ptr<ListenerImpl> create_listener() {
        return std::make_shared<ListenerImpl>(
            context_,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context_),
            std::make_shared<FileReaderImplBuilder>()
        );
    }

    std::shared_ptr<TransfererImpl> create_transferer() {
        return std::make_shared<TransfererImpl>(
            context_,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context_),
            std::make_shared<FileWriterImplBuilder>()
        );
    }

    std::shared_ptr<AddressGatherer> create_address_gatherer() {
        return std::make_shared<AddressGathererImpl>(
            context_,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context_)
        );
    }
private:
    std::shared_ptr<net::io_context> context_;
};

}
}
