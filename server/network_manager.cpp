#include "network_manager.hpp"
#include "common_types.hpp"
#include "connection_handler.hpp"
#include "logger.hpp"
#include "presenter.hpp"
NetworkManager::~NetworkManager() {
    if(is_running_) {
        context_.stop();
        context_thread_.join();
    }
}
int NetworkManager::init(Port port) {
    if(is_running_) return 1;
    if(context_thread_.joinable()) 
        context_thread_.join();
    is_running_ = true;
    co_spawn(context_, listen(port), net::detached);
    context_thread_ = std::thread([this] {
        context_.run();
        context_.restart();
        Logger::log() << "context_ finished.\n";
        is_running_ = false;
    });
    return 0;
}
void NetworkManager::set_presenter(std::shared_ptr<Presenter> presenter) {
    presenter_ = presenter;
}
net::awaitable<void> NetworkManager::listen(Port port) {
    SockPtr tcp_socket = co_await get_connection(port);
    if(!tcp_socket) {
        Logger::log() << "failed to open socket." << std::endl;
        if(auto presenter = presenter_.lock()) {
            presenter->cant_open_socket();
        }
        co_return;
    }
    Address remote_address = tcp_socket->remote_endpoint().address().to_string();
    Port remote_port = tcp_socket->remote_endpoint().port();
    Logger::log() << "connected from " << remote_address << ":" << remote_port << std::endl;
    if(auto presenter = presenter_.lock()) {
        presenter->connected(remote_address, remote_port);
    }
    ConnectionHandler handler(context_, std::move(tcp_socket), presenter_);
    if(co_await handler.handle()) {
        if(auto presenter = presenter_.lock()) {
            presenter->connection_aborted(remote_address, remote_port);
        }
    } else {
        if(auto presenter = presenter_.lock()) {
            presenter->file_transfered();
        }
    }
}
net::awaitable<SockPtr> NetworkManager::get_connection(Port port) {
    tcpip::endpoint endpoint(tcpip::v4(), port);
    try {
        SocketCloser socketCloser = [] (tcpip::socket* socket) {
            ErrorCode ec;
            socket->shutdown(tcpip::socket::shutdown_both, ec);
            socket->close();
        };
        tcpip::acceptor acceptor(context_, endpoint);
        std::unique_ptr<tcpip::socket, SocketCloser> tcp_socket (new tcpip::socket(context_), socketCloser);
        Logger::log() << "listening for a connection on " << endpoint.address() << ":" << endpoint.port() << std::endl;
        if(auto presenter = presenter_.lock()) {
            presenter->connection_opened(endpoint.address().to_string(), endpoint.port());
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

