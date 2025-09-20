#pragma once
#include "address_gatherer_impl.hpp"
#include "context_wrapper.hpp"
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
    std::shared_ptr<ListenerImpl> create_listener() {
        ContextWrapper context;
        return std::make_shared<ListenerImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context),
            std::make_shared<FileReaderImplBuilder>()
        );
    }

    std::shared_ptr<TransfererImpl> create_transferer() {
        ContextWrapper context;
        return std::make_shared<TransfererImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context),
            std::make_shared<FileWriterImplBuilder>()
        );
    }

    std::shared_ptr<AddressGatherer> create_address_gatherer() {
        ContextWrapper context;
        return std::make_shared<AddressGathererImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplBuilder>(context)
        );
    }
};

}
}
