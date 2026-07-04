#pragma once
#include "address_gatherer_impl.hpp"
#include "file_reader_impl_builder.hpp"
#include "file_writer_impl_builder.hpp"
#include "listener_impl.hpp"
#include "socket_manager_impl_builder.hpp"
#include "transferer_impl.hpp"
#include "listeners_lookup_impl.hpp"
#include "socket_manager_multicast_impl.hpp"

namespace p2ptransfer {
namespace model {

template<typename T>
concept ThreadWrapperT = std::is_base_of_v<ThreadWrapper, T>;

/// Creates main Model objects and hides DI overwhelming.
template<ThreadWrapperT ThreadWrapperType>
class ModelBuilder {
public:
    std::shared_ptr<ListenerImpl> create_listener() {
        ContextPtr context;
        return std::make_shared<ListenerImpl>(
            context,
            std::make_shared<ThreadWrapperType>(),
            std::make_shared<SocketManagerImplBuilder>(context),
            std::make_shared<FileReaderImplBuilder>()
        );
    }

    std::shared_ptr<TransfererImpl> create_transferer() {
        ContextPtr context;
        return std::make_shared<TransfererImpl>(
            context,
            std::make_shared<ThreadWrapperType>(),
            std::make_shared<SocketManagerImplBuilder>(context),
            std::make_shared<FileWriterImplBuilder>()
        );
    }

    std::shared_ptr<AddressGatherer> create_address_gatherer() {
        ContextPtr context;
        return std::make_shared<AddressGathererImpl>(
            context,
            std::make_shared<ThreadWrapperType>(),
            std::make_shared<SocketManagerImplBuilder>(context)
        );
    }

    std::shared_ptr<ListenersLookupImpl> create_listeners_lookup() {
        ContextPtr context;
        return std::make_shared<ListenersLookupImpl>(
            context,
            std::make_shared<SocketManagerMulticastImpl>(context),
            std::make_shared<ThreadWrapperType>()
        );
    }
};

}
}
