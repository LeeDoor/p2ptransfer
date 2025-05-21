#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/awaitable.hpp>
#include "network_headers.hpp"
#include "send_request_data.hpp"

class ConnectionHandler {
public:
    ConnectionHandler(net::io_context& ctx, tcpip::socket&& socket)
        : io_(ctx), socket_(std::move(socket)){}
    net::awaitable<void> handle();
private:
    net::awaitable<std::optional<SendRequestData>> handle_send_request(std::string& buffer);
    net::awaitable<std::optional<std::string>> async_readline(std::string& buffer);
    net::awaitable<std::optional<std::string>> handle_method(std::string& buffer);
    net::awaitable<std::optional<std::string>> handle_filename(std::string& buffer);
    net::awaitable<std::optional<size_t>> handle_filesize(std::string& buffer);

    static constexpr std::string REQUEST_METHOD = "REQUEST";
    static constexpr size_t REQUEST_METHOD_SIZE = std::string_view(REQUEST_METHOD).size();
    static constexpr std::string FILE_HEADER = "FILE";
    static constexpr size_t FILE_HEADER_SIZE = std::string_view(FILE_HEADER).size();
    static constexpr std::string SIZE_HEADER = "SIZE";
    static constexpr size_t SIZE_HEADER_SIZE = std::string_view(SIZE_HEADER).size();

    net::io_context& io_;
    tcpip::socket socket_;
};
