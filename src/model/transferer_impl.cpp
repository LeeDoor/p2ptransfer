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

net::awaitable<std::shared_ptr<SocketManager>> TransfererImpl::connect(const Address& address, Port port) {
    try {
        auto socket = co_await socket_builder_->tcp_connecting_to(address, port);
        auto rem_endpoint = socket->get_remote_endpoint();
        callback()->connected(rem_endpoint.address, rem_endpoint.port);
        co_return socket;
    } catch (const std::exception& ex) {
        callback()->cant_open_socket();
        throw;
    }
}

net::awaitable<void> TransfererImpl::send_file(std::shared_ptr<SocketManager> socket, const Filename& filename){
    SocketManager::Endpoint rem_endpoint{"Unloaded address", 404};
    try {
        rem_endpoint = socket->get_remote_endpoint();
        auto file_writer = file_writer_builder_->create_file_writer(callback(), socket);
        co_await file_writer->write_file(filename);
        callback()->file_transfered();
    } catch (const std::exception& ex) {
        callback()->connection_aborted(rem_endpoint.address, rem_endpoint.port);
        throw;
    }
    co_return;
}

net::awaitable<void> TransfererImpl::connect_and_send(const Address& address, Port port, const Filename& filename) {
    try {
        auto socket = co_await connect(address, port);
        co_await send_file(socket, filename);
    } catch (const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
    }
}

void TransfererImpl::transfer_file(const Address& address, Port port, const Filename& filename) {
    if(!thread_wrapper_->is_running()) {
        net::co_spawn(*context_, connect_and_send(address, port, filename), net::detached);
        thread_wrapper_->execute([this]{
            context_->run();
            context_->restart();
        });
    }
}

}
}
