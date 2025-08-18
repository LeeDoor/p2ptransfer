#include "transferer_impl.hpp"

namespace p2ptransfer {
namespace model {

TransfererImpl::TransfererImpl(
    ContextPtr context,
    ThreadWrapperPtr thread_wrapper,
    SocketManagerBuilderPtr socket_builder,
    FileWriterBuilderPtr file_writer_builder)
    : context_{context}
    , thread_wrapper_{thread_wrapper}
    , socket_builder_{socket_builder}
    , file_writer_builder_{file_writer_builder}
{}

net::awaitable<void> TransfererImpl::connect(const Address& address, Port port, const Filename& filename) {
    try {
        auto socket = co_await socket_builder_->tcp_connecting_to(address, port);
        callback()->connected(socket->get_remote_endpoint().address, socket->get_remote_endpoint().port);
        auto file_writer = file_writer_builder_->create_file_writer(callback(), socket);
    } catch (const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
        throw;
    }
    co_return;
}

void TransfererImpl::transfer_file(const Address& address, Port port, const Filename& filename) {
    if(!thread_wrapper_->is_running()) {
        net::co_spawn(*context_, connect(address, port, filename), net::detached);
        thread_wrapper_->execute([this]{
            context_->run();
            context_->restart();
        });
    }
}

}
}
