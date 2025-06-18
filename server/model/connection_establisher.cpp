#include "connection_establisher.hpp"
#include "common_types.hpp"
#include "file_processor.hpp"
#include "logger.hpp"

void ConnectionEstablisher::try_join_context_thread() {
    if(context_thread_.joinable()) {
        context_thread_.join();
    }
}
void ConnectionEstablisher::run_context_thread() {
    context_thread_ = std::thread([this] {
        context_.run();
        context_.restart();
        is_running_ = false;
    });
}
ConnectionEstablisher::~ConnectionEstablisher() {
    if(is_running_) {
        context_.stop();
    }
    try_join_context_thread();
}
void ConnectionEstablisher::listen_if_not_already(Port port) {
    if(is_running_) return;
    try_join_context_thread();
    is_running_ = true;
    co_spawn(context_, listen_async(port), net::detached);
    run_context_thread();
    return;
}
net::awaitable<void> ConnectionEstablisher::listen_async(Port port) {
    SockPtr tcp_socket = co_await get_connection(port);
    auto callback = callback_.lock();
    if(!callback) co_return;
    if(!tcp_socket) {
        Logger::log() << "failed to open socket." << std::endl;
        callback->cant_open_socket();
        co_return;
    }
    Address remote_address = tcp_socket->remote_endpoint().address().to_string();
    Port remote_port = tcp_socket->remote_endpoint().port();
    Logger::log() << "connected from " << remote_address << ":" << remote_port << std::endl;
    callback->connected(remote_address, remote_port);
    FileProcessor handler(context_, std::move(tcp_socket));
    handler.set_callback(static_pointer_cast<IFileTransferCallback>(callback));
    if(co_await handler.read_remote_file()) {
        callback->connection_aborted(remote_address, remote_port);
    } else {
        callback->file_transfered();
    }
}
net::awaitable<SockPtr> ConnectionEstablisher::get_connection(Port port) {
    tcpip::endpoint endpoint(tcpip::v4(), port);
    try {
        SocketCloser socketCloser = [] (tcpip::socket* socket) {
            ErrorCode ec;
            socket->shutdown(tcpip::socket::shutdown_both, ec);
            socket->close();
        };
        tcpip::acceptor acceptor(context_, endpoint);
        SockPtr tcp_socket (new tcpip::socket(context_), socketCloser);
        Logger::log() << "listening for a connection on " << endpoint.address() << ":" << endpoint.port() << std::endl;
        if(auto callback = callback_.lock()) {
            callback->connection_opened(endpoint.address().to_string(), endpoint.port());
        }
        auto [ec] = co_await acceptor.async_accept(*tcp_socket.get(), net::as_tuple(net::use_awaitable));
        if(ec) {
            Logger::log() << "failed to accept connection: " << ec.what() << std::endl;
            co_return nullptr;
        }
        co_return tcp_socket;
    } catch (const std::exception& ex) {
        Logger::log() << "failed while creating acceptor: " << ex.what() << std::endl;
        co_return nullptr;
    }
}
