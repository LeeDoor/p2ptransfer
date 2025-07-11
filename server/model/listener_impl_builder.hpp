#pragma once
#include "file_processor_impl_builder.hpp"
#include "listener_builder.hpp"
#include "listener_impl.hpp"
#include "thread_wrapper_impl.hpp"
#include "socket_manager_impl_factory.hpp"

class ListenerImplBuilder : public ListenerBuilder {
public:
    std::shared_ptr<Listener> create_listener() override {
        std::shared_ptr<net::io_context> context;
        return std::make_shared<ListenerImpl>(
            context,
            std::make_shared<ThreadWrapperImpl>(),
            std::make_shared<SocketManagerImplFactory>(context),
            std::make_shared<FileProcessorImplBuilder>()
        );
    }
};
