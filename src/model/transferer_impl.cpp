#include "transferer_impl.hpp"
#include "thread_wrapper.hpp"
#include "socket_manager_builder.hpp"
#include "socket_manager.hpp"
#include "file_writer_builder.hpp"
#include "file_writer.hpp"

namespace p2ptransfer {
namespace model {

TransfererImpl::TransfererImpl(
    ContextPtr context,
    ThreadWrapperPtr thread_wrapper,
    SocketManagerBuilderPtr socket_builder,
    FileWriterBuilderPtr file_writer_builder)
    : context_{context}
    , thread_wrapper_{thread_wrapper}
    , socket_manager_builder_{socket_builder}
    , file_writer_builder_{file_writer_builder}
{}
TransfererImpl::~TransfererImpl() {
    stop();
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

net::awaitable<void> TransfererImpl::connect_and_send(Address address, Port port, Filename filename) {
    try {
        auto socket = co_await connect(address, port);
        co_await send_file(socket, filename);
    } catch (const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
    }
    socket_manager_ = nullptr;
}

net::awaitable<std::shared_ptr<SocketManager>> TransfererImpl::connect(const Address& address, Port port) {
    try {
        socket_manager_ = co_await socket_manager_builder_->tcp_connecting_to(address, port);
        auto rem_endpoint = socket_manager_->get_remote_endpoint();
        callback()->connected(rem_endpoint.address, rem_endpoint.port);
        co_return socket_manager_;
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

void TransfererImpl::stop() {
    socket_manager_builder_->cancel();
    if(socket_manager_) {
        socket_manager_->stop();
    }
}

}
}
