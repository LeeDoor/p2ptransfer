#include "listener_impl.hpp"
#include "logger.hpp"
#include "thread_wrapper.hpp"
#include "file_reader_builder.hpp"
#include "file_reader.hpp"
#include "socket_manager_builder.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

ListenerImpl::ListenerImpl(ContextPtr context,
                           ThreadWrapperPtr thread_wrapper,
                           SocketManagerBuilderPtr socket_manager_builder,
                           FileReaderBuilderPtr file_reader_builder) :
    thread_wrapper_(thread_wrapper),
    context_(context),
    socket_manager_builder_(socket_manager_builder),
    file_reader_builder_(file_reader_builder)
{}
ListenerImpl::~ListenerImpl() {
    context_->stop();
}

void ListenerImpl::listen_if_not_already(Port port) {
    if(!thread_wrapper_->is_running()) { 
        spawn_and_run(port);
    }
}

void ListenerImpl::spawn_and_run(Port port) {
    spawn_listen_coroutine(port);
    thread_wrapper_->execute([this] {
        context_->run();
        context_->restart();
    });
}

void ListenerImpl::spawn_listen_coroutine(Port port) {
    net::co_spawn(*context_, listen_async(port), net::detached);
}

net::awaitable<void> ListenerImpl::listen_async(Port port) {
    try {
        auto socket_manager = co_await connect_and_listen(port);
        auto file_reader = file_reader_builder_->create_file_reader(
            WithNetworkCallback::callback(), WithListenerCallback::callback(), socket_manager);
        co_await file_reader->try_read_file();
    } catch(const std::exception& ex) {
        Logger::log() << ex.what() << std::endl;
    }
}

net::awaitable<ListenerImpl::SocketManagerPtr> ListenerImpl::connect_and_listen(Port port) {
    try {
        auto socket_manager = co_await socket_manager_builder_->tcp_listening_at(port);
        auto endpoint = socket_manager->get_remote_endpoint();
        WithNetworkCallback::callback()->connected(endpoint.address, endpoint.port);
        co_return socket_manager;
    } catch(const std::exception& ex) {
        WithNetworkCallback::callback()->cant_open_socket();
        throw;
    }
}

}
}
